from datetime import datetime
from typing import Literal, Optional

from shared.python.models.OctopusEnergy import OctopusConsumptionResponse
from shared.python.extensions.aiohttp.api_client import APIClient

GroupByType = Literal["hour", "day", "week", "month", "quarter"]


class OctopusEnergyClient(APIClient):
    api_key: str

    def __init__(self, api_key: str) -> None:
        super().__init__(f"{api_key}:@api.octopus.energy/v1/", ssl=True)
        self.api_key = api_key

    async def get_gas_consumption(
        self,
        mprn: str,
        serial_number: str,
        period_from: Optional[datetime] = None,
        period_to: Optional[datetime] = None,
        page: Optional[int] = None,
        page_size: Optional[int] = None,
        order_by: Optional[int] = None,
        group_by: Optional[GroupByType] = None,
    ) -> OctopusConsumptionResponse:
        response = await self.get(
            f"/gas-meter-points/{mprn}/meters/{serial_number}/consumption/",
            params={
                "period_from": period_from,
                "period_to": period_to,
                "page": page,
                "page_size": page_size,
                "order_by": order_by,
                "group_by": group_by,
            }
        )
        return OctopusConsumptionResponse.parse_obj(
            await response.json()
        )

    async def get_electric_consumption(
        self,
        mpan: str,
        serial_number: str,
        period_from: Optional[datetime] = None,
        period_to: Optional[datetime] = None,
        page: Optional[int] = None,
        page_size: Optional[int] = None,
        order_by: Optional[int] = None,
        group_by: Optional[GroupByType] = None,
    ) -> OctopusConsumptionResponse:
        response = await self.get(
            f"/electricity-meter-points/{mpan}/meters/{serial_number}/consumption/",
            params={
                "period_from": period_from,
                "period_to": period_to,
                "page": page,
                "page_size": page_size,
                "order_by": order_by,
                "group_by": group_by,
            }
        )
        return OctopusConsumptionResponse.parse_obj(
            await response.json()
        )
