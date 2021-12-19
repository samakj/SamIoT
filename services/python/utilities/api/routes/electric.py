import asyncio
from decimal import Decimal
from typing import List, Optional
from aiohttp_pydantic import PydanticView
from datetime import datetime
from aiohttp.web import RouteTableDef, Response, HTTPNotFound

from shared.python.extensions.aiohttp.responses.json import json_response

from Application import UtilitiesAPIApplication
from shared.python.extensions.aioredis import CachedJSONResponse
from shared.python.models.Utilities import UtilitiesConsumption

ELECTRIC_V0_ROUTES = RouteTableDef()


@ELECTRIC_V0_ROUTES.view("/v0/electric")
class ElectricsV0View(PydanticView):
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
        Tags: Electric
        """
        app: UtilitiesAPIApplication = self.request.app

        if not app.electric_store:
            raise ValueError(
                "Electric store not initialised before querying data."
            )

        cache = CachedJSONResponse(
            self,
            app.electric_store.get_electric_consumptions,
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

        return await cache.get_response()

    async def post(self, electric: UtilitiesConsumption) -> Response:
        """
        Tags: Electric
        """
        app: UtilitiesAPIApplication = self.request.app

        if not app.electric_store:
            raise ValueError(
                "Electric store not initialised before querying data."
            )

        _electric = await app.electric_store.upsert_electric_consumption(electric)

        if _electric is not None and app.cache is not None:
            base_url = f"{'/'.join(str(self.request.url).split('/')[:3])}/v0/electric"
            invalid_keys = set()
            invalid_keys.update(await app.cache.keys(f"utilities:api:route:{base_url}"))
            invalid_keys.update(await app.cache.keys(f"utilities:api:route:{base_url}?*"))
            invalid_keys.update(await app.cache.keys(f"utilities:api:route:{base_url}/{_electric.id}"))
            invalid_keys.update(await app.cache.keys(f"utilities:api:route:{base_url}/timestamp/*"))
            if invalid_keys:
                await app.cache.delete(*invalid_keys)

        return json_response(electric)


@ELECTRIC_V0_ROUTES.view("/v0/electric/{id:\d+}")
class ElectricV0View(PydanticView):
    async def get(self, id: int, /) -> Response:
        """
        Tags: Electric
        """
        app: UtilitiesAPIApplication = self.request.app

        if not app.electric_store:
            raise ValueError(
                "Electric consumption store not initialised before querying data."
            )

        cache = CachedJSONResponse(
            self,
            app.electric_store.get_electric_consumption,
            args=(id,),
            expiry=15 * 60,
            prefix="utilities:api"
        )

        return await cache.get_response()


@ELECTRIC_V0_ROUTES.view("/v0/electric/timestamp/{timestamp}")
class ElectricTimestampV0View(PydanticView):
    async def get(self, timestamp: datetime, /) -> Response:
        """
        Tags: Electric
        """
        app: UtilitiesAPIApplication = self.request.app

        if not app.electric_store:
            raise ValueError(
                "Electric store not initialised before querying data."
            )

        cache = CachedJSONResponse(
            self,
            app.electric_store.get_electric_consumption_by_nearest_timestamp,
            args=(timestamp,),
            expiry=15 * 60,
            prefix="utilities:api"
        )

        return await cache.get_response()
