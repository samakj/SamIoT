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
from shared.python.models.Weather import DailyWeather

DAILY_V0_ROUTES = RouteTableDef()


@DAILY_V0_ROUTES.view("/v0/daily")
class DailysV0View(PydanticView):
    async def get(
        self,
        id: Optional[List[int]] = None,
        timestamp: Optional[List[datetime]] = None,
        owm_weather_id: Optional[List[int]] = None,
        owm_weather_title: Optional[List[str]] = None,
        temperature_gte: Optional[Decimal] = None,
        temperature_lte: Optional[Decimal] = None,
        temperature_min_gte: Optional[Decimal] = None,
        temperature_min_lte: Optional[Decimal] = None,
        temperature_max_gte: Optional[Decimal] = None,
        temperature_max_lte: Optional[Decimal] = None,
        apparent_temperature_gte: Optional[Decimal] = None,
        apparent_temperature_lte: Optional[Decimal] = None,
        timestamp_gte: Optional[datetime] = None,
        timestamp_lte: Optional[datetime] = None,
    ) -> Response:
        """
        Tags: Daily
        """
        app: WeatherAPIApplication = self.request.app

        if not app.daily_store:
            raise ValueError(
                "Daily weather store not initialised before querying data."
            )

        return json_response(
            await try_route_cache(
                self,
                app.daily_store.get_daily_weather,
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
                    "temperature_min_gte": temperature_min_gte,
                    "temperature_min_lte": temperature_min_lte,
                    "temperature_max_gte": temperature_max_gte,
                    "temperature_max_lte": temperature_max_lte,
                    "apparent_temperature_gte": apparent_temperature_gte,
                    "apparent_temperature_lte": apparent_temperature_lte,
                    "timestamp_gte": timestamp_gte,
                    "timestamp_lte": timestamp_lte,
                },
                expiry=15 * 60,
                prefix="weather:api"
            )
        )

    async def post(self, daily: DailyWeather) -> Response:
        """
        Tags: Daily
        """
        app: WeatherAPIApplication = self.request.app

        if not app.daily_store:
            raise ValueError(
                "Daily weather store not initialised before querying data."
            )

        _daily = await app.daily_store.upsert_daily_weather(daily)

        if _daily is not None and app.cache is not None:
            base_url = f"{'/'.join(str(self.request.url).split('/')[:3])}/v0/daily"
            invalid_keys = set()
            invalid_keys.update(await app.cache.keys(f"weather:api:route:{base_url}"))
            invalid_keys.update(await app.cache.keys(f"weather:api:route:{base_url}?*"))
            invalid_keys.update(await app.cache.keys(f"weather:api:route:{base_url}/{_daily.id}"))
            invalid_keys.update(await app.cache.keys(f"weather:api:route:{base_url}/timestamp/{_daily.timestamp.isoformat()}"))
            if invalid_keys:
                await app.cache.delete(*invalid_keys)

        return json_response(daily)


@DAILY_V0_ROUTES.view("/v0/daily/{id:\d+}")
class DailyV0View(PydanticView):
    async def get(self, id: int, /) -> Response:
        """
        Tags: Daily
        """
        app: WeatherAPIApplication = self.request.app

        if not app.daily_store:
            raise ValueError(
                "Daily weather store not initialised before querying data."
            )

        daily = await try_route_cache(
            self,
            app.daily_store.get_daily_weather,
            args=(id,),
            expiry=15 * 60,
            prefix="weather:api"
        )

        if daily is None:
            raise HTTPNotFound()

        return json_response(daily)


@DAILY_V0_ROUTES.view("/v0/daily/timestamp/{timestamp}")
class DailyTimestampV0View(PydanticView):
    async def get(self, timestamp: datetime, /) -> Response:
        """
        Tags: Daily
        """
        app: WeatherAPIApplication = self.request.app

        if not app.daily_store:
            raise ValueError(
                "Daily weather store not initialised before querying data."
            )

        daily = await try_route_cache(
            self,
            app.daily_store.get_daily_weather_by_timestamp,
            args=(timestamp,),
            expiry=15 * 60,
            prefix="weather:api"
        )

        if daily is None:
            raise HTTPNotFound()

        return json_response(daily)
