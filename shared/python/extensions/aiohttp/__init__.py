import asyncpg
import aioredis
from asyncpg import Pool
from aioredis import Redis
from aiohttp.web import Application
from typing import Optional, Union


class ApplicationWithDatabase(Application):
    db: Optional[Pool] = None

    async def connect_db(
        self,
        host: str,
        port: Union[int, str] = 5432,
        name: Optional[str] = None,
        username: Optional[str] = None,
        password: Optional[str] = None
    ) -> None:
        user_prefix = ""
        db_name_suffix = ""

        if username is not None:
            user_prefix = f"{username}"
            if password is not None:
                user_prefix += f":{password}"
            user_prefix += "@"
        if name is not None:
            db_name_suffix = f"/{name}"

        self.db = await asyncpg.create_pool(
            f"postgresql://{user_prefix}{host}:{port}{db_name_suffix}"
        )


class ApplicationWithCache(Application):
    cache: Optional[Redis] = None

    async def connect_cache(
        self,
        host: str,
        port: Union[int, str] = 5432,
        name: Optional[str] = None,
        username: Optional[str] = None,
        password: Optional[str] = None
    ) -> None:
        user_prefix = ""
        db_name_suffix = ""

        if username is not None:
            user_prefix = f"{username}"
            if password is not None:
                user_prefix += f":{password}"
            user_prefix += "@"
        if name is not None:
            db_name_suffix = f"/{name}"

        self.db = await aioredis.from_url(
            f"redis://{user_prefix}{host}:{port}{db_name_suffix}"
        )


class ApplicationWithDatabaseAndCache(ApplicationWithDatabase, ApplicationWithCache):
    pass
