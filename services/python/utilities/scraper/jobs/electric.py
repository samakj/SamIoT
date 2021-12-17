import asyncio
import logging
from datetime import datetime, timedelta
import os
from typing import Any, List, Optional
from asyncio import Task

from clients.OctopusEnergyClient import OctopusEnergyClient
from shared.python.models.OctopusEnergy import OctopusConsumption
from shared.python.models.Utilities import UtilitiesConsumption
from shared.python.clients.utilities import UtilitiesClient

LOG = logging.getLogger()


class ElectricJob:
    octopus_energy_client: OctopusEnergyClient
    utilities_client: UtilitiesClient
    last_run: datetime
    next_run: datetime
    period: timedelta
    is_running: bool

    task: Optional[Task[Any]]

    def __init__(
        self,
        octopus_energy_client: OctopusEnergyClient,
        utilities_client: UtilitiesClient,
        period: timedelta = timedelta(minutes=1)
    ) -> None:
        self.octopus_energy_client = octopus_energy_client
        self.utilities_client = utilities_client
        self.last_run = datetime.utcnow()
        self.next_run = self.last_run + period
        self.period = period
        self.is_running = False
        self.task = None

    async def handle_consumptions(
        self, response: List[OctopusConsumption]
    ) -> None:
        for result in response:
            consumption = UtilitiesConsumption.from_octopus_consumption(result)
            consumption.timestamp = consumption.timestamp.replace(tzinfo=None)
            await self.utilities_client.electric.upsert_electric_consumption(consumption)

    async def _electric_task(self) -> None:
        while True:
            while datetime.utcnow() < self.next_run:
                await asyncio.sleep(0.1)

            self.last_run = datetime.utcnow()
            self.next_run = self.last_run + self.period
            self.is_running = True

            from_date = (
                datetime.utcnow().replace(hour=0, minute=0, second=0, microsecond=0) -
                timedelta(days=7)
            )
            response = await self.octopus_energy_client.get_electric_consumption(
                mpan=os.environ["ELECTRIC_METER_MPAN"],
                serial_number=os.environ["ELECTRIC_METER_SERIAL_NUMBER"],
                period_from=from_date,
                page_size=1000
            )
            LOG.info(
                f"Scraped {len(response.results)} electric readings, from {from_date} -> now"
            )
            await self.handle_consumptions(response.results)

            self.is_running = False

    async def create_electric_task(self) -> None:
        cancelled = False
        while self.task is None and not cancelled:
            self.task = asyncio.create_task(self._electric_task())
            try:
                await self.task
            except asyncio.CancelledError:
                cancelled = True
            except Exception as error:
                if error is not None:
                    LOG.exception(error)
                    LOG.error(
                        "Electric task failed with above error, restarting in 5s..."
                    )
                else:
                    LOG.warning(
                        "Electric task ended without exception, restarting in 5s..."
                    )
                await asyncio.sleep(5)
        self.task = None

    async def start(self) -> Task[Any]:
        LOG.info(f"Starting electric task.")
        return (
            self.task
            if self.task is not None else
            asyncio.create_task(self.create_electric_task())
        )

    async def stop(self) -> None:
        LOG.info(f"Stopping electric task.")

        if self.task is not None:
            try:
                self.task.cancel()
                await self.task
            except asyncio.CancelledError:
                pass
            except Exception as error:
                LOG.error(
                    f"Tried stopping electric task but got error:"
                )
                LOG.exception(error)
        self.task = None
