import asyncio
import logging
import os
import re
from aiohttp.web_middlewares import normalize_path_middleware
from aiohttp_middlewares.cors import cors_middleware
from dotenv import load_dotenv
from aiohttp_pydantic import oas

from Application import UtilitiesAPIApplication
from routes.electric import ELECTRIC_V0_ROUTES
from routes.gas import GAS_V0_ROUTES
from shared.python.extensions.aiohttp.middleware.error_handler import error_handler
from shared.python.extensions.aiohttp.middleware.request_logger import request_logger
from shared.python.extensions.aiohttp.routes.default import DEFAULT_ROUTES

load_dotenv()
logging.basicConfig(
    level=logging.INFO,
    format="[%(asctime)s] %(levelname)s: %(message)s"
)
LOG = logging.getLogger(__name__)


async def close_db(app: UtilitiesAPIApplication):
    if app.db is not None:
        await app.db.close()


async def close_cache(app: UtilitiesAPIApplication):
    if app.cache is not None:
        await app.cache.close()


async def create_app() -> UtilitiesAPIApplication:
    try:
        app = UtilitiesAPIApplication(
            middlewares=[
                cors_middleware(
                    origins=[re.compile(r"^https?\:\/\/localhost")]
                ),
                request_logger(
                    [{"method": "PATCH", "regex": "/v0/devices/[\d]+"}]
                ),
                error_handler,
                normalize_path_middleware(
                    append_slash=False,
                    remove_slash=True
                ),
            ]
        )

        try:
            await app.connect_db(
                host=os.environ["UTILITIES_DB_HOST"],
                port=os.environ["UTILITIES_DB_PORT"],
                name=os.environ["UTILITIES_DB_NAME"],
                username=os.environ["UTILITIES_DB_USER"],
                password=os.environ["UTILITIES_DB_PASS"],
            )
        except:
            LOG.error('Failed to connect to db, restart to try again.')

        app.connect_gas_store()
        app.connect_electric_store()

        await app.connect_cache(
            host=os.environ["UTILITIES_CACHE_HOST"],
            port=os.environ["UTILITIES_CACHE_PORT"],
            # name=os.environ["UTILITIES_CACHE_NAME"],
            # username=os.environ["UTILITIES_CACHE_USER"],
            # password=os.environ["UTILITIES_CACHE_PASS"],
        )

        app.add_routes(DEFAULT_ROUTES)
        app.add_routes(GAS_V0_ROUTES)
        app.add_routes(ELECTRIC_V0_ROUTES)

        oas.setup(app, title_spec="Utilities API", url_prefix="/docs")

        app.on_shutdown.append(close_db)
        app.on_shutdown.append(close_cache)
    except:
        await asyncio.sleep(5)
        raise
    return app
