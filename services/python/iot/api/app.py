import asyncio
import logging
import os
import re
from aiohttp.web_response import json_response
from dotenv import load_dotenv
from aiohttp.web import normalize_path_middleware
from aiohttp_pydantic import oas
from aiohttp_middlewares import cors_middleware

from Application import IoTAPIApplication
from routes.locations import LOCATIONS_V0_ROUTES
from routes.devices import DEVICES_V0_ROUTES
from routes.metrics import METRICS_V0_ROUTES
from routes.measurements import MEASUREMENTS_V0_ROUTES
from shared.python.extensions.aiohttp.middleware.error_handler import error_handler
from shared.python.extensions.aiohttp.middleware.request_logger import request_logger
from shared.python.extensions.aiohttp.routes.default import DEFAULT_ROUTES

load_dotenv()
logging.basicConfig(
    level=logging.INFO,
    format="[%(asctime)s] %(levelname)s: %(message)s"
)
LOG = logging.getLogger(__name__)


async def cleanup_sockets(app: IoTAPIApplication):
    if app.devices_store is not None:
        await app.devices_store.unlisten_all()
    if app.locations_store is not None:
        await app.locations_store.unlisten_all()
    if app.measurements_store is not None:
        await app.measurements_store.unlisten_all()
    if app.metrics_store is not None:
        await app.metrics_store.unlisten_all()


async def close_db(app: IoTAPIApplication):
    if app.db is not None:
        await app.db.close()


async def close_cache(app: IoTAPIApplication):
    if app.cache is not None:
        await app.cache.close()


async def create_app() -> IoTAPIApplication:
    try:
        app = IoTAPIApplication(
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
                host=os.environ["IOT_DB_HOST"],
                port=os.environ["IOT_DB_PORT"],
                name=os.environ["IOT_DB_NAME"],
                username=os.environ["IOT_DB_USER"],
                password=os.environ["IOT_DB_PASS"],
            )
        except:
            LOG.error('Failed to connect to db, restart to try again.')

        app.connect_devices_store()
        app.connect_locations_store()
        app.connect_measurements_store()
        app.connect_metrics_store()

        await app.connect_cache(
            host=os.environ["IOT_CACHE_HOST"],
            port=os.environ["IOT_CACHE_PORT"],
            # name=os.environ["IOT_CACHE_NAME"],
            # username=os.environ["IOT_CACHE_USER"],
            # password=os.environ["IOT_CACHE_PASS"],
        )

        app.add_routes(DEFAULT_ROUTES)
        app.add_routes(DEVICES_V0_ROUTES)
        app.add_routes(LOCATIONS_V0_ROUTES)
        app.add_routes(MEASUREMENTS_V0_ROUTES)
        app.add_routes(METRICS_V0_ROUTES)

        oas.setup(app, title_spec="IoT API", url_prefix="/docs")

        app.on_shutdown.append(cleanup_sockets)
        app.on_shutdown.append(close_db)
        app.on_shutdown.append(close_cache)
    except:
        await asyncio.sleep(5)
        raise
    return app
