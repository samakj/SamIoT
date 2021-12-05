from decimal import Decimal
from datetime import datetime

from models.OpenWeatherMap import OpenWeatherMapCurrentAndForecastResponse, OpenWeatherMapHistoricResponse
from shared.python.extensions.aiohttp.api_client import APIClient


class OpenWeatherMapClient(APIClient):
    api_key: str

    def __init__(self, api_key: str) -> None:
        super().__init__("api.openweathermap.org/data/2.5/", ssl=True)
        self.api_key = api_key

    async def get_current_and_forecast(
        self, lat: Decimal, lon: Decimal
    ) -> OpenWeatherMapCurrentAndForecastResponse:
        response = await self.get(
            "/onecall",
            params={
                "lat": lat,
                "lon": lon,
                "units": "metric",
                "appid": self.api_key,
            }
        )
        return OpenWeatherMapCurrentAndForecastResponse.parse_obj(
            await response.json()
        )

    async def get_historic(
        self, lat: Decimal, lon: Decimal, date: datetime
    ) -> OpenWeatherMapHistoricResponse:
        response = await self.get(
            "/onecall/timemachine",
            params={
                "lat": lat,
                "lon": lon,
                "date": datetime.fromisoformat(date.isoformat()[:10]).timestamp(),
                "units": "metric",
                "appid": self.api_key,
            }
        )
        return OpenWeatherMapHistoricResponse.parse_obj(
            await response.json()
        )
