import asyncio
from decimal import Decimal
from typing import List, Optional
from aiohttp_pydantic import PydanticView
from datetime import datetime
from aiohttp.web import RouteTableDef, Response, HTTPNotFound

from shared.python.extensions.aiohttp.responses.json import json_response

from Application import UtilitiesAPIApplication
from shared.python.extensions.aioredis import try_route_cache
from shared.python.models.Utilities import UtilitiesConsumption

GAS_V0_ROUTES = RouteTableDef()


@GAS_V0_ROUTES.view("/v0/gas")
class GassV0View(PydanticView):
    async def get(
        self,
        id: Optional[List[int]] = None,
        timestamp: Optional[List[datetime]] = None,
        timestamp_gte: Optional[datetime] = None,
        timestamp_lte: Optional[datetime] = None,
        consumption_gte: Optional[Decimal] = None,
        consumption_lte: Optional[Decimal] = None,
    ) -> Response:
        """
        Tags: Gas
        """
        app: UtilitiesAPIApplication = self.request.app

        if not app.gas_store:
            raise ValueError(
                "Gas store not initialised before querying data."
            )

        return json_response(
            await try_route_cache(
                self,
                app.gas_store.get_gas_consumption,
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
                    "timestamp_gte": timestamp_gte,
                    "timestamp_lte": timestamp_lte,
                    "consumption_gte": consumption_gte,
                    "consumption_lte": consumption_lte,
                },
                expiry=15 * 60,
                prefix="utilities:api"
            )
        )

    async def post(self, gas: UtilitiesConsumption) -> Response:
        """
        Tags: Gas
        """
        app: UtilitiesAPIApplication = self.request.app

        if not app.gas_store:
            raise ValueError(
                "Gas store not initialised before querying data."
            )

        _gas = await app.gas_store.upsert_gas_consumption(gas)

        if _gas is not None and app.cache is not None:
            base_url = f"{'/'.join(str(self.request.url).split('/')[:3])}/v0/gas"
            invalid_keys = set()
            invalid_keys.update(await app.cache.keys(f"utilities:api:route:{base_url}"))
            invalid_keys.update(await app.cache.keys(f"utilities:api:route:{base_url}?*"))
            invalid_keys.update(await app.cache.keys(f"utilities:api:route:{base_url}/{_gas.id}"))
            invalid_keys.update(await app.cache.keys(f"utilities:api:route:{base_url}/timestamp/*"))
            if invalid_keys:
                await app.cache.delete(*invalid_keys)

        return json_response(gas)


@GAS_V0_ROUTES.view("/v0/gas/{id:\d+}")
class GasV0View(PydanticView):
    async def get(self, id: int, /) -> Response:
        """
        Tags: Gas
        """
        app: UtilitiesAPIApplication = self.request.app

        if not app.gas_store:
            raise ValueError(
                "Gas consumption store not initialised before querying data."
            )

        gas = await try_route_cache(
            self,
            app.gas_store.get_gas_consumption,
            args=(id,),
            expiry=15 * 60,
            prefix="utilities:api"
        )

        if gas is None:
            raise HTTPNotFound()

        return json_response(gas)


@GAS_V0_ROUTES.view("/v0/gas/timestamp/{timestamp}")
class GasTimestampV0View(PydanticView):
    async def get(self, timestamp: datetime, /) -> Response:
        """
        Tags: Gas
        """
        app: UtilitiesAPIApplication = self.request.app

        if not app.gas_store:
            raise ValueError(
                "Gas store not initialised before querying data."
            )

        gas = await try_route_cache(
            self,
            app.gas_store.get_gas_consumption_by_nearest_timestamp,
            args=(timestamp,),
            expiry=15 * 60,
            prefix="utilities:api"
        )

        if gas is None:
            raise HTTPNotFound()

        return json_response(gas)
