from typing import Any, Dict, List, Optional

from shared.python.models.Location import Location
from shared.python.stores.BaseStore import BaseStore


class LocationsStore(BaseStore):
    async def create_location(
        self, name: str, tags: List[str] = []
    ) -> Optional[Location]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        INSERT INTO locations (name, tags)
                        VALUES ($1, $2)
                        RETURNING id, name, tags
                    """,
                    name,
                    tags
                )

                result = (
                    Location.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

                if result is not None:
                    await self.broadcast("CREATE", result, result.id)

                return result

    async def get_location(self, id: int) -> Optional[Location]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        SELECT id, name, tags
                        FROM locations
                        WHERE id=$1
                    """,
                    id,
                )

                return (
                    Location.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

    async def get_location_by_name(self, name: str) -> Optional[Location]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        SELECT id, name, tags
                        FROM locations
                        WHERE name=$1
                    """,
                    name,
                )

                return (
                    Location.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

    async def get_locations(
        self,
        ids: Optional[List[int]] = None,
        names: Optional[List[str]] = None,
        tags: Optional[List[str]] = None,
    ) -> List[Location]:
        values = []
        filters = []

        if ids is not None:
            filters.append(f"id=ANY(${len(values) + 1})")
            values.append(ids)
        if names is not None:
            filters.append(f"name=ANY(${len(values) + 1})")
            values.append(names)
        if filters and tags is not None:
            filters = [f"({' OR '.join(filters)})"]
        if tags is not None:
            filters.append(f"tags&&${len(values) + 1}")
            values.append(tags)
        if not filters:
            filters = ["TRUE"]

        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetch(
                    f"""
                        SELECT id, name, tags
                        FROM locations
                        WHERE {" AND ".join(filters)}
                    """,
                    *values
                )

                return [Location.parse_obj(dict(row)) for row in db_response]

    async def update_location(
        self,
        id: int,
        name: Optional[str] = None,
        tags: Optional[List[str]] = None,
    ) -> Optional[Location]:
        values: List[Any] = [id]
        updates = []

        if name is not None:
            updates.append(f"name=${len(values) + 1}")
            values.append(name)
        if tags is not None:
            updates.append(f"tags=${len(values) + 1}")
            values.append(tags)
        if not updates:
            return await self.get_location(id)

        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        UPDATE locations
                        SET {", ".join(updates)}
                        WHERE id=$1
                        RETURNING id, name, tags
                    """,
                    *values
                )

                result = (
                    Location.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

                if result is not None:
                    await self.broadcast("UPDATE", result, id)

                return result

    async def add_location_tag(
        self,
        id: int,
        tag: str,
    ) -> Optional[Location]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        UPDATE locations
                        SET tags=ARRAY_APPEND(tags, $2)
                        WHERE id=$1
                        RETURNING id, name, tags
                    """,
                    id,
                    tag
                )

                result = (
                    Location.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

                if result is not None:
                    await self.broadcast("UPDATE", result, id)

                return result

    async def remove_location_tag(
        self,
        id: int,
        tag: str,
    ) -> Optional[Location]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        UPDATE locations
                        SET tags=ARRAY_REMOVE(tags, $2)
                        WHERE id=$1
                        RETURNING id, name, tags
                    """,
                    id,
                    tag
                )

                result = (
                    Location.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

                if result is not None:
                    await self.broadcast("UPDATE", result, id)

                return result

    async def delete_location(
        self,
        id: int,
    ) -> Optional[Dict[str, int]]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        DELETE FROM locations
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
