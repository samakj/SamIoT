from decimal import Decimal
from aiohttp import ClientResponseError
from datetime import datetime
from typing import List, Optional, Union

from shared.python.extensions.aiohttp.api_client import APIClient
from shared.python.extensions.aiohttp.websocket import EventCallback, EventWithMessageCallback, Websocket
from shared.python.json import serialise_json
from shared.python.models.Utilities import UtilitiesConsumption


class GasClient(APIClient):
    async def get_gas_consumption(self, id: int) -> Optional[UtilitiesConsumption]:
        try:
            response = await self.get(f"/v0/gas/{id}")
            return UtilitiesConsumption.parse_obj(await response.json())
        except ClientResponseError as error:
            if error.status == 404:
                return None
            raise

    async def get_gas_consumption_by_timestamp(
        self, timestamp: datetime
    ) -> Optional[UtilitiesConsumption]:
        try:
            response = await self.get(f"/v0/gas/timestamp/{timestamp}")
            return UtilitiesConsumption.parse_obj(await response.json())
        except ClientResponseError as error:
            if error.status == 404:
                return None
            raise

    async def get_gas_consumptions(
        self,
        id: Optional[Union[int, List[int]]] = None,
        timestamp: Optional[Union[datetime, List[datetime]]] = None,
        timestamp_gte: Optional[datetime] = None,
        timestamp_lte: Optional[datetime] = None,
        consumption_gte: Optional[Decimal] = None,
        consumption_lte: Optional[Decimal] = None,
    ) -> List[UtilitiesConsumption]:
        response = await self.get(
            "/v0/gas",
            params={
                "id": id,
                "timestamp": timestamp,
                "timestamp_gte": timestamp_gte,
                "timestamp_lte": timestamp_lte,
                "apparent_temperature_gte": consumption_gte,
                "apparent_temperature_lte": consumption_lte,
            }
        )

        return [
            UtilitiesConsumption.parse_obj(item)
            for item in await response.json()
        ]

    async def upsert_gas_consumption(
        self, gas: UtilitiesConsumption
    ) -> Optional[UtilitiesConsumption]:
        try:
            response = await self.post(
                f"/v0/gas/", data=serialise_json(gas)
            )
            return UtilitiesConsumption.parse_obj(await response.json())
        except ClientResponseError as error:
            if error.status == 404:
                return None
            raise

    async def listen_gas_consumption(
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
