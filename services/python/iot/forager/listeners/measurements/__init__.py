import logging
from typing import Dict, List

from shared.python.models.Device import Device
from shared.python.models.Location import Location
from shared.python.clients.iot import IoTClient
from stores.device_websockets import DeviceWebsocketsStore
from listeners.measurements.EventHandlers import EventHandlers

LOG = logging.getLogger()


class MeasurementListener:
    websocket_store: DeviceWebsocketsStore
    iot_client: IoTClient
    devices: Dict[int, Device]
    locations: Dict[int, Location]

    watch_list: List[int]
    event_handlers: EventHandlers

    def __init__(
        self,
        websocket_store: DeviceWebsocketsStore,
        iot_client: IoTClient
    ) -> None:
        self.websocket_store = websocket_store
        self.iot_client = iot_client
        self.devices = {}
        self.locations = {}

        self.watch_list = []
        self.event_handlers = EventHandlers(websocket_store, iot_client)

    async def update_devices(self) -> None:
        self.devices = {
            device.id: device
            for device in
            await self.iot_client.devices.get_devices()
        }

    async def update_locations(self) -> None:
        self.locations = {
            location.id: location
            for location in
            await self.iot_client.locations.get_locations()
        }

    async def set_watch_list(self, device_ids: List[int]) -> None:
        self.watch_list = device_ids

    async def enact_watch_list(self) -> None:
        for device_id in self.watch_list:
            if device_id not in self.devices.keys():
                await self.update_devices()
            if device_id not in self.devices.keys():
                LOG.warn(
                    f"Device {device_id} doesn't exist, ignoring in watch list."
                )
                continue
            if self.websocket_store.get_websocket(device_id) is None:
                device = self.devices[device_id]
                await self.websocket_store.create(
                    device,
                    on_connect=self.event_handlers.connect(device),
                    on_message=self.event_handlers.message(device),
                    on_text=self.event_handlers.text(device),
                    on_json=self.event_handlers.json(device),
                    on_error=self.event_handlers.error(device),
                    on_close=self.event_handlers.close(device),
                    on_disconnect=self.event_handlers.disconnect(device),
                )

    async def watch_all(self) -> None:
        await self.update_devices()
        await self.set_watch_list(
            [int(device_id) for device_id in self.devices.keys()]
        )
