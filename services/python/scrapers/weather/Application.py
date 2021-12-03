import os
from typing import Optional

from aiohttp.web_app import Application

from clients.OpenWeatherMapClient import OpenWeatherMapClient


class WeatherScraperApplication(Application):
    open_weather_map_client: Optional[OpenWeatherMapClient] = None

    def connect_open_weather_map_client(self) -> None:
        self.open_weather_map_client = OpenWeatherMapClient(
            os.environ["OWM_API_KEY"])
