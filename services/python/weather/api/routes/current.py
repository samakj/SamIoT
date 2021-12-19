from aiohttp_pydantic import PydanticView
from aiohttp.web import RouteTableDef, Response

from shared.python.extensions.aiohttp.responses.json import json_response

from Application import WeatherAPIApplication
from shared.python.extensions.aioredis import CachedJSONResponse
from shared.python.models.Weather import CurrentWeather

CURRENT_V0_ROUTES = RouteTableDef()


@CURRENT_V0_ROUTES.view("/v0/current")
class CurrentV0View(PydanticView):
    async def get(self) -> Response:
        """
        Tags: Current
        """
        app: WeatherAPIApplication = self.request.app

        if not app.current_store:
            raise ValueError(
                "Current weather store not initialised before querying data."
            )

        cache = CachedJSONResponse(
            self,
            app.current_store.get_current_weather,
            expiry=15 * 60,
            prefix="weather:api"
        )

        return await cache.get_response()

    async def post(self, current: CurrentWeather) -> Response:
        """
        Tags: Current
        """
        app: WeatherAPIApplication = self.request.app

        if not app.current_store:
            raise ValueError(
                "Current weather store not initialised before querying data."
            )

        _current = await app.current_store.upsert_current_weather(current)

        if _current is not None and app.cache is not None:
            base_url = f"{'/'.join(str(self.request.url).split('/')[:3])}/v0/current"
            invalid_keys = set()
            invalid_keys.update(await app.cache.keys(f"weather:api:route:{base_url}"))
            invalid_keys.update(await app.cache.keys(f"weather:api:route:{base_url}?*"))
            if invalid_keys:
                await app.cache.delete(*invalid_keys)

        return json_response(current)
