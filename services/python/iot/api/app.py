import asyncio
import logging
import os
from dotenv import load_dotenv

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


async def create_app() -> IoTAPIApplication:
    try:
        app = IoTAPIApplication(
            middlewares=[
                request_logger(
                    [{"method": "PATCH", "regex": "/v0/devices/[\d]+"}]
                ),
                error_handler
            ]
        )

        await app.connect_db(
            host=os.environ["IOT_DB_HOST"],
            port=os.environ["IOT_DB_PORT"],
            name=os.environ["IOT_DB_NAME"],
            username=os.environ["IOT_DB_USER"],
            password=os.environ["IOT_DB_PASS"],
        )

        app.connect_devices_store()
        app.connect_locations_store()
        app.connect_measurements_store()
        app.connect_metrics_store()

        app.add_routes(DEFAULT_ROUTES)
        app.add_routes(DEVICES_V0_ROUTES)
        app.add_routes(LOCATIONS_V0_ROUTES)
        app.add_routes(MEASUREMENTS_V0_ROUTES)
        app.add_routes(METRICS_V0_ROUTES)

        app.on_shutdown.append(cleanup_sockets)
    except:
        await asyncio.sleep(5)
        raise
    return app
