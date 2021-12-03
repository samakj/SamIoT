import asyncio
import logging
from dotenv import load_dotenv
from aiohttp_pydantic import oas

from Application import ForagerApplication
from rotues.listener import LISTENERS_V0_ROUTES
from rotues.websockets import WEBSOCKETS_V0_ROUTES
from shared.python.extensions.aiohttp.middleware.error_handler import error_handler
from shared.python.extensions.aiohttp.middleware.request_logger import request_logger
from shared.python.extensions.aiohttp.routes.default import DEFAULT_ROUTES

load_dotenv()
logging.basicConfig(
    level=logging.INFO,
    format="[%(asctime)s] %(levelname)s: %(message)s"
)
LOG = logging.getLogger(__name__)


async def cleanup_sockets(app: ForagerApplication):
    if app.device_websocket_store is not None:
        await app.device_websocket_store.delete_websockets()


async def create_app() -> ForagerApplication:
    try:
        app = ForagerApplication(
            middlewares=[
                request_logger(
                    [{"method": "PATCH", "regex": "/v0/devices/[\d]+"}]
                ),
                error_handler
            ]
        )

        app.connect_iot_client()
        app.connect_device_websocket_store()
        app.connect_measurements_listener()

        app.add_routes(DEFAULT_ROUTES)
        app.add_routes(LISTENERS_V0_ROUTES)
        app.add_routes(WEBSOCKETS_V0_ROUTES)

        oas.setup(app, title_spec="IoT API", url_prefix="/docs")

        await app.measurements_listener.watch_all()

        app.on_shutdown.append(cleanup_sockets)
    except:
        await asyncio.sleep(5)
        raise
    return app
