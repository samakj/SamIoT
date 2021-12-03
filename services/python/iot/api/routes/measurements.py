import re
import asyncio
from dataclasses import asdict
from decimal import Decimal
from typing import List, Optional, Union
from aiohttp_pydantic import PydanticView
from datetime import datetime, timedelta
from aiohttp.web import RouteTableDef, Response, HTTPNotFound, WebSocketResponse

from shared.python.extensions.aiohttp.responses.json import json_response

from Application import IoTAPIApplication
from shared.python.extensions.aioredis import try_route_cache
from shared.python.models.Measurement import Measurement, ValueType

MEASUREMENTS_V0_ROUTES = RouteTableDef()


def parse_value(value: Optional[str] = None) -> ValueType:
    if value is None:
        return None

    if re.fullmatch('-?[\d]+', value):
        return int(value)
    if re.fullmatch('-?[\d]+\.[\d]+', value):
        return Decimal(value)
    if value.lower() in ('true', 'yes'):
        return True
    if value.lower() in ('false', 'no'):
        return False

    return value


@MEASUREMENTS_V0_ROUTES.view("/v0/measurements")
class MeasurementsV0View(PydanticView):
    async def get(
        self,
        id: Optional[Union[int, List[int]]] = None,
        device_id: Optional[Union[int, List[int]]] = None,
        location_id: Optional[Union[int, List[int]]] = None,
        metric_id: Optional[Union[int, List[int]]] = None,
        tags: Optional[Union[str, List[str]]] = None,
        timestamp_gte: Optional[datetime] = None,
        timestamp_lte: Optional[datetime] = None,
        value: Optional[str] = None,
        value_gte: Optional[str] = None,
        value_lte: Optional[str] = None,
    ) -> Response:
        """
        Tags: Measurements
        """
        app: IoTAPIApplication = self.request.app

        if not app.measurements_store:
            raise ValueError(
                "Measurement store not initialised before querying data."
            )

        return json_response(
            await try_route_cache(
                self,
                app.measurements_store.get_measurements,
                kwargs={
                    "ids": (
                        id if isinstance(id, list) else
                        [id] if id is not None else
                        None
                    ),
                    "device_ids": (
                        device_id if isinstance(device_id, list) else
                        [device_id] if device_id is not None else
                        None
                    ),
                    "location_ids": (
                        location_id if isinstance(location_id, list) else
                        [location_id] if location_id is not None else
                        None
                    ),
                    "metric_ids": (
                        metric_id if isinstance(metric_id, list) else
                        [metric_id] if metric_id is not None else
                        None
                    ),
                    "tags": (
                        tags if isinstance(tags, list) else
                        [tags] if tags is not None else
                        None
                    ),
                    "timestamp_gte": timestamp_gte,
                    "timestamp_lte": timestamp_lte,
                    "value": parse_value(value),
                    "value_gte": parse_value(value_gte),
                    "value_lte": parse_value(value_lte),
                },
                expiry=15 * 60,
            )
        )

    async def post(self, measurement: Measurement):
        """
        Tags: Measurements
        """
        app: IoTAPIApplication = self.request.app

        if not app.measurements_store:
            raise ValueError(
                "Measurement store not initialised before querying data."
            )

        _measurement = await app.measurements_store.create_measurement(
            timestamp=measurement.timestamp,
            device_id=measurement.device_id,
            location_id=measurement.location_id,
            metric_id=measurement.metric_id,
            tags=measurement.tags,
            value=measurement.value,
        )

        if _measurement is not None and app.cache is not None:
            base_url = f"{'/'.join(str(self.request.url).split('/')[:3])}/v0/measurements"
            invalid_keys = set()
            invalid_keys.update(await app.cache.keys(f"route:{base_url}"))
            invalid_keys.update(await app.cache.keys(f"route:{base_url}?*"))
            invalid_keys.update(await app.cache.keys(f"route:{base_url}/{_measurement.id}"))
            if invalid_keys:
                await app.cache.delete(*invalid_keys)

        return json_response(_measurement)


