import asyncio
from dataclasses import asdict
from typing import List, Optional, Union
from aiohttp_pydantic import PydanticView
from datetime import datetime
from aiohttp.web import RouteTableDef, Response, HTTPNotFound, WebSocketResponse

from shared.python.extensions.aiohttp.responses.json import json_response

from Application import ForagerApplication
from shared.python.extensions.aioredis import try_route_cache

LISTENERS_V0_ROUTES = RouteTableDef()


@LISTENERS_V0_ROUTES.view("/v0/watch-list")
class DevicesV0View(PydanticView):
    async def get(self) -> Response:
        """
        Tags: Listener
        """
        app: ForagerApplication = self.request.app

        return json_response([
            app.measurements_listener.devices[device_id]
            for device_id in app.measurements_listener.watch_list
        ])
