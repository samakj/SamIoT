from decimal import Decimal
from aiohttp import ClientResponseError
from datetime import datetime
from typing import List, Optional, Union

from shared.python.extensions.aiohttp.api_client import APIClient
from shared.python.extensions.aiohttp.websocket import EventCallback, EventWithMessageCallback, Websocket
from shared.python.json import serialise_json
from shared.python.models.Weather import DailyWeather


class DailyClient(APIClient):
    async def get_daily_weather(self, id: int) -> Optional[DailyWeather]:
        try:
            response = await self.get(f"/v0/daily/{id}")
            return DailyWeather.parse_obj(await response.json())
        except ClientResponseError as error:
            if error.status == 404:
                return None
            raise

    async def get_daily_weather_by_timestamp(
        self, timestamp: datetime
    ) -> Optional[DailyWeather]:
        try:
            response = await self.get(f"/v0/daily/timestamp/{timestamp}")
            return DailyWeather.parse_obj(await response.json())
        except ClientResponseError as error:
            if error.status == 404:
                return None
            raise

    async def get_daily_weathers(
        self,
        id: Optional[Union[int, List[int]]] = None,
        timestamp: Optional[Union[datetime, List[datetime]]] = None,
        owm_weather_id: Optional[Union[int, List[int]]] = None,
        owm_weather_title: Optional[Union[str, List[str]]] = None,
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
    ) -> List[DailyWeather]:
        response = await self.get(
            "/v0/daily",
            params={
                "id": id,
                "timestamp": timestamp,
                "owm_weather_id": owm_weather_id,
                "owm_weather_title": owm_weather_title,
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
            }
        )

        return [
            DailyWeather.parse_obj(item)
            for item in await response.json()
        ]

    async def upsert_daily_weather(
        self, daily: DailyWeather
    ) -> Optional[DailyWeather]:
        try:
            response = await self.post(
                f"/v0/daily/", data=serialise_json(daily)
            )
            return DailyWeather.parse_obj(await response.json())
        except ClientResponseError as error:
            if error.status == 404:
                return None
            raise

    async def listen_daily_weather(
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
