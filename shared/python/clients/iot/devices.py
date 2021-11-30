from aiohttp import ClientResponseError
from datetime import datetime
from typing import List, Optional, Union

from shared.python.extensions.aiohttp.api_client import APIClient
from shared.python.extensions.aiohttp.websocket import EventCallback, EventWithMessageCallback, Websocket
from shared.python.models.Device import Device
from shared.python.json import serialise_json


class DevicesClient(APIClient):
    async def get_device(self, id: int) -> Optional[Device]:
        try:
            response = await self.get(f"/v0/devices/{id}")
            return Device.parse_obj(await response.json())
        except ClientResponseError as error:
            if error.status == 404:
                return None
            raise

    async def get_device_by_ip(self, ip: str) -> Optional[Device]:
        try:
            response = await self.get(f"/v0/devices/ip/{ip}")
            return Device.parse_obj(await response.json())
        except ClientResponseError as error:
            if error.status == 404:
                return None
            raise

    async def get_device_by_mac(self, mac: str) -> Optional[Device]:
        try:
            response = await self.get(f"/v0/devices/mac/{mac}")
            return Device.parse_obj(await response.json())
        except ClientResponseError as error:
            if error.status == 404:
                return None
            raise

    async def get_devices(
        self,
        id: Optional[Union[int, List[int]]] = None,
        mac: Optional[Union[str, List[str]]] = None,
        ip: Optional[Union[str, List[str]]] = None,
        location_id: Optional[Union[int, List[int]]] = None,
        last_message_gte: Optional[datetime] = None,
        last_message_lte: Optional[datetime] = None
    ) -> List[Device]:
        response = await self.get(
            "/v0/devices",
            params={
                "id": id,
                "mac": mac,
                "ip": ip,
                "location_id": location_id,
                "last_message_gte": last_message_gte,
                "last_message_lte": last_message_lte,
            }
        )

        return [
            Device.parse_obj(item)
            for item in await response.json()
        ]

    async def update_device(self, device: Device) -> Optional[Device]:
        try:
            response = await self.patch(
                f"/v0/devices/{device.id}", data=serialise_json(device)
            )
            return Device.parse_obj(await response.json())
        except ClientResponseError as error:
            if error.status == 404:
                return None
            raise

    async def listen_device(
        self,
        id: int,
        socket_id: str = "",
        name: str = "",
        auto_reconnect_time: int = 15,
        *,
        on_connect: Optional[EventCallback] = None,
        on_disconnect: Optional[EventCallback] = None,
        on_message: Optional[EventWithMessageCallback] = None,
        on_text: Optional[EventWithMessageCallback] = None,
        on_json: Optional[EventWithMessageCallback] = None,
        on_error: Optional[EventWithMessageCallback] = None,
        on_close: Optional[EventWithMessageCallback] = None
    ) -> Websocket:
        return await self.listen(
            f"/v0/devices/ws/{id}",
            socket_id,
            name,
            auto_reconnect_time,
            on_connect=on_connect,
            on_disconnect=on_disconnect,
            on_message=on_message,
            on_text=on_text,
            on_json=on_json,
            on_error=on_error,
            on_close=on_close,
        )

    async def listen_devices(
        self,
        socket_id: str = "",
        name: str = "",
        auto_reconnect_time: int = 15,
        *,
        on_connect: Optional[EventCallback] = None,
        on_disconnect: Optional[EventCallback] = None,
        on_message: Optional[EventWithMessageCallback] = None,
        on_text: Optional[EventWithMessageCallback] = None,
        on_json: Optional[EventWithMessageCallback] = None,
        on_error: Optional[EventWithMessageCallback] = None,
        on_close: Optional[EventWithMessageCallback] = None
    ) -> Websocket:
        return await self.listen(
            f"/v0/devices/ws",
            socket_id,
            name,
            auto_reconnect_time,
            on_connect=on_connect,
            on_disconnect=on_disconnect,
            on_message=on_message,
            on_text=on_text,
            on_json=on_json,
            on_error=on_error,
            on_close=on_close,
        )
