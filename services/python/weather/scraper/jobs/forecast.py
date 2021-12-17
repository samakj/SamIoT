import asyncio
import logging
from datetime import datetime, timedelta, timezone
from decimal import Decimal
import os
from typing import Any, List, Optional
from asyncio import Task

from clients.OpenWeatherMapClient import OpenWeatherMapClient
from shared.python.models.OpenWeatherMap import OpenWeatherMapCurrentResponse, OpenWeatherMapDailyResponse, OpenWeatherMapHourlyResponse
from shared.python.models.Weather import CurrentWeather, DailyWeather, HourlyWeather
from shared.python.clients.weather import WeatherClient

LOG = logging.getLogger()


class ForecastJob:
    open_weather_map_client: OpenWeatherMapClient
    weather_client: WeatherClient
    last_run: datetime
    next_run: datetime
    period: timedelta
    is_running: bool

    task: Optional[Task[Any]]

    def __init__(
        self,
        open_weather_map_client: OpenWeatherMapClient,
        weather_client: WeatherClient,
        period: timedelta = timedelta(minutes=1)
    ) -> None:
        self.open_weather_map_client = open_weather_map_client
        self.weather_client = weather_client
        self.last_run = datetime.utcnow()
        self.next_run = self.last_run + period
        self.period = period
        self.is_running = False
        self.task = None

    async def handle_current_response(
        self, response: OpenWeatherMapCurrentResponse
    ) -> None:
        current_weather = CurrentWeather.from_owm_current_response(response)

        if current_weather.timestamp < datetime(year=2021, month=1, day=1, tzinfo=timezone.utc):
            current_weather.timestamp = datetime.fromtimestamp(
                current_weather.timestamp.timestamp() * 1000
            )

        current_weather.timestamp = current_weather.timestamp.replace(
            tzinfo=None
        )
        current_weather.sunrise = current_weather.sunrise.replace(
            tzinfo=None
        )
        current_weather.sunset = current_weather.sunset.replace(
            tzinfo=None
        )

        LOG.info(
            f"Scraped current weather: {current_weather.timestamp} -> " +
            f"{current_weather.owm_weather_title}, {current_weather.temperature}" +
            f"°c but feels like {current_weather.apparent_temperature}°c"
        )

        await self.weather_client.current.upsert_current_weather(current_weather)

    async def handle_hourly_response(
        self, response: List[OpenWeatherMapHourlyResponse]
    ) -> None:
        hourly_weathers = []

        for _response in response:
            hourly_weather = HourlyWeather.from_owm_hourly_response(_response)

            if hourly_weather.timestamp < datetime(year=2021, month=1, day=1, tzinfo=timezone.utc):
                hourly_weather.timestamp = datetime.fromtimestamp(
                    hourly_weather.timestamp.timestamp() * 1000
                )

            hourly_weather.timestamp = hourly_weather.timestamp.replace(
                tzinfo=None
            )

            hourly_weathers.append(hourly_weather)

        LOG.info(
            f"Scraped {len(hourly_weathers)} hourly weathers, from " +
            f"{hourly_weathers[0].timestamp} -> {hourly_weathers[-1].timestamp}"
        )

        await asyncio.gather(*[
            self.weather_client.hourly.upsert_hourly_weather(hourly_weather)
            for hourly_weather in hourly_weathers
        ])

    async def handle_daily_response(
        self, response: List[OpenWeatherMapDailyResponse]
    ) -> None:
        daily_weathers = []

        for _response in response:
            daily_weather = DailyWeather.from_owm_daily_response(_response)

            if daily_weather.timestamp < datetime(year=2021, month=1, day=1, tzinfo=timezone.utc):
                daily_weather.timestamp = datetime.fromtimestamp(
                    daily_weather.timestamp.timestamp() * 1000
                )

            daily_weather.timestamp = daily_weather.timestamp.replace(
                hour=12
            ).replace(tzinfo=None)
            daily_weather.sunrise = daily_weather.sunrise.replace(
                tzinfo=None
            )
            daily_weather.sunset = daily_weather.sunset.replace(
                tzinfo=None
            )
            daily_weather.moonrise = daily_weather.moonrise.replace(
                tzinfo=None
            )
            daily_weather.moonset = daily_weather.moonset.replace(
                tzinfo=None
            )

            daily_weathers.append(daily_weather)

        LOG.info(
            f"Scraped {len(daily_weathers)} daily weathers, from " +
            f"{daily_weathers[0].timestamp} -> {daily_weathers[-1].timestamp}"
        )

        await asyncio.gather(*[
            self.weather_client.daily.upsert_daily_weather(daily_weather)
            for daily_weather in daily_weathers
        ])

    async def _forecast_task(self) -> None:
        while True:
            while datetime.utcnow() < self.next_run:
                await asyncio.sleep(0.1)

            self.last_run = datetime.utcnow()
            self.next_run = self.last_run + self.period
            self.is_running = True

            response = await self.open_weather_map_client.get_current_and_forecast(
                lat=Decimal(os.environ["LOCATION_LATITUDE"]), lon=Decimal(os.environ["LOCATION_LONGITUDE"])
            )
            asyncio.gather(
                self.handle_current_response(response.current),
                self.handle_hourly_response(response.hourly),
                self.handle_daily_response(response.daily),
            )

            self.is_running = False

    async def create_forecast_task(self) -> None:
        cancelled = False
        while self.task is None and not cancelled:
            self.task = asyncio.create_task(self._forecast_task())
            try:
                await self.task
            except asyncio.CancelledError:
                cancelled = True
            except Exception as error:
                if error is not None:
                    LOG.exception(error)
                    LOG.error(
                        "Forecast task failed with above error, restarting in 5s..."
                    )
                else:
                    LOG.warning(
                        "Forecast task ended without exception, restarting in 5s..."
                    )
                await asyncio.sleep(5)
        self.task = None

    async def start(self) -> Task[Any]:
        LOG.info(f"Starting forecast task.")
        return (
            self.task
            if self.task is not None else
            asyncio.create_task(self.create_forecast_task())
        )

    async def stop(self) -> None:
        LOG.info(f"Stopping forecast task.")

        if self.task is not None:
            try:
                self.task.cancel()
                await self.task
            except asyncio.CancelledError:
                pass
            except Exception as error:
                LOG.error(
                    f"Tried stopping forecast task but got error:"
                )
                LOG.exception(error)
        self.task = None
