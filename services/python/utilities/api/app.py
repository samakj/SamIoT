import asyncio
import logging
import os
from dotenv import load_dotenv
from aiohttp_pydantic import oas

from Application import UtilitiesAPIApplication
from routes.electricity import ELECTRICITY_V0_ROUTES
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


async def create_app() -> UtilitiesAPIApplication:
    try:
        app = UtilitiesAPIApplication(
            middlewares=[
                request_logger(
                    [{"method": "PATCH", "regex": "/v0/devices/[\d]+"}]
                ),
                error_handler
            ]
        )

        await app.connect_db(
            host=os.environ["UTILITIES_DB_HOST"],
            port=os.environ["UTILITIES_DB_PORT"],
            name=os.environ["UTILITIES_DB_NAME"],
            username=os.environ["UTILITIES_DB_USER"],
            password=os.environ["UTILITIES_DB_PASS"],
        )

        app.connect_gas_store()
        app.connect_electricity_store()

        await app.connect_cache(
            host=os.environ["UTILITIES_CACHE_HOST"],
            port=os.environ["UTILITIES_CACHE_PORT"],
            # name=os.environ["UTILITIES_CACHE_NAME"],
            # username=os.environ["UTILITIES_CACHE_USER"],
            # password=os.environ["UTILITIES_CACHE_PASS"],
        )

        app.add_routes(DEFAULT_ROUTES)
        app.add_routes(GAS_V0_ROUTES)
        app.add_routes(ELECTRICITY_V0_ROUTES)

        oas.setup(app, title_spec="Utilities API", url_prefix="/docs")
    except:
        await asyncio.sleep(5)
        raise
    return app
