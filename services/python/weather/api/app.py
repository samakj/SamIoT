import asyncio
import logging
import os
from dotenv import load_dotenv
from aiohttp_pydantic import oas

from Application import WeatherAPIApplication
from services.python.weather.api.routes.daily import DAILY_V0_ROUTES
from services.python.weather.api.routes.hourly import HOURLY_V0_ROUTES
from services.python.weather.api.routes.current import CURRENT_V0_ROUTES
from shared.python.extensions.aiohttp.middleware.error_handler import error_handler
from shared.python.extensions.aiohttp.middleware.request_logger import request_logger
from shared.python.extensions.aiohttp.routes.default import DEFAULT_ROUTES

load_dotenv()
logging.basicConfig(
    level=logging.INFO,
    format="[%(asctime)s] %(levelname)s: %(message)s"
)
LOG = logging.getLogger(__name__)


async def create_app() -> WeatherAPIApplication:
    try:
        app = WeatherAPIApplication(
            middlewares=[
                request_logger(
                    [{"method": "PATCH", "regex": "/v0/devices/[\d]+"}]
                ),
                error_handler
            ]
        )

        await app.connect_db(
            host=os.environ["WATHER_DB_HOST"],
            port=os.environ["WATHER_DB_PORT"],
            name=os.environ["WATHER_DB_NAME"],
            username=os.environ["WATHER_DB_USER"],
            password=os.environ["WATHER_DB_PASS"],
        )

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
    except:
        await asyncio.sleep(5)
        raise
    return app
