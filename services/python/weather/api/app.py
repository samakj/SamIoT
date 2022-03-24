import asyncio
import logging
import os
import re
from aiohttp.web_middlewares import normalize_path_middleware
from aiohttp_middlewares.cors import cors_middleware
from dotenv import load_dotenv
from aiohttp_pydantic import oas

from Application import WeatherAPIApplication
from routes.daily import DAILY_V0_ROUTES
from routes.hourly import HOURLY_V0_ROUTES
from routes.current import CURRENT_V0_ROUTES
from shared.python.extensions.aiohttp.middleware.error_handler import error_handler
from shared.python.extensions.aiohttp.middleware.request_logger import request_logger
from shared.python.extensions.aiohttp.routes.default import DEFAULT_ROUTES

load_dotenv()
logging.basicConfig(
    level=logging.INFO,
    format="[%(asctime)s] %(levelname)s: %(message)s"
)
LOG = logging.getLogger(__name__)


async def close_db(app: WeatherAPIApplication):
    if app.db is not None:
        await app.db.close()


async def close_cache(app: WeatherAPIApplication):
    if app.cache is not None:
        await app.cache.close()


async def create_app() -> WeatherAPIApplication:
    try:
        app = WeatherAPIApplication(
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
                host=os.environ["WEATHER_DB_HOST"],
                port=os.environ["WEATHER_DB_PORT"],
                name=os.environ["WEATHER_DB_NAME"],
                username=os.environ["WEATHER_DB_USER"],
                password=os.environ["WEATHER_DB_PASS"],
            )
        except:
            LOG.error('Failed to connect to db, restart to try again.')

        app.connect_current_store()
        app.connect_hourly_store()
        app.connect_daily_store()

        await app.connect_cache(
            host=os.environ["WEATHER_CACHE_HOST"],
            port=os.environ["WEATHER_CACHE_PORT"],
            # name=os.environ["WEATHER_CACHE_NAME"],
            # username=os.environ["WEATHER_CACHE_USER"],
            # password=os.environ["WEATHER_CACHE_PASS"],
        )

        app.add_routes(DEFAULT_ROUTES)
        app.add_routes(CURRENT_V0_ROUTES)
        app.add_routes(HOURLY_V0_ROUTES)
        app.add_routes(DAILY_V0_ROUTES)

        oas.setup(app, title_spec="Weather API", url_prefix="/docs")

        app.on_shutdown.append(close_db)
        app.on_shutdown.append(close_cache)
    except:
        await asyncio.sleep(5)
        raise
    return app
