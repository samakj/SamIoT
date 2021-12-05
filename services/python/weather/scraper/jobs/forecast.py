import asyncio
import logging
from datetime import datetime, timedelta
from decimal import Decimal
import os
from typing import Any, Optional
from clients.OpenWeatherMapClient import OpenWeatherMapClient
from asyncio import Task

LOG = logging.getLogger()


class ForecastJob:
    open_weather_map_client: OpenWeatherMapClient
    last_run: datetime
    next_run: datetime
    period: timedelta
    is_running: bool

    task: Optional[Task[Any]]

    def __init__(
        self,
        open_weather_map_client: OpenWeatherMapClient,
        period: timedelta = timedelta(minutes=1)
    ) -> None:
        self.open_weather_map_client = open_weather_map_client
        self.last_run = datetime.utcnow()
        self.next_run = self.last_run + period
        self.period = period
        self.is_running = False
        self.task = None

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
            print(
                f"{response.current.dt} Temperature is {response.current.temp}c but feels like {response.current.feels_like}c"
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
