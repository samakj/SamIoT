from typing import Dict, List, Optional
from asyncpg import Pool

from shared.python.models.Location import Location


class LocationsStore:
    db: Pool

    def __init__(self, db: Pool) -> None:
        self.db = db

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

                return (
                    Location.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

    async def get_location(self, id: int) -> Optional[Location]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        SELET id, name, tags
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
                        SELET id, name, tags
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
        ids: Optional[List[str]] = None,
        names: Optional[List[str]] = None,
        tags: Optional[List[str]] = None,
    ) -> List[Location]:
        filters = []

        if ids is not None:
            filters.append("id=ANY($1)")
        if names is not None:
            filters.append("name=ANY($2)")
        if tags is not None:
            filters = [f"({' OR '.join(filters)})", "tags&&$3"]

        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        SELET id, name, tags
                        FROM locations
                        WHERE {" AND ".join(filters)}
                    """,
                    ids,
                    names,
                    tags
                )

                return [Location.parse_obj(dict(row)) for row in db_response]

    async def update_location(
        self,
        id: int,
        name: Optional[str] = None,
        tags: Optional[List[str]] = None,
    ) -> Optional[Location]:
        updates = []

        if name is not None:
            updates.append("name=$1")
        if tags is not None:
            updates.append("tags=$2")
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
                    name,
                    tags
                )

                return (
                    Location.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

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

                return (
                    Location.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

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

                return (
                    Location.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

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

                return (
                    {"id": id}
                    if db_response else
                    None
                )
