from typing import Dict, List, Optional

from shared.python.models.Metric import Metric
from shared.python.stores.BaseStore import BaseStore


class MetricsStore(BaseStore):
    async def create_metric(
        self, name: str, abbreviation: str, unit: Optional[str] = None
    ) -> Optional[Metric]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        INSERT INTO metrics (name, abbreviation, unit)
                        VALUES ($1, $2, $3)
                        RETURNING id, name, abbreviation, unit
                    """,
                    name,
                    abbreviation,
                    unit
                )

                result = (
                    Metric.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

                if result is not None:
                    await self.broadcast("CREATE", result, result.id)

                return result

    async def get_metric(self, id: int) -> Optional[Metric]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        SELET id, name, abbreviation, unit
                        FROM metrics
                        WHERE id=$1
                    """,
                    id,
                )

                return (
                    Metric.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

    async def get_metric_by_name(self, name: str) -> Optional[Metric]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        SELET id, name, abbreviation, unit
                        FROM metrics
                        WHERE name=$1
                    """,
                    name,
                )

                return (
                    Metric.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

    async def get_metric_by_abbreviation(
        self, abbreviation: str
    ) -> Optional[Metric]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        SELET id, name, abbreviation, unit
                        FROM metrics
                        WHERE abbreviation=$1
                    """,
                    abbreviation,
                )

                return (
                    Metric.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

    async def get_metrics(
        self,
        ids: Optional[List[str]] = None,
        names: Optional[List[str]] = None,
        abbreviations: Optional[List[str]] = None,
    ) -> List[Metric]:
        filters = []

        if ids is not None:
            filters.append("id=ANY($1)")
        if names is not None:
            filters.append("name=ANY($2)")
        if abbreviations is not None:
            filters.append("abbreviation=ANY($2)")

        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetch(
                    f"""
                        SELET id, name, abbreviation, unit
                        FROM metrics
                        WHERE {" OR ".join(filters)}
                    """,
                    ids,
                    names,
                    abbreviations
                )

                return [Metric.parse_obj(dict(row)) for row in db_response]

    async def update_metric(
        self,
        id: int,
        name: Optional[str] = None,
        abbreviation: Optional[str] = None,
        unit: Optional[str] = None,
    ) -> Optional[Metric]:
        updates = []

        if name is not None:
            updates.append("name=$1")
        if abbreviation is not None:
            updates.append("abbreviation=$2")
        if unit is not None:
            updates.append("unit=$2")
        if not updates:
            return await self.get_metric(id)

        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        UPDATE metrics
                        SET {", ".join(updates)}
                        WHERE id=$1
                        RETURNING id, name, abbreviation, unit
                    """,
                    id,
                    name,
                    abbreviation,
                    unit
                )

                result = (
                    Metric.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

                if result is not None:
                    await self.broadcast("UPDATE", result, id)

                return result

    async def delete_metric(
        self,
        id: int,
    ) -> Optional[Dict[str, int]]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        DELETE FROM metrics
                        WHERE id=$1
                        RETURN TRUE
                    """,
                    id
                )

                result = (
                    {"id": id}
                    if db_response else
                    None
                )

                if result is not None:
                    await self.broadcast("DELETE", result, id)

                return result
