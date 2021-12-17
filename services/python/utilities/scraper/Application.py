import os
from typing import Optional

from aiohttp.web_app import Application

from shared.python.clients.utilities import UtilitiesClient
from clients.OctopusEnergyClient import OctopusEnergyClient
from jobs.gas import GasJob
from jobs.electric import ElectricJob


class UtilitiesScraperApplication(Application):
    octopus_energy_client: Optional[OctopusEnergyClient] = None
    utilities_client: Optional[UtilitiesClient] = None
    gas_job: Optional[GasJob] = None
    electric_job: Optional[ElectricJob] = None

    def connect_octopus_energy_client(self) -> None:
        self.octopus_energy_client = OctopusEnergyClient(
            os.environ["OCTOPUS_ENERGY_API_KEY"]
        )

    def connect_utilities_client(self) -> None:
        self.utilities_client = UtilitiesClient(
            host=os.environ["UTILITIES_API_HOST"],
            port=os.environ["UTILITIES_API_PORT"],
            ssl=False
        )

    def connect_gas_job(self) -> None:
        if self.octopus_energy_client is None:
            raise ValueError(
                "Open utilities map client was not initialised" +
                "before connecting gas job."
            )
        if self.utilities_client is None:
            raise ValueError(
                "Utilities client was not initialised" +
                "before connecting gas job."
            )

        self.gas_job = GasJob(
            self.octopus_energy_client,
            self.utilities_client
        )

    def connect_electric_job(self) -> None:
        if self.octopus_energy_client is None:
            raise ValueError(
                "Open utilities map client was not initialised" +
                "before connecting electric job."
            )
        if self.utilities_client is None:
            raise ValueError(
                "Utilities client was not initialised" +
                "before connecting electric job."
            )

        self.electric_job = ElectricJob(
            self.octopus_energy_client,
            self.utilities_client
        )
