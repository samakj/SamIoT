import os
from aiohttp.web import Application
from listeners.measurements import MeasurementsListener

from stores.device_websockets import DeviceWebsocketsStore
from shared.python.clients.iot import IoTClient


class ForagerApplication(Application):
    iot_client: IoTClient
    measurements_listener: MeasurementsListener
    device_websocket_store: DeviceWebsocketsStore

    def connect_measurements_listener(self) -> None:
        self.measurements_listener = MeasurementsListener(
            self.device_websocket_store, self.iot_client
        )

    def connect_device_websocket_store(self) -> None:
        self.device_websocket_store = DeviceWebsocketsStore()

    def connect_iot_client(self) -> None:
        self.iot_client = IoTClient(
            host=os.environ["IOT_API_HOST"],
            port=os.environ["IOT_API_PORT"],
            ssl=False
        )
