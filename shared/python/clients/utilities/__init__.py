from shared.python.extensions.aiohttp.api_client import APIClient
from shared.python.clients.utilities.gas import GasClient
from shared.python.clients.utilities.electricity import ElectricityClient


class UtilitiesClient(APIClient):
    gas: GasClient
    electricity: ElectricityClient

    def __init__(self, host: str, port: int, ssl: bool = True) -> None:
        super().__init__(host, port, ssl=ssl)
        self.gas = GasClient(host, port, ssl=ssl)
        self.electricity = ElectricityClient(host, port, ssl=ssl)
