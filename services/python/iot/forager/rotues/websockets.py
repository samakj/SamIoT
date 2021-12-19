import asyncio
from dataclasses import asdict
from typing import List, Optional, Union
from aiohttp_pydantic import PydanticView
from datetime import datetime
from aiohttp.web import RouteTableDef, Response, HTTPNotFound, WebSocketResponse

from shared.python.extensions.aiohttp.responses.json import json_response

from Application import ForagerApplication
from shared.python.extensions.aioredis import CachedJSONResponse

WEBSOCKETS_V0_ROUTES = RouteTableDef()


@WEBSOCKETS_V0_ROUTES.view("/v0/websockets")
class DevicesV0View(PydanticView):
    async def get(self) -> Response:
        """
        Tags: Websockets
        """
        app: ForagerApplication = self.request.app

        async def get_devices():
            return {
                device_id: await app.device_websocket_store.websockets[device_id].serialise()
                for device_id in app.device_websocket_store.websockets
            }

        cache = CachedJSONResponse(
            self,
            get_devices,
            expiry=10,
            prefix="iot:forager"
        )

        return await cache.get_response()
