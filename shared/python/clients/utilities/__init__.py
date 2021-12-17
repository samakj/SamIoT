from shared.python.extensions.aiohttp.api_client import APIClient
from shared.python.clients.utilities.gas import GasClient
from shared.python.clients.utilities.electric import ElectricClient


class UtilitiesClient(APIClient):
    gas: GasClient
    electric: ElectricClient

    def __init__(self, host: str, port: int, ssl: bool = True) -> None:
        super().__init__(host, port, ssl=ssl)
        self.gas = GasClient(host, port, ssl=ssl)
        self.electric = ElectricClient(host, port, ssl=ssl)
