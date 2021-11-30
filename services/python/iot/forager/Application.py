import os
from aiohttp.web import Application

from shared.python.clients.iot import IoTClient


class ForagerApplication(Application):
    iot_client: IoTClient

    def connect_iot_client(self) -> None:
        self.iot_client = IoTClient(
            host=os.environ["IOT_HOST"], port=os.environ["IOT_PORT"]
        )
