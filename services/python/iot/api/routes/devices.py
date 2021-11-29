import asyncio
from dataclasses import asdict
from typing import List, Optional, Union
from aiohttp_pydantic import PydanticView
from datetime import datetime
from aiohttp.web import RouteTableDef, Response, HTTPNotFound, WebSocketResponse

from shared.python.extensions.aiohttp.responses.json import json_response

from Application import IoTAPIApplication
from shared.python.models.Device import Device

DEVICES_V0_ROUTES = RouteTableDef()


@DEVICES_V0_ROUTES.view("/v0/devices")
class DevicesV0View(PydanticView):
    async def get(
        self,
        id: Optional[Union[int, List[int]]] = None,
        mac: Optional[Union[str, List[str]]] = None,
        ip: Optional[Union[str, List[str]]] = None,
        location_id: Optional[Union[int, List[int]]] = None,
        last_message_gte: Optional[datetime] = None,
        last_message_lte: Optional[datetime] = None
    ) -> Response:
        app: IoTAPIApplication = self.request.app

        if not app.devices_store:
            raise ValueError(
                "Device store not initialised before querying data."
            )

        return json_response(
            await app.devices_store.get_devices(
                id
                if isinstance(id, list) else
                [id]
                if id is not None else
                None,
                mac
                if isinstance(mac, list) else
                [mac]
                if mac is not None else
                None,
                ip
                if isinstance(ip, list) else
                [ip]
                if ip is not None else
                None,
                location_id
                if isinstance(location_id, list) else
                [location_id]
                if location_id is not None else
                None,
                last_message_gte,
                last_message_lte
            )
        )


@DEVICES_V0_ROUTES.view("/v0/devices/{id:\d+}")
class DeviceV0View(PydanticView):
    async def get(self, id: int) -> Response:
        app: IoTAPIApplication = self.request.app

        if not app.devices_store:
            raise ValueError(
                "Device store not initialised before querying data."
            )

        device = await app.devices_store.get_device(id)

        if device is None:
            raise HTTPNotFound()

        return json_response(device)

    async def patch(self, device: Device) -> Response:
        app: IoTAPIApplication = self.request.app

        if not app.devices_store:
            raise ValueError(
                "Device store not initialised before querying data."
            )

        _device = await app.devices_store.update_device(
            id=device.id,
            last_message=device.last_message
        )

        if _device is None:
            raise HTTPNotFound()

        return json_response(_device)


@DEVICES_V0_ROUTES.view("/v0/devices/mac/{mac}")
class DeviceMacV0View(PydanticView):
    async def get(self, mac: str) -> Response:
        app: IoTAPIApplication = self.request.app

        if not app.devices_store:
            raise ValueError(
                "Device store not initialised before querying data."
            )

        device = await app.devices_store.get_device_by_mac(mac)

        if device is None:
            raise HTTPNotFound()

        return json_response(device)


@DEVICES_V0_ROUTES.view("/v0/devices/ip/{ip}")
class DeviceIPV0View(PydanticView):
    async def get(self, ip: str) -> Response:
        app: IoTAPIApplication = self.request.app

        if not app.devices_store:
            raise ValueError(
                "Device store not initialised before querying data."
            )

        device = await app.devices_store.get_device_by_ip(ip)

        if device is None:
            raise HTTPNotFound()

        return json_response(device)


@DEVICES_V0_ROUTES.view("/v0/devices/ws")
class DevicesWebsocketV0View(PydanticView):
    async def get(self, socket_id: Optional[str] = "") -> WebSocketResponse:
        app: IoTAPIApplication = self.request.app

        if not app.devices_store:
            raise ValueError(
                "Device store not initialised before querying data."
            )

        _socket_id = (
            socket_id
            if socket_id is not None
            else self.request.remote or str(datetime.utcnow().timestamp())
        )

        websocket = WebSocketResponse()
        await websocket.prepare(self.request)

        return await app.devices_store.listen(
            _socket_id, websocket
        )


@DEVICES_V0_ROUTES.view("/v0/devices/ws/{id:\d+}")
class DeviceWebsocketV0View(PydanticView):
    async def get(self, id: int, socket_id: Optional[str] = "") -> WebSocketResponse:
        app: IoTAPIApplication = self.request.app

        socket_id = (
            self.request.query["socket_id"]
            if self.request.query.get("socket_id") is not None
            else (self.request.remote or str(datetime.utcnow().timestamp())) + f"/{id}"
        )

        websocket = WebSocketResponse()
        await websocket.prepare(self.request)

        return await app.devices_store.listen(
            socket_id, websocket, id
        )
