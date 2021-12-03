import logging
from typing import Dict, Optional
from datetime import datetime

from aiohttp.http_websocket import WSMessage

from shared.python.extensions.aiohttp.websocket import EventCallback, EventWithMessageCallback, Websocket
from shared.python.clients.iot import IoTClient
from shared.python.models.Device import Device

LOG = logging.getLogger()


class DeviceWebsocketsStore:
    websockets: Dict[int, Websocket]

    def __init__(self) -> None:
        self.websockets = {}

    async def create(
        self,
        device: Device,
        *,
        on_connect: Optional[EventCallback] = None,
        on_disconnect: Optional[EventCallback] = None,
        on_message: Optional[EventWithMessageCallback] = None,
        on_text: Optional[EventWithMessageCallback] = None,
        on_json: Optional[EventWithMessageCallback] = None,
        on_error: Optional[EventWithMessageCallback] = None,
        on_close: Optional[EventWithMessageCallback] = None,
    ) -> Optional[Websocket]:
        if device.id in self.websockets:
            LOG.warning(
                f"Creating device {device.id} websocket already which exists,"
                " cancelling old task..."
            )
            await self.delete_websocket(device.id)

        self.websockets[device.id] = Websocket(
            f"ws://{device.ip}{device.websocket_path}",
            id=f"forager:device:{device.id}",
            name=f"Device {device.id}",
            on_connect=on_connect,
            on_disconnect=on_disconnect,
            on_message=on_message,
            on_text=on_text,
            on_json=on_json,
            on_error=on_error,
            on_close=on_close,
        )
        await self.websockets[device.id].start()

        return self.websockets[device.id]

    async def get_websocket(self, id: int) -> Optional[Websocket]:
        return self.websockets.get(id, None)

    async def get_websocket_by_device(
        self, device: Device
    ) -> Optional[Websocket]:
        return await self.get_websocket(device.id)

    async def get_websockets(self) -> Dict[int, Websocket]:
        return self.websockets

    async def delete_websocket(self, id: int) -> None:
        LOG.info(f"Shutting down device {id} websocket...")
        await self.websockets[id].stop()
        del self.websockets[id]
        LOG.info(f"Done.")

    async def delete_websocket_by_device(self, device: Device) -> None:
        return await self.delete_websocket(device.id)

    async def delete_websockets(self) -> None:
        LOG.info(
            f"Shutting down {len(self.websockets.keys())} device"
            " websockets..."
        )
        for id in [id for id in self.websockets.keys()]:
            await self.delete_websocket(id)

        LOG.info(f"Done.")