@ MEASUREMENTS_V0_ROUTES.view("/v0/measurements/{id:\d+}")
class MeasurementV0View(PydanticView):
    async def get(self, id: int, /) -> Response:
        """
        Tags: Measurements
        """
        app: IoTAPIApplication = self.request.app

        if not app.measurements_store:
            raise ValueError(
                "Measurement store not initialised before querying data."
            )

        measurement = await try_route_cache(
            self,
            app.measurements_store.get_measurement,
            args=(id,),
            expiry=15 * 60
        )

        if measurement is None:
            raise HTTPNotFound()

        return json_response(measurement)


@ MEASUREMENTS_V0_ROUTES.view("/v0/measurements/ws")
class MeasurementsWebsocketV0View(PydanticView):
    async def get(self, socket_id: Optional[str] = "") -> WebSocketResponse:
        """
        Tags: Measurements
        """
        app: IoTAPIApplication = self.request.app

        if not app.measurements_store:
            raise ValueError(
                "Measurement store not initialised before querying data."
            )

        _socket_id = (
            socket_id
            if socket_id is not None
            else self.request.remote or str(datetime.utcnow().timestamp())
        )

        websocket = WebSocketResponse()
        await websocket.prepare(self.request)

        return await app.measurements_store.listen(
            _socket_id, websocket
        )


@ MEASUREMENTS_V0_ROUTES.view("/v0/measurements/ws/{id:\d+}")
class MeasurementWebsocketV0View(PydanticView):
    async def get(self, id: int, /, socket_id: Optional[str] = "") -> WebSocketResponse:
        """
        Tags: Measurements
        """
        app: IoTAPIApplication = self.request.app

        socket_id = (
            self.request.query["socket_id"]
            if self.request.query.get("socket_id") is not None
            else (self.request.remote or str(datetime.utcnow().timestamp())) + f"/{id}"
        )

        websocket = WebSocketResponse()
        await websocket.prepare(self.request)

        return await app.measurements_store.listen(
            socket_id, websocket, id
        )


@ MEASUREMENTS_V0_ROUTES.view("/v0/measurements/latest")
class MeasurementsLatestV0View(PydanticView):
    async def get(
        self,
        device_id: Optional[Union[int, List[int]]] = None,
        location_id: Optional[Union[int, List[int]]] = None,
        metric_id: Optional[Union[int, List[int]]] = None,
        tags: Optional[Union[str, List[str]]] = None,
    ) -> Response:
        """
        Tags: Measurements
        """
        app: IoTAPIApplication = self.request.app

        if not app.measurements_store:
            raise ValueError(
                "Measurement store not initialised before querying data."
            )

        return json_response(
            await try_route_cache(
                self,
                app.measurements_store.get_latest_measurements,
                kwargs={
                    "device_id": (
                        device_id if isinstance(device_id, list) else
                        [device_id] if device_id is not None else
                        None
                    ),
                    "location_id": (
                        location_id if isinstance(location_id, list) else
                        [location_id] if location_id is not None else
                        None
                    ),
                    "metric_id": (
                        metric_id if isinstance(metric_id, list) else
                        [metric_id] if metric_id is not None else
                        None
                    ),
                    "tags": (
                        tags if isinstance(tags, list) else
                        [tags] if tags is not None else
                        None
                    ),
                },
                expiry=15 * 60
            )
        )


@ MEASUREMENTS_V0_ROUTES.view("/v0/measurements/average/{location_id:\d+}/{metric_id:\d+}/{tags}")
class MeasurementsAverageV0View(PydanticView):
    async def get(
        self,
        location_id: int,
        metric_id: int,
        tags: str,
        /,
        start: Optional[datetime] = None,
        end: Optional[datetime] = None,
        period: Optional[int] = None
    ) -> Response:
        """
        Period is in seconds.

        Tags: Measurements
        """
        app: IoTAPIApplication = self.request.app

        if not app.measurements_store:
            raise ValueError(
                "Measurement store not initialised before querying data."
            )

        _end = end or datetime.utcnow().replace(minute=0, second=0, microsecond=0)
        _start = start or (_end - timedelta(days=1))
        _period = timedelta(seconds=period or 3600)

        return json_response(
            await try_route_cache(
                self,
                app.measurements_store.get_time_weighted_average_range,
                kwargs={
                    "location_id": location_id,
                    "metric_id": metric_id,
                    "tags": tags.split(','),
                    "start": _start,
                    "end": _end,
                    "period": _period
                },
                expiry=15 if _end > datetime.utcnow() else 15 * 60
            )
        )
