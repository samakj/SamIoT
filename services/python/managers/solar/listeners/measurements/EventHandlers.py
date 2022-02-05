import logging
from typing import Dict, Optional, Union

from aiohttp import ClientSession
from aiohttp.http_websocket import WSMessage

from shared.python.extensions.aiohttp.websocket import EventCallback, EventWithMessageCallback, Websocket
from shared.python.models.Measurement import Measurement
from shared.python.models.Metric import Metric
from shared.python.models.Device import Device
from shared.python.clients.iot import IoTClient
from shared.python.json import parse_json

LOG = logging.getLogger()

BATTERY_TAG = "battery"
INVERTER_TAG = "inverter"
SOLAR_DEVICES_NO = 8
EXCESS_POWER_DEVICES_NO = 6

async def set_relay(endpoint: str, state: bool) -> bool:
    async with ClientSession() as session:
        async with session.post(endpoint, data="true" if state else "false") as response:
            return parse_json(await response.text()).get("on")

class EventHandlers:
    websocket_store: Websocket
    iot_client: IoTClient
    metrics: Dict[int, Metric]
    metrics_by_name: Dict[str, Metric]
    devices: Dict[int, Device]
    
    inverter_status: Optional[bool]
    solar_devices_status: Optional[bool]
    excess_power_devices_status: Optional[bool]

    def __init__(
        self,
        iot_client: IoTClient
    ) -> None:
        self.iot_client = iot_client
        self.metrics = {}
        self.metrics_by_name = {}
        self.devices = {}
        
        self.inverter_status = None
        self.solar_devices_status = None
        self.excess_power_devices_status = None

    async def update_metrics(self) -> None:
        metrics = {}
        metrics_by_name = {}

        for metric in await self.iot_client.metrics.get_metrics():
            metrics[metric.id] = metric
            metrics_by_name[metric.name] = metric

        self.metrics = metrics
        self.metrics_by_name = metrics_by_name

    async def get_metric(self, identifier: Union[str, int]) -> Optional[Metric]:
        if isinstance(identifier, int):
            if identifier not in self.metrics.keys():
                await self.update_metrics()
            return self.metrics.get(identifier)

        if (identifier not in self.metrics_by_name.keys()):
            await self.update_metrics()
        return self.metrics_by_name.get(identifier)

    async def update_devices(self) -> None:
        devices = {}

        for device in await self.iot_client.devices.get_devices():
            devices[device.id] = device

        self.devices = devices

    async def get_device(self, identifier: int) -> Optional[Device]:
        if identifier not in self.devices.keys():
            await self.update_devices()
        return self.devices.get(identifier)
    
    async def get_relay_states(self) -> None:
        on_metric = await self.get_metric("on")

        if on_metric is None:
            LOG.error("Failed to get on metric.")
            return
        
        for measurement in await self.iot_client.measurements.get_latest_measurements(
            tags="solar",
            metric_id=on_metric.id
        ):
            if INVERTER_TAG in measurement.tags:
                self.inverter_status = measurement.value
                LOG.info(f"Inverter initialised as {'ON' if measurement.value else 'OFF' }")
            if f"relay{SOLAR_DEVICES_NO}" in measurement.tags:
                self.solar_devices_status = measurement.value
                LOG.info(f"Solar devices initialised as {'ON' if measurement.value else 'OFF' }")
            if f"relay{EXCESS_POWER_DEVICES_NO}" in measurement.tags:
                self.excess_power_devices_status = measurement.value
                LOG.info(f"Excess power devices initialised as {'ON' if measurement.value else 'OFF' }")
            

    def connect(self) -> EventCallback:
        async def callback(weboscket: Websocket) -> None:
            LOG.info("Websocket connected.")
            
        return callback

    def message(self) -> EventWithMessageCallback:
        async def callback(weboscket: Websocket, message: WSMessage) -> None:
            pass
        return callback

    def text(self) -> EventWithMessageCallback:
        async def callback(weboscket: Websocket, message: WSMessage) -> None:
            LOG.warning(
                f"Websocket text handler called for below, could be an error?"
            )
            LOG.warning(message.data)
        return callback

    def json(self) -> EventWithMessageCallback:
        async def callback(weboscket: Websocket, message: WSMessage) -> None:
            data = message.json(loads=parse_json)
            measurement: Measurement = Measurement.parse_obj(data["data"])
            percentage_metric = await self.get_metric("percentage")
            on_metric = await self.get_metric("on")
            
            if percentage_metric is None:
                LOG.error("Failed to get percentage metric.")
                return
            if on_metric is None:
                LOG.error("Failed to get on metric.")
                return
            
            if "solar" in measurement.tags:
                device = await self.get_device(measurement.device_id)
                
                if (
                    INVERTER_TAG in measurement.tags and
                    measurement.metric_id == on_metric.id
                ):
                    LOG.info(f"Inverter reported as {'ON' if measurement.value else 'OFF' }")
                    self.inverter_status = measurement.value
                if (
                    f"relay{SOLAR_DEVICES_NO}" in measurement.tags and
                    measurement.metric_id == on_metric.id
                ):
                    LOG.info(f"Solar devices reported as {'ON' if measurement.value else 'OFF' }")
                    self.solar_devices_status = measurement.value
                if (
                    f"relay{EXCESS_POWER_DEVICES_NO}" in measurement.tags and
                    measurement.metric_id == on_metric.id
                ):
                    LOG.info(f"Excess power devices reported as {'ON' if measurement.value else 'OFF' }")
                    self.excess_power_devices_status = measurement.value
                
                if device is None:
                    LOG.error(f"Failed to get device {measurement.device_id}")
                    return
                
                if (
                    BATTERY_TAG in measurement.tags and 
                    measurement.metric_id == percentage_metric.id
                ):
                    if measurement.value < 25:
                        if self.inverter_status is None or self.inverter_status:
                            LOG.info(
                                f"Solar batteries at {int(measurement.value)}%,"
                                " turning off inverter."
                            )
                            self.inverter_status = await set_relay(f"http://{device.ip}/inverter", False)
                        if self.solar_devices_status is None or self.solar_devices_status:
                            LOG.info(
                                f"Solar batteries at {int(measurement.value)}%,"
                                " turning off solar only devices."
                            )
                            self.solar_devices_status = await set_relay(f"http://{device.ip}/relay/{SOLAR_DEVICES_NO}", False)
                    if measurement.value > 75:
                        if self.inverter_status is None or not self.inverter_status:
                            LOG.info(
                                f"Solar batteries at {int(measurement.value)}%,"
                                " turning on inverter."
                            )
                            self.inverter_status = await set_relay(f"http://{device.ip}/inverter", True)
                        if self.solar_devices_status is None or not self.solar_devices_status:
                            LOG.info(
                                f"Solar batteries at {int(measurement.value)}%,"
                                " turning on solar only devices."
                            )
                            self.solar_devices_status = await set_relay(f"http://{device.ip}/relay/{SOLAR_DEVICES_NO}", True)
                    if measurement.value < 85:
                        if self.excess_power_devices_status is None or self.excess_power_devices_status:
                            LOG.info(
                                f"Solar batteries at {int(measurement.value)}%,"
                                " turning off excess power devices."
                            )
                            self.excess_power_devices_status = await set_relay(f"http://{device.ip}/relay/{EXCESS_POWER_DEVICES_NO}", False)
                    if measurement.value > 90:
                        if self.excess_power_devices_status is None or not self.excess_power_devices_status:
                            LOG.info(
                                f"Solar batteries at {int(measurement.value)}%,"
                                " turning off excess power devices."
                            )
                            self.excess_power_devices_status = await set_relay(f"http://{device.ip}/relay/{EXCESS_POWER_DEVICES_NO}", True)

        return callback

    def error(self) -> EventWithMessageCallback:
        async def callback(weboscket: Websocket, message: WSMessage) -> None:
            LOG.warning(f"Websocket errored.")
        return callback

    def close(self) -> EventWithMessageCallback:
        async def callback(weboscket: Websocket, message: WSMessage) -> None:
            LOG.warning(f"Websocket closed.")
        return callback

    def disconnect(self) -> EventCallback:
        async def callback(weboscket: Websocket) -> None:
            LOG.info("Websocket disconnected.")
        return callback
