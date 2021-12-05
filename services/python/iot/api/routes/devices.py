import asyncio
from dataclasses import asdict
from typing import List, Optional, Union
from aiohttp_pydantic import PydanticView
from datetime import datetime
from aiohttp.web import RouteTableDef, Response, HTTPNotFound, WebSocketResponse

from shared.python.extensions.aiohttp.responses.json import json_response

from Application import IoTAPIApplication
from shared.python.extensions.aioredis import try_route_cache
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
        """
        Tags: Devices
        """
        app: IoTAPIApplication = self.request.app

        if not app.devices_store:
            raise ValueError(
                "Device store not initialised before querying data."
            )

        return json_response(
            await try_route_cache(
                self,
                app.devices_store.get_devices,
                kwargs={
                    "ids": (
                        id if isinstance(id, list) else
                        [id] if id is not None else
                        None
                    ),
                    "macs": (
                        mac if isinstance(mac, list) else
                        [mac] if mac is not None else
                        None
                    ),
                    "ips": (
                        ip if isinstance(ip, list) else
                        [ip] if ip is not None else
                        None
                    ),
                    "location_ids": (
                        location_id if isinstance(location_id, list) else
                        [location_id] if location_id is not None else
                        None
                    ),
                    "last_message_gte": last_message_gte,
                    "last_message_lte": last_message_lte
                },
                expiry=15 * 60,
                prefix="iot:api"
            )
        )


@DEVICES_V0_ROUTES.view("/v0/devices/{id:\d+}")
class DeviceV0View(PydanticView):
    async def get(self, id: int, /) -> Response:
        """
        Tags: Devices
        """
        app: IoTAPIApplication = self.request.app

        if not app.devices_store:
            raise ValueError(
                "Device store not initialised before querying data."
            )

        device = await try_route_cache(
            self,
            app.devices_store.get_device,
            args=(id,),
            expiry=15 * 60,
            prefix="iot:api"
        )

        if device is None:
            raise HTTPNotFound()

        return json_response(device)

    async def patch(self, device: Device) -> Response:
        """
        Tags: Devices
        """
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

        if app.cache is not None:
            base_url = f"{'/'.join(str(self.request.url).split('/')[:3])}/v0/locations"
            invalid_keys = set()
            invalid_keys.update(await app.cache.keys(f"iot:api:route:{base_url}"))
            invalid_keys.update(await app.cache.keys(f"iot:api:route:{base_url}?*"))
            invalid_keys.update(await app.cache.keys(f"iot:api:route:{base_url}/{_device.id}"))
            invalid_keys.update(await app.cache.keys(f"iot:api:route:{base_url}/mac/{_device.mac}"))
            invalid_keys.update(await app.cache.keys(f"iot:api:route:{base_url}/ip/{_device.ip}"))

            if invalid_keys:
                await app.cache.delete(*invalid_keys)

        return json_response(_device)


@DEVICES_V0_ROUTES.view("/v0/devices/mac/{mac}")
class DeviceMacV0View(PydanticView):
    async def get(self, mac: str, /) -> Response:
        """
        Tags: Devices
        """
        app: IoTAPIApplication = self.request.app

        if not app.devices_store:
            raise ValueError(
                "Device store not initialised before querying data."
            )

        device = await try_route_cache(
            self,
            app.devices_store.get_device_by_mac,
            args=(mac,),
            expiry=15 * 60,
            prefix="iot:api"
        )

        if device is None:
            raise HTTPNotFound()

        return json_response(device)


@DEVICES_V0_ROUTES.view("/v0/devices/ip/{ip}")
class DeviceIPV0View(PydanticView):
    async def get(self, ip: str, /) -> Response:
        """
        Tags: Devices
        """
        app: IoTAPIApplication = self.request.app

        if not app.devices_store:
            raise ValueError(
                "Device store not initialised before querying data."
            )

        device = await try_route_cache(
            self,
            app.devices_store.get_device_by_ip,
            args=(ip,),
            expiry=15 * 60,
            prefix="iot:api"
        )

        if device is None:
            raise HTTPNotFound()

        return json_response(device)


@DEVICES_V0_ROUTES.view("/v0/devices/ws")
class DevicesWebsocketV0View(PydanticView):
    async def get(self, socket_id: Optional[str] = "") -> WebSocketResponse:
        """
        Tags: Devices
        """
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
    async def get(self, id: int, /, socket_id: Optional[str] = "") -> WebSocketResponse:
        """
        Tags: Devices
        """
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
