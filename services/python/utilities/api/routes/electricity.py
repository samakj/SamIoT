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

ELECTRICITY_V0_ROUTES = RouteTableDef()


@ELECTRICITY_V0_ROUTES.view("/v0/electricity")
class ElectricitysV0View(PydanticView):
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
        Tags: Electricity
        """
        app: UtilitiesAPIApplication = self.request.app

        if not app.electricity_store:
            raise ValueError(
                "Electricity store not initialised before querying data."
            )

        return json_response(
            await try_route_cache(
                self,
                app.electricity_store.get_electricity_consumption,
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

    async def post(self, electricity: UtilitiesConsumption) -> Response:
        """
        Tags: Electricity
        """
        app: UtilitiesAPIApplication = self.request.app

        if not app.electricity_store:
            raise ValueError(
                "Electricity store not initialised before querying data."
            )

        _electricity = await app.electricity_store.upsert_electricity_consumption(electricity)

        if _electricity is not None and app.cache is not None:
            base_url = f"{'/'.join(str(self.request.url).split('/')[:3])}/v0/electricity"
            invalid_keys = set()
            invalid_keys.update(await app.cache.keys(f"utilities:api:route:{base_url}"))
            invalid_keys.update(await app.cache.keys(f"utilities:api:route:{base_url}?*"))
            invalid_keys.update(await app.cache.keys(f"utilities:api:route:{base_url}/{_electricity.id}"))
            invalid_keys.update(await app.cache.keys(f"utilities:api:route:{base_url}/timestamp/*"))
            if invalid_keys:
                await app.cache.delete(*invalid_keys)

        return json_response(electricity)


@ELECTRICITY_V0_ROUTES.view("/v0/electricity/{id:\d+}")
class ElectricityV0View(PydanticView):
    async def get(self, id: int, /) -> Response:
        """
        Tags: Electricity
        """
        app: UtilitiesAPIApplication = self.request.app

        if not app.electricity_store:
            raise ValueError(
                "Electricity consumption store not initialised before querying data."
            )

        electricity = await try_route_cache(
            self,
            app.electricity_store.get_electricity_consumption,
            args=(id,),
            expiry=15 * 60,
            prefix="utilities:api"
        )

        if electricity is None:
            raise HTTPNotFound()

        return json_response(electricity)


@ELECTRICITY_V0_ROUTES.view("/v0/electricity/timestamp/{timestamp}")
class ElectricityTimestampV0View(PydanticView):
    async def get(self, timestamp: datetime, /) -> Response:
        """
        Tags: Electricity
        """
        app: UtilitiesAPIApplication = self.request.app

        if not app.electricity_store:
            raise ValueError(
                "Electricity store not initialised before querying data."
            )

        electricity = await try_route_cache(
            self,
            app.electricity_store.get_electricity_consumption_by_nearest_timestamp,
            args=(timestamp,),
            expiry=15 * 60,
            prefix="utilities:api"
        )

        if electricity is None:
            raise HTTPNotFound()

        return json_response(electricity)
