import asyncio
import logging
import os
from dotenv import load_dotenv
from aiohttp_pydantic import oas

from Application import UtilitiesScraperApplication
from shared.python.extensions.aiohttp.middleware.error_handler import error_handler
from shared.python.extensions.aiohttp.middleware.request_logger import request_logger
from shared.python.extensions.aiohttp.routes.default import DEFAULT_ROUTES

load_dotenv()
logging.basicConfig(
    level=logging.INFO,
    format="[%(asctime)s] %(levelname)s: %(message)s"
)
LOG = logging.getLogger(__name__)


async def create_app() -> UtilitiesScraperApplication:
    try:
        app = UtilitiesScraperApplication(
            middlewares=[
                request_logger(
                    [{"method": "PATCH", "regex": "/v0/devices/[\d]+"}]
                ),
                error_handler
            ]
        )

        app.connect_octopus_energy_client()
        app.connect_utilities_client()
        app.connect_gas_job()
        app.connect_electric_job()

        app.add_routes(DEFAULT_ROUTES)

        oas.setup(app, title_spec="Utilities Scraper", url_prefix="/docs")

        if app.gas_job is not None:
            await app.gas_job.start()
        if app.electric_job is not None:
            await app.electric_job.start()
    except:
        await asyncio.sleep(5)
        raise
    return app
