import asyncio
import logging
import os
from dotenv import load_dotenv

from Application import IoTAPIApplication
from shared.python.extensions.aiohttp.middleware.error_handler import error_handler
from shared.python.extensions.aiohttp.middleware.request_logger import request_logger

load_dotenv()
logging.basicConfig(
    level=logging.INFO,
    format="[%(asctime)s] %(levelname)s: %(message)s"
)
LOG = logging.getLogger(__name__)


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

        # app.on_shutdown.append(lambda _: app["devices_store"].unlisten_all())
        # app.on_shutdown.append(lambda _: app["locations_store"].unlisten_all())
        # app.on_shutdown.append(lambda _: app["measurements_store"].unlisten_all())
        # app.on_shutdown.append(lambda _: app["metrics_store"].unlisten_all())
        # app.on_shutdown.append(lambda _: app["raw_reports_store"].unlisten_all())
    except:
        await asyncio.sleep(5)
        raise
    return app
