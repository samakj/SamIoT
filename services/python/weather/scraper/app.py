import asyncio
import logging
import os
from dotenv import load_dotenv
from aiohttp_pydantic import oas

from Application import WeatherScraperApplication
from shared.python.extensions.aiohttp.middleware.error_handler import error_handler
from shared.python.extensions.aiohttp.middleware.request_logger import request_logger
from shared.python.extensions.aiohttp.routes.default import DEFAULT_ROUTES

load_dotenv()
logging.basicConfig(
    level=logging.INFO,
    format="[%(asctime)s] %(levelname)s: %(message)s"
)
LOG = logging.getLogger(__name__)


async def create_app() -> WeatherScraperApplication:
    try:
        app = WeatherScraperApplication(
            middlewares=[
                request_logger(
                    [{"method": "PATCH", "regex": "/v0/devices/[\d]+"}]
                ),
                error_handler
            ]
        )

        app.connect_open_weather_map_client()
        app.connect_weather_client()
        app.connect_forecast_job()

        app.add_routes(DEFAULT_ROUTES)

        oas.setup(app, title_spec="Weather Scraper", url_prefix="/docs")

        if app.forecast_job is not None:
            await app.forecast_job.start()
    except:
        await asyncio.sleep(5)
        raise
    return app
