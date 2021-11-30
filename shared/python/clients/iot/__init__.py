from shared.python.extensions.aiohttp.api_client import APIClient
from shared.python.clients.iot.devices import DevicesClient
from shared.python.clients.iot.locations import LocationsClient
from shared.python.clients.iot.measurements import MeasurementsClient
from shared.python.clients.iot.metrics import MetricsClient


class IoTClient(APIClient):
    devices: DevicesClient
    locations: LocationsClient
    measurements: MeasurementsClient
    metrics: MetricsClient

    def __init__(self, host: str, port: int, ssl: bool = True) -> None:
        super().__init__(host, port, ssl=ssl)
        self.devices = DevicesClient(host, port, ssl=ssl)
        self.locations = LocationsClient(host, port, ssl=ssl)
        self.measurements = MeasurementsClient(host, port, ssl=ssl)
        self.metrics = MetricsClient(host, port, ssl=ssl)
