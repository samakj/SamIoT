from aiohttp import ClientResponseError
from datetime import datetime
from typing import List, Optional, Union

from shared.python.extensions.aiohttp.api_client import APIClient
from shared.python.extensions.aiohttp.websocket import EventCallback, EventWithMessageCallback, Websocket
from shared.python.json import serialise_json
from shared.python.models.Weather import CurrentWeather


class CurrentClient(APIClient):
    async def get_current_weather(self) -> Optional[CurrentWeather]:
        try:
            response = await self.get(f"/v0/current")
            return CurrentWeather.parse_obj(await response.json())
        except ClientResponseError as error:
            if error.status == 404:
                return None
            raise

    async def upsert_current_weather(
        self, current: CurrentWeather
    ) -> Optional[CurrentWeather]:
        try:
            response = await self.post(
                f"/v0/current/", data=serialise_json(current)
            )
            return CurrentWeather.parse_obj(await response.json())
        except ClientResponseError as error:
            if error.status == 404:
                return None
            raise

    async def listen_current_weather(
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
