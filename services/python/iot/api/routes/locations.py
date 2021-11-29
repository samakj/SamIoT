import asyncio
from dataclasses import asdict
from typing import List, Optional
from aiohttp_pydantic import PydanticView
from datetime import datetime
from aiohttp.web import RouteTableDef, Response, HTTPNotFound, WebSocketResponse

from shared.python.extensions.aiohttp.responses.json import json_response

from Application import IoTAPIApplication

LOCATIONS_V0_ROUTES = RouteTableDef()


@LOCATIONS_V0_ROUTES.view("/v0/locations")
class LocationsV0View(PydanticView):
    async def get(
        self,
        ids: Optional[List[int]] = None,
        names: Optional[List[str]] = None,
        tags: Optional[List[str]] = None
    ) -> Response:
        app: IoTAPIApplication = self.request.app

        if not app.locations_store:
            raise ValueError(
                "Location store not initialised before querying data."
            )

        return json_response(
            await app.locations_store.get_locations(ids, names, tags)
        )


@LOCATIONS_V0_ROUTES.view("/v0/locations/{id:\d+}")
class LocationV0View(PydanticView):
    async def get(self, id: int) -> Response:
        app: IoTAPIApplication = self.request.app

        if not app.locations_store:
            raise ValueError(
                "Location store not initialised before querying data."
            )

        location = await app.locations_store.get_location(id)

        if location is None:
            raise HTTPNotFound()

        return json_response(location)


@LOCATIONS_V0_ROUTES.view("/v0/locations/name/{name}")
class LocationNameV0View(PydanticView):
    async def get(self, name: str) -> Response:
        app: IoTAPIApplication = self.request.app

        if not app.locations_store:
            raise ValueError(
                "Location store not initialised before querying data."
            )

        location = await app.locations_store.get_location_by_name(name)

        if location is None:
            raise HTTPNotFound()

        return json_response(location)


@LOCATIONS_V0_ROUTES.view("/v0/locations/ws")
class LocationsWebsocketV0View(PydanticView):
    async def get(self, socket_id: Optional[str] = "") -> WebSocketResponse:
        app: IoTAPIApplication = self.request.app

        if not app.locations_store:
            raise ValueError(
                "Location store not initialised before querying data."
            )

        _socket_id = (
            socket_id
            if socket_id is not None
            else self.request.remote or str(datetime.utcnow().timestamp())
        )

        websocket = WebSocketResponse()
        await websocket.prepare(self.request)

        return await app.locations_store.listen(
            _socket_id, websocket
        )


@LOCATIONS_V0_ROUTES.view("/v0/locations/ws/{id:\d+}")
class LocationWebsocketV0View(PydanticView):
    async def get(self, id: int, socket_id: Optional[str] = "") -> WebSocketResponse:
        app: IoTAPIApplication = self.request.app

        socket_id = (
            self.request.query["socket_id"]
            if self.request.query.get("socket_id") is not None
            else (self.request.remote or str(datetime.utcnow().timestamp())) + f"/{id}"
        )

        websocket = WebSocketResponse()
        await websocket.prepare(self.request)

        return await app.locations_store.listen(
            socket_id, websocket, id
        )
