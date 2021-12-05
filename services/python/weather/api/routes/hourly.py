import asyncio
from dataclasses import asdict
from decimal import Decimal
from typing import List, Optional, Union
from aiohttp_pydantic import PydanticView
from datetime import datetime
from aiohttp.web import RouteTableDef, Response, HTTPNotFound, WebSocketResponse

from shared.python.extensions.aiohttp.responses.json import json_response

from Application import WeatherAPIApplication
from shared.python.extensions.aioredis import try_route_cache
from shared.python.models.Weather import HourlyWeather

HOURLY_V0_ROUTES = RouteTableDef()


@HOURLY_V0_ROUTES.view("/v0/hourly")
class HourlysV0View(PydanticView):
    async def get(
        self,
        id: Optional[List[int]] = None,
        timestamp: Optional[List[datetime]] = None,
        owm_weather_id: Optional[List[int]] = None,
        owm_weather_title: Optional[List[str]] = None,
        temperature_gte: Optional[Decimal] = None,
        temperature_lte: Optional[Decimal] = None,
        apparent_temperature_gte: Optional[Decimal] = None,
        apparent_temperature_lte: Optional[Decimal] = None,
        timestamp_gte: Optional[datetime] = None,
        timestamp_lte: Optional[datetime] = None,
    ) -> Response:
        """
        Tags: Hourly
        """
        app: WeatherAPIApplication = self.request.app

        if not app.hourly_store:
            raise ValueError(
                "Hourly weather store not initialised before querying data."
            )

        return json_response(
            await try_route_cache(
                self,
                app.hourly_store.get_hourly_weather,
                kwargs={
                    "ids": (
                        id if isinstance(id, list) else
                        [id] if id is not None else
                        None
                    ),
                    "timestamps": (
                        timestamp if isinstance(timestamp, list) else
                        [timestamp] if timestamp is not None else
                        None
                    ),
                    "owm_weather_ids": (
                        owm_weather_id if isinstance(owm_weather_id, list) else
                        [owm_weather_id] if owm_weather_id is not None else
                        None
                    ),
                    "owm_weather_titles": (
                        owm_weather_title if isinstance(owm_weather_title, list) else
                        [owm_weather_title] if owm_weather_title is not None else
                        None
                    ),
                    "temperature_gte": temperature_gte,
                    "temperature_lte": temperature_lte,
                    "apparent_temperature_gte": apparent_temperature_gte,
                    "apparent_temperature_lte": apparent_temperature_lte,
                    "timestamp_gte": timestamp_gte,
                    "timestamp_lte": timestamp_lte,
                },
                expiry=15 * 60,
                prefix="weather:api"
            )
        )

    async def post(self, hourly: HourlyWeather) -> Response:
        """
        Tags: Hourly
        """
        app: WeatherAPIApplication = self.request.app

        if not app.hourly_store:
            raise ValueError(
                "Hourly weather store not initialised before querying data."
            )

        _hourly = await app.hourly_store.upsert_hourly_weather(hourly)

        if _hourly is not None and app.cache is not None:
            base_url = f"{'/'.join(str(self.request.url).split('/')[:3])}/v0/hourly"
            invalid_keys = set()
            invalid_keys.update(await app.cache.keys(f"weather:api:route:{base_url}"))
            invalid_keys.update(await app.cache.keys(f"weather:api:route:{base_url}?*"))
            invalid_keys.update(await app.cache.keys(f"weather:api:route:{base_url}/{_hourly.id}"))
            invalid_keys.update(await app.cache.keys(f"weather:api:route:{base_url}/timestamp/*"))
            if invalid_keys:
                await app.cache.delete(*invalid_keys)

        return json_response(hourly)


@HOURLY_V0_ROUTES.view("/v0/hourly/{id:\d+}")
class HourlyV0View(PydanticView):
    async def get(self, id: int, /) -> Response:
        """
        Tags: Hourly
        """
        app: WeatherAPIApplication = self.request.app

        if not app.hourly_store:
            raise ValueError(
                "Hourly weather store not initialised before querying data."
            )

        hourly = await try_route_cache(
            self,
            app.hourly_store.get_hourly_weather,
            args=(id,),
            expiry=15 * 60,
            prefix="weather:api"
        )

        if hourly is None:
            raise HTTPNotFound()

        return json_response(hourly)


@HOURLY_V0_ROUTES.view("/v0/hourly/timestamp/{timestamp}")
class HourlyTimestampV0View(PydanticView):
    async def get(self, timestamp: datetime, /) -> Response:
        """
        Tags: Hourly
        """
        app: WeatherAPIApplication = self.request.app

        if not app.hourly_store:
            raise ValueError(
                "Hourly weather store not initialised before querying data."
            )

        hourly = await try_route_cache(
            self,
            app.hourly_store.get_hourly_weather_by_nearest_timestamp,
            args=(timestamp,),
            expiry=15 * 60,
            prefix="weather:api"
        )

        if hourly is None:
            raise HTTPNotFound()

        return json_response(hourly)
