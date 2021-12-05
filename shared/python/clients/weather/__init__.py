from shared.python.extensions.aiohttp.api_client import APIClient
from shared.python.clients.weather.daily import DailyClient
from shared.python.clients.weather.current import CurrentClient
from shared.python.clients.weather.hourly import HourlyClient


class WeatherClient(APIClient):
    current: CurrentClient
    daily: DailyClient
    hourly: HourlyClient

    def __init__(self, host: str, port: int, ssl: bool = True) -> None:
        super().__init__(host, port, ssl=ssl)
        self.current = CurrentClient(host, port, ssl=ssl)
        self.daily = DailyClient(host, port, ssl=ssl)
        self.hourly = HourlyClient(host, port, ssl=ssl)
