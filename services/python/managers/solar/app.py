import asyncio
import logging
import os
import re
from aiohttp.web_middlewares import normalize_path_middleware
from aiohttp_middlewares.cors import cors_middleware
from dotenv import load_dotenv
from aiohttp_pydantic import oas

from Application import SolarControllerApplication
from shared.python.extensions.aiohttp.middleware.error_handler import error_handler
from shared.python.extensions.aiohttp.middleware.request_logger import request_logger
from shared.python.extensions.aiohttp.routes.default import DEFAULT_ROUTES

load_dotenv()
logging.basicConfig(
    level=logging.INFO,
    format="[%(asctime)s] %(levelname)s: %(message)s"
)
LOG = logging.getLogger(__name__)


async def close_cache(app: SolarControllerApplication):
    if app.cache is not None:
        await app.cache.close()


# async def cleanup_sockets(app: ForagerApplication):
#     if app.device_websocket_store is not None:
#         await app.device_websocket_store.delete_websockets()


async def create_app() -> SolarControllerApplication:
    try:
        app = SolarControllerApplication(
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

        app.connect_iot_client()
        app.connect_measurements_listener()

        # await app.connect_cache(
        #     host=os.environ["SOLAR_CONTROLLER_CACHE_HOST"],
        #     port=os.environ["SOLAR_CONTROLLER_CACHE_PORT"],
        #     # name=os.environ["SOLAR_CONTROLLER_CACHE_NAME"],
        #     # username=os.environ["SOLAR_CONTROLLER_CACHE_USER"],
        #     # password=os.environ["SOLAR_CONTROLLER_CACHE_PASS"],
        # )

        app.add_routes(DEFAULT_ROUTES)

        oas.setup(app, title_spec="Solar Controller API", url_prefix="/docs")

        # app.on_shutdown.append(cleanup_sockets)
        app.on_shutdown.append(close_cache)

        await app.measurements_listener.start()
    except:
        await asyncio.sleep(5)
        raise
    return app
