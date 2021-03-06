from typing import Any, Dict, List, Optional

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
                        SELECT id, name, abbreviation, unit
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
                        SELECT id, name, abbreviation, unit
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
                        SELECT id, name, abbreviation, unit
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
        ids: Optional[List[int]] = None,
        names: Optional[List[str]] = None,
        abbreviations: Optional[List[str]] = None,
    ) -> List[Metric]:
        values = []
        filters = []

        if ids is not None:
            filters.append(f"id=ANY(${len(values) + 1})")
            values.append(ids)
        if names is not None:
            filters.append(f"name=ANY(${len(values) + 1})")
            values.append(names)
        if abbreviations is not None:
            filters.append(f"abbreviation=ANY(${len(values) + 1})")
            values.append(abbreviations)
        if not filters:
            filters = ["TRUE"]

        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetch(
                    f"""
                        SELECT id, name, abbreviation, unit
                        FROM metrics
                        WHERE {" OR ".join(filters)}
                    """,
                    *values
                )

                return [Metric.parse_obj(dict(row)) for row in db_response]

    async def update_metric(
        self,
        id: int,
        name: Optional[str] = None,
        abbreviation: Optional[str] = None,
        unit: Optional[str] = None,
    ) -> Optional[Metric]:
        values: List[Any] = [id]
        updates = []

        if name is not None:
            updates.append(f"name=${len(values) + 1}")
            values.append(name)
        if abbreviation is not None:
            updates.append(f"abbreviation=${len(values) + 1}")
            values.append(abbreviation)
        if unit is not None:
            updates.append(f"unit=${len(values) + 1}")
            values.append(unit)
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
                    *values
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
