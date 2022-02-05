import os
from listeners.measurements import MeasurementsListener

from shared.python.extensions.aiohttp import ApplicationWithCache
from shared.python.clients.iot import IoTClient


class SolarControllerApplication(ApplicationWithCache):
    iot_client: IoTClient
    measurements_listener: MeasurementsListener

    def connect_measurements_listener(self) -> None:
        self.measurements_listener = MeasurementsListener(self.iot_client)

    def connect_iot_client(self) -> None:
        self.iot_client = IoTClient(
            host=os.environ["IOT_API_HOST"],
            port=os.environ["IOT_API_PORT"],
            ssl=False
        )
