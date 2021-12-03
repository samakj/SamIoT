import os
from aiohttp.web import Application

from stores.device_websockets import DeviceWebsocketsStore
from shared.python.clients.iot import IoTClient
from shared.python.queue import Queue


class ForagerApplication(Application):
    iot_client: IoTClient
    queue: Queue
    device_websocket_store: DeviceWebsocketsStore

    def connect_queue(self) -> None:
        self.queue = Queue()

    def connect_device_websocket_store(self) -> None:
        self.device_websocket_store = DeviceWebsocketsStore()

    def connect_iot_client(self) -> None:
        self.iot_client = IoTClient(
            host=os.environ["IOT_HOST"], port=os.environ["IOT_PORT"]
        )
