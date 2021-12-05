import os
from typing import Optional

from aiohttp.web_app import Application

from clients.OpenWeatherMapClient import OpenWeatherMapClient
from jobs.forecast import ForecastJob


class WeatherScraperApplication(Application):
    open_weather_map_client: Optional[OpenWeatherMapClient] = None
    forecast_job: Optional[ForecastJob] = None

    def connect_open_weather_map_client(self) -> None:
        self.open_weather_map_client = OpenWeatherMapClient(
            os.environ["OWM_API_KEY"]
        )

    def connect_forecast_job(self) -> None:
        if self.open_weather_map_client is None:
            raise ValueError(
                "Open weather map client wal not initialised" +
                "before connecting forecast job."
            )

        self.forecast_job = ForecastJob(self.open_weather_map_client)
