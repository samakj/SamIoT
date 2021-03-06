import asyncio
from dataclasses import asdict
from typing import List, Optional, Union
from aiohttp_pydantic import PydanticView
from datetime import datetime
from aiohttp.web import RouteTableDef, Response, HTTPNotFound, WebSocketResponse

from shared.python.extensions.aiohttp.responses.json import json_response

from Application import IoTAPIApplication
from shared.python.extensions.aioredis import CachedJSONResponse

METRICS_V0_ROUTES = RouteTableDef()


@METRICS_V0_ROUTES.view("/v0/metrics")
class MetricsV0View(PydanticView):
    async def get(
        self,
        id: Optional[Union[int, List[int]]] = None,
        name: Optional[Union[str, List[str]]] = None,
        abbreviation: Optional[Union[str, List[str]]] = None
    ) -> Response:
        """
        Tags: Metrics
        """
        app: IoTAPIApplication = self.request.app

        if not app.metrics_store:
            raise ValueError(
                "Metric store not initialised before querying data."
            )

        cache = CachedJSONResponse(
            self,
            app.metrics_store.get_metrics,
            kwargs={
                "ids": (
                    id if isinstance(id, list) else
                    [id] if id is not None else
                    None
                ),
                "names": (
                    name if isinstance(name, list) else
                    [name] if name is not None else
                    None
                ),
                "abbreviations": (
                    abbreviation if isinstance(abbreviation, list) else
                    [abbreviation] if abbreviation is not None else
                    None
                )
            },
            expiry=15 * 60,
            prefix="iot:api"
        )

        return await cache.get_response()


@METRICS_V0_ROUTES.view("/v0/metrics/{id:\d+}")
class MetricV0View(PydanticView):
    async def get(self, id: int, /) -> Response:
        """
        Tags: Metrics
        """
        app: IoTAPIApplication = self.request.app

        if not app.metrics_store:
            raise ValueError(
                "Metric store not initialised before querying data."
            )

        cache = CachedJSONResponse(
            self,
            app.metrics_store.get_metric,
            args=(id,),
            expiry=15 * 60,
            prefix="iot:api"
        )

        return await cache.get_response()


@ METRICS_V0_ROUTES.view("/v0/metrics/name/{name}")
class MetricNameV0View(PydanticView):
    async def get(self, name: str, /) -> Response:
        """
        Tags: Metrics
        """
        app: IoTAPIApplication = self.request.app

        if not app.metrics_store:
            raise ValueError(
                "Metric store not initialised before querying data."
            )

        cache = CachedJSONResponse(
            self,
            app.metrics_store.get_metric_by_name,
            args=(name,),
            expiry=15 * 60,
            prefix="iot:api"
        )

        return await cache.get_response()


@ METRICS_V0_ROUTES.view("/v0/metrics/abbreviation/{abbreviation}")
class MetricAbbreviationV0View(PydanticView):
    async def get(self, abbreviation: str, /) -> Response:
        """
        Tags: Metrics
        """
        app: IoTAPIApplication = self.request.app

        if not app.metrics_store:
            raise ValueError(
                "Metric store not initialised before querying data."
            )

        cache = CachedJSONResponse(
            self,
            app.metrics_store.get_metric_by_abbreviation,
            args=(abbreviation,),
            expiry=15 * 60,
            prefix="iot:api"
        )

        return await cache.get_response()


@ METRICS_V0_ROUTES.view("/v0/metrics/ws")
class MetricsWebsocketV0View(PydanticView):
    async def get(self, socket_id: Optional[str] = "") -> WebSocketResponse:
        """
        Tags: Metrics
        """
        app: IoTAPIApplication = self.request.app

        if not app.metrics_store:
            raise ValueError(
                "Metric store not initialised before querying data."
            )

        _socket_id = (
            socket_id
            if socket_id is not None
            else self.request.remote or str(datetime.utcnow().timestamp())
        )

        websocket = WebSocketResponse()
        await websocket.prepare(self.request)

        return await app.metrics_store.listen(
            _socket_id, websocket
        )


@ METRICS_V0_ROUTES.view("/v0/metrics/ws/{id:\d+}")
class MetricWebsocketV0View(PydanticView):
    async def get(self, id: int, /, socket_id: Optional[str] = "") -> WebSocketResponse:
        """
        Tags: Metrics
        """
        app: IoTAPIApplication = self.request.app

        socket_id = (
            self.request.query["socket_id"]
            if self.request.query.get("socket_id") is not None
            else (self.request.remote or str(datetime.utcnow().timestamp())) + f"/{id}"
        )

        websocket = WebSocketResponse()
        await websocket.prepare(self.request)

        return await app.metrics_store.listen(
            socket_id, websocket, id
        )
