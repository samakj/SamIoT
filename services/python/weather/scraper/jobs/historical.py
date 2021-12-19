import asyncio
import logging
from datetime import datetime, timedelta, timezone
from decimal import Decimal
import os
from typing import Any, List, Optional
from asyncio import Task

from clients.OpenWeatherMapClient import OpenWeatherMapClient
from shared.python.models.OpenWeatherMap import OpenWeatherMapHourlyResponse
from shared.python.models.Weather import HourlyWeather
from shared.python.clients.weather import WeatherClient

LOG = logging.getLogger()


class HistoricalJob:
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
        period: timedelta = timedelta(minutes=5)
    ) -> None:
        self.open_weather_map_client = open_weather_map_client
        self.weather_client = weather_client
        self.last_run = datetime.utcnow()
        self.next_run = self.last_run + period
        self.period = period
        self.is_running = False
        self.task = None

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
            f"Scraped {len(hourly_weathers)} historical hourly weathers, from " +
            f"{hourly_weathers[0].timestamp} -> {hourly_weathers[-1].timestamp}"
        )

        await asyncio.gather(*[
            self.weather_client.hourly.upsert_hourly_weather(hourly_weather)
            for hourly_weather in hourly_weathers
        ])

    async def _historical_task(self) -> None:
        while True:
            while datetime.utcnow() < self.next_run:
                await asyncio.sleep(0.1)

            self.last_run = datetime.utcnow()
            self.next_run = self.last_run + self.period
            self.is_running = True

            response = await self.open_weather_map_client.get_historical(
                date=datetime.utcnow() - timedelta(days=1),
                lat=Decimal(os.environ["LOCATION_LATITUDE"]),
                lon=Decimal(os.environ["LOCATION_LONGITUDE"])
            )
            await self.handle_hourly_response(response.hourly)

            self.is_running = False

    async def create_historical_task(self) -> None:
        cancelled = False
        while self.task is None and not cancelled:
            self.task = asyncio.create_task(self._historical_task())
            try:
                await self.task
            except asyncio.CancelledError:
                cancelled = True
            except Exception as error:
                if error is not None:
                    LOG.exception(error)
                    LOG.error(
                        "Historical task failed with above error, restarting in 5s..."
                    )
                else:
                    LOG.warning(
                        "Historical task ended without exception, restarting in 5s..."
                    )
                await asyncio.sleep(5)
        self.task = None

    async def start(self) -> Task[Any]:
        LOG.info(f"Starting historical task.")
        return (
            self.task
            if self.task is not None else
            asyncio.create_task(self.create_historical_task())
        )

    async def stop(self) -> None:
        LOG.info(f"Stopping historical task.")

        if self.task is not None:
            try:
                self.task.cancel()
                await self.task
            except asyncio.CancelledError:
                pass
            except Exception as error:
                LOG.error(
                    f"Tried stopping historical task but got error:"
                )
                LOG.exception(error)
        self.task = None
