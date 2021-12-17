from decimal import Decimal
from typing import List, Optional
from datetime import datetime

from shared.python.models.Utilities import UtilitiesConsumption
from shared.python.stores.BaseStore import BaseStore


class ElectricityStore(BaseStore):
    async def upsert_electricity_consumption(
        self,
        consumption: UtilitiesConsumption
    ) -> Optional[UtilitiesConsumption]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        INSERT INTO electricity (timestamp, consumption)
                        VALUES ($1, $2)
                        ON CONFLICT (timestamp) DO
                            UPDATE SET consumption=$2
                        RETURNING *
                    """,
                    consumption.timestamp,
                    consumption.consumption,
                )

                result = (
                    UtilitiesConsumption.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

                if result is not None:
                    await self.broadcast("UPDATE", result, result.id)

                return result

    async def get_electricity_consumption(self, id: int) -> Optional[UtilitiesConsumption]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    """
                        SELECT * 
                        FROM electricity
                        WHERE id=$1
                    """,
                    id
                )

                return (
                    UtilitiesConsumption.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

    async def get_electricity_consumption_by_timestamp(
        self, timestamp: datetime
    ) -> Optional[UtilitiesConsumption]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    """
                        SELECT * 
                        FROM electricity
                        WHERE timestamp=$1
                    """,
                    timestamp
                )

                return (
                    UtilitiesConsumption.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

    async def get_electricity_consumption_by_nearest_timestamp(
        self, timestamp: datetime
    ) -> Optional[UtilitiesConsumption]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    """
                        SELECT * 
                        FROM (
                            SELECT * 
                            FROM electricity
                            WHERE timestamp<=$1
                            UNION
                            SELECT * 
                            FROM electricity
                            WHERE timestamp>=$1
                        )
                        ORDER BY ABS(extract(epoch from timestamp) - extract(epoch from $1))
                        LIMIT 1
                    """,
                    timestamp
                )

                return (
                    UtilitiesConsumption.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

    async def get_electricity_consumptions(
        self,
        ids: Optional[List[int]] = None,
        timestamps: Optional[List[datetime]] = None,
        timestamp_gte: Optional[Decimal] = None,
        timestamp_lte: Optional[Decimal] = None,
        consumption_gte: Optional[Decimal] = None,
        consumption_lte: Optional[Decimal] = None,
    ) -> List[UtilitiesConsumption]:
        values = []
        filters = []

        if ids is not None:
            filters.append(f"id=ANY(${len(values) + 1})")
            values.append(ids)
        if timestamps is not None:
            filters.append(f"timestamp=ANY(${len(values) + 1})")
            values.append(timestamps)
        if filters:
            filters = [f"({' OR '.join(filters)})"]
        if timestamp_gte:
            filters.append(f"timestamp>${len(values) + 1}")
            values.append(timestamp_gte)
        if timestamp_lte:
            filters.append(f"timestamp>${len(values) + 1}")
            values.append(timestamp_lte)
        if consumption_gte:
            filters.append(f"consumption>${len(values) + 1}")
            values.append(consumption_gte)
        if consumption_lte:
            filters.append(f"consumption>${len(values) + 1}")
            values.append(consumption_lte)
        if not filters:
            filters = ["TRUE"]

        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetch(
                    f"""
                        SELECT *
                        FROM electricity
                        WHERE {" AND ".join(filters)}
                    """,
                    *values
                )

                return [UtilitiesConsumption.parse_obj(dict(row)) for row in db_response]
