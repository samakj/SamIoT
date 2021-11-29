import asyncio
from abc import ABCMeta
from aiohttp.web import RouteTableDef, Response, json_response, View
from aiohttp_cors import CorsViewMixin

DEFAULT_ROUTES = RouteTableDef()


@DEFAULT_ROUTES.view("/")
class RootView(View, CorsViewMixin):
    async def get(self) -> Response:
        routes = {}

        for route in self.request.app.router.routes():
            info = route.get_info()
            path = info.get("formatter", info.get("path"))

            if path is None or path == "/":
                continue

            pathSplit = path.split("/")
            version = pathSplit[1]
            resource = pathSplit[2]

            routes[version] = routes.get(version, {})
            routes[version][resource] = routes[version].get(resource, {})
            routes[version][resource][path] = routes[version][resource].get(
                path, []
            )

            if isinstance(route.handler, ABCMeta):
                for method in ["GET", "PATCH", "POST", "DELETE"]:
                    if hasattr(route.handler, method.lower()):
                        routes[version][resource][path].append(method)
            else:
                routes[version][resource][path].append(route.method)

        return json_response(routes)


@DEFAULT_ROUTES.view("/v0/ping")
class V0Ping(View, CorsViewMixin):
    async def get(self) -> Response:
        return json_response({"ping": "pong"})
