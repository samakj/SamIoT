from aiohttp import ClientResponseError
from typing import List, Optional, Union

from shared.python.extensions.aiohttp.api_client import APIClient
from shared.python.extensions.aiohttp.websocket import EventCallback, EventWithMessageCallback, Websocket
from shared.python.models.Metric import Metric


class MetricsClient(APIClient):
    async def get_metric(self, id: int) -> Optional[Metric]:
        try:
            response = await self.get(f"/v0/metrics/{id}")
            return Metric.parse_obj(await response.json())
        except ClientResponseError as error:
            if error.status == 404:
                return None
            raise

    async def get_metric_by_name(self, name: str) -> Optional[Metric]:
        try:
            response = await self.get(f"/v0/metrics/name/{name}")
            return Metric.parse_obj(await response.json())
        except ClientResponseError as error:
            if error.status == 404:
                return None
            raise

    async def get_metric_by_abbreviation(self, abbreviation: str) -> Optional[Metric]:
        try:
            response = await self.get(f"/v0/metrics/abbreviation/{abbreviation}")
            return Metric.parse_obj(await response.json())
        except ClientResponseError as error:
            if error.status == 404:
                return None
            raise

    async def get_metrics(
        self,
        id: Optional[Union[int, List[int]]] = None,
        name: Optional[Union[str, List[str]]] = None,
        abbreviation: Optional[Union[str, List[str]]] = None
    ) -> List[Metric]:
        response = await self.get(
            "/v0/metrics",
            params={"id": id, "name": name, "abbreviation": abbreviation}
        )

        return [
            Metric.parse_obj(item)
            for item in await response.json()
        ]

    async def listen_metric(
        self,
        id: int,
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
            f"/v0/metrics/ws/{id}",
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

    async def listen_metrics(
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
            f"/v0/metrics/ws",
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
