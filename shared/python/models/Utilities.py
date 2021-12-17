from datetime import datetime
from decimal import Decimal
from pydantic import BaseModel


from shared.python.models.OctopusEnergy import OctopusConsumption


class UtilitiesConsumption(BaseModel):
    timestamp: datetime
    consumption: Decimal

    @staticmethod
    def from_octopus_consumption(
        octopus_consumption: OctopusConsumption
    ) -> 'BaseModel':
        return BaseModel(
            timestamp=octopus_consumption.interval_start,
            consumption=octopus_consumption.consumption
        )
