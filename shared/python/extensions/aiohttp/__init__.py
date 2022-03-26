import logging
import asyncpg
import asyncio
from asyncpg import Pool
from aioredis import Redis
from aiohttp.web import Application
from typing import Optional, Union

LOG = logging.getLogger()

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

        LOG.info(
            f"Connecting to db at: postgresql://{host}:{port}{db_name_suffix}, " +
            f"username: {username if username is not None else 'None'}"
        )
        
        while self.db is None:
            try:
                self.db = await asyncpg.create_pool(
                    f"postgresql://{user_prefix}{host}:{port}{db_name_suffix}"
                )
            except:
                pass
            
            if self.db is None:
                LOG.warning("Failed to connect to db, trying again in 5 seconds.")
                await asyncio.sleep(5)


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
        cache_name_suffix = ""

        if username is not None:
            user_prefix = f"{username}"
            if password is not None:
                user_prefix += f":{password}"
            user_prefix += "@"
        if name is not None:
            cache_name_suffix = f"/{name}"
        
        LOG.info(
            f"Connecting to cache at: redis://{host}:{port}{cache_name_suffix}, " +
            f"username: {username if username is not None else 'None'}"
        )

        while self.cache is None:
            try:
                self.cache = await Redis.from_url(
                    f"redis://{user_prefix}{host}:{port}{cache_name_suffix}"
                )
            except:
                pass
                
            if self.cache is None:
                LOG.warning("Failed to connect to cache, trying again in 5 seconds.")
                await asyncio.sleep(5)


class ApplicationWithDatabaseAndCache(ApplicationWithDatabase, ApplicationWithCache):
    pass
