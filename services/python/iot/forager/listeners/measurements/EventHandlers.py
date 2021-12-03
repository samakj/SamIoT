import logging
from datetime import datetime, timedelta
from iso8601 import parse_date
from typing import Dict, List, Optional, Union

from aiohttp.http_websocket import WSMessage

from shared.python.extensions.aiohttp.websocket import EventCallback, EventWithMessageCallback, Websocket
from shared.python.models.Device import Device
from shared.python.models.Metric import Metric
from shared.python.clients.iot import IoTClient
from shared.python.models.Measurement import Measurement
from shared.python.json import parse_json
from stores.device_websockets import DeviceWebsocketsStore

LOG = logging.getLogger()


class EventHandlers:
    websocket_store: DeviceWebsocketsStore
    iot_client: IoTClient
    metrics: Dict[int, Metric]
    metrics_by_name: Dict[str, Metric]
    metrics_by_abbreviation: Dict[str, Metric]

    def __init__(
        self,
        websocket_store: DeviceWebsocketsStore,
        iot_client: IoTClient
    ) -> None:
        self.websocket_store = websocket_store
        self.iot_client = iot_client
        self.metrics = {}

    async def update_metrics(self) -> None:
        metrics = {}
        metrics_by_name = {}
        metrics_by_abbreviation = {}

        for metric in await self.iot_client.metrics.get_metrics():
            metrics[metric.id] = metric
            metrics_by_name[metric.name] = metric
            metrics_by_abbreviation[metric.abbreviation] = metric

        self.metrics = metrics
        self.metrics_by_name = metrics_by_name
        self.metrics_by_abbreviation = metrics_by_abbreviation

    async def get_metric(self, identifier: Union[str, int]) -> Optional[Metric]:
        if isinstance(identifier, int):
            if identifier not in self.metrics.keys():
                await self.update_metrics()
            return self.metrics.get(identifier)

        if (
            identifier not in self.metrics_by_name.keys() and
            identifier not in self.metrics_by_abbreviation.keys()
        ):
            await self.update_metrics()
        return self.metrics_by_name.get(identifier, self.metrics_by_abbreviation.get(identifier))

    def connect(self, device: Device) -> EventCallback:
        async def callback(weboscket: Websocket) -> None:
            now = datetime.utcnow()
            metric = await self.get_metric("connected")

            if metric is None:
                LOG.warn(
                    f"'connected' not in api, skipping connected measurement."
                )
                return

            await self.iot_client.measurements.create_measurement(
                Measurement(
                    id=-1,
                    timestamp=now,
                    device_id=device.id,
                    location_id=device.location_id,
                    metric_id=metric.id,
                    tags=["meta"],
                    value_type="boolean",
                    value=True
                )
            )

            await self.iot_client.devices.update_device(
                Device(
                    id=device.id,
                    mac=device.mac,
                    ip=device.ip,
                    websocket_path=device.websocket_path,
                    location_id=device.location_id,
                    last_message=now,
                )
            )
        return callback

    def message(self, device: Device) -> EventWithMessageCallback:
        async def callback(weboscket: Websocket, message: WSMessage) -> None:
            await self.iot_client.devices.update_device(
                Device(
                    id=device.id,
                    mac=device.mac,
                    ip=device.ip,
                    websocket_path=device.websocket_path,
                    location_id=device.location_id,
                    last_message=datetime.utcnow(),
                )
            )
        return callback

    def text(self, device: Device) -> EventWithMessageCallback:
        async def callback(weboscket: Websocket, message: WSMessage) -> None:
            LOG.warning(
                f"Device {device.id} websocket text handler called for below, could be an error?"
            )
            LOG.warning(message.data)
        return callback

    def json(self, device: Device) -> EventWithMessageCallback:
        async def callback(weboscket: Websocket, message: WSMessage) -> None:
            data = message.json(loads=parse_json)
            if data.get("type") == "ping":
                return

            timestamp = parse_date(data.get("timestamp", "1970-01-01"))
            if datetime.utcnow() - timestamp > timedelta(days=1):
                timestamp = datetime.utcnow()

            metric = await self.get_metric(data.get("metric"))
            if metric is None:
                LOG.warn(
                    f"Device {device.id} sent unknown metric '{data.get('metric')}' skipping."
                )
                return

            await self.iot_client.measurements.create_measurement(
                Measurement(
                    id=-1,
                    timestamp=timestamp,
                    device_id=device.id,
                    location_id=data["location_id"],
                    metric_id=metric.id,
                    tags=data["tags"],
                    value_type="boolean",
                    value=data["value"]
                )
            )

        return callback

    def error(self, device: Device) -> EventWithMessageCallback:
        async def callback(weboscket: Websocket, message: WSMessage) -> None:
            LOG.warning(f"Device {device.id} websocket errored.")
            await self.websocket_store.delete_websocket_by_device(device)
        return callback

    def close(self, device: Device) -> EventWithMessageCallback:
        async def callback(weboscket: Websocket, message: WSMessage) -> None:
            LOG.warning(f"Device {device.id} websocket closed.")
            await self.websocket_store.delete_websocket_by_device(device)
        return callback

    def disconnect(self, device: Device) -> EventCallback:
        async def callback(weboscket: Websocket) -> None:
            metric = await self.get_metric("connected")

            if metric is None:
                LOG.warn(
                    f"'connected' not in api, skipping disconnected measurement."
                )
                return

            await self.iot_client.measurements.create_measurement(
                Measurement(
                    id=-1,
                    timestamp=datetime.utcnow(),
                    device_id=device.id,
                    location_id=device.location_id,
                    metric_id=metric.id,
                    tags=["meta"],
                    value_type="boolean",
                    value=False
                )
            )
        return callback
