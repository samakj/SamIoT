from aiohttp import ClientResponseError
from datetime import datetime
from typing import Any, Dict, List, Optional, Union

from shared.python.extensions.aiohttp.api_client import APIClient
from shared.python.extensions.aiohttp.websocket import EventCallback, EventWithMessageCallback, Websocket
from shared.python.models.Measurement import Measurement
from shared.python.json import serialise_json, to_json_serialisable


class MeasurementsClient(APIClient):
    async def create_measurement(self, measurement: Measurement) -> Optional[Measurement]:
        try:
            response = await self.post(f"/v0/measurements", data=serialise_json(measurement))
            return Measurement.parse_obj(await response.json())
        except ClientResponseError as error:
            if error.status == 404:
                return None
            raise

    async def get_measurement(self, id: int) -> Optional[Measurement]:
        try:
            response = await self.get(f"/v0/measurements/{id}")
            return Measurement.parse_obj(await response.json())
        except ClientResponseError as error:
            if error.status == 404:
                return None
            raise

    async def get_measurements(
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
    ) -> List[Measurement]:
        response = await self.get(
            "/v0/measurements",
            params={
                "id": id,
                "device_id": device_id,
                "location_id": location_id,
                "metric_id": metric_id,
                "tags": tags,
                "timestamp_gte": timestamp_gte,
                "timestamp_lte": timestamp_lte,
                "value": value,
                "value_gte": value_gte,
                "value_lte": value_lte,
            }
        )

        return [
            Measurement.parse_obj(item)
            for item in await response.json()
        ]

    async def get_latest_measurements(
        self,
        device_id: Optional[Union[int, List[int]]] = None,
        location_id: Optional[Union[int, List[int]]] = None,
        metric_id: Optional[Union[int, List[int]]] = None,
        tags: Optional[Union[str, List[str]]] = None,
    ) -> List[Measurement]:
        response = await self.get(
            "/v0/measurements/latest",
            params={
                "device_id": device_id,
                "location_id": location_id,
                "metric_id": metric_id,
                "tags": tags,
            }
        )

        return [
            Measurement.parse_obj(item)
            for item in await response.json()
        ]

    async def get_average_measurements(
        self,
        location_id: int,
        metric_id: int,
        tags: str,
        start: Optional[datetime] = None,
        end: Optional[datetime] = None,
        period: Optional[int] = None
    ) -> List[Dict[str, Any]]:
        response = await self.get(
            f"/v0/measurements/average/{location_id}/{metric_id}/{tags.join(',')}",
            params=to_json_serialisable(
                {"start": start, "end": end, "period": period}
            )
        )

        return await response.json()

    async def listen_measurement(
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
            f"/v0/measurements/ws/{id}",
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

    async def listen_measurements(
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
            f"/v0/measurements/ws",
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
