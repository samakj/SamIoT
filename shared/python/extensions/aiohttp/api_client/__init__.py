from typing import Any, Optional
from aiohttp import ClientSession, ClientResponse

from aiohttp.client_reqrep import ClientResponse
from shared.python.extensions.aiohttp.websocket import EventCallback, EventWithMessageCallback, Websocket

from shared.python.json import to_json_serialisable, serialise_json


class APIClient:
    host: str
    port: int
    ssl: bool
    http_base_url: str
    websocket_base_url: str

    def __init__(self, host: str, port: int, ssl: bool = True) -> None:
        self.host = host
        self.port = port
        self.ssl = ssl
        self.http_base_url = f"http{'s' if self.ssl else ''}://{self.host}:{self.port}"
        self.websocket_base_url = f"ws{'s' if self.ssl else ''}://{self.host}:{self.port}"

    async def request(
        self,
        method: str,
        endpoint: str,
        *args: Any,
        **kwargs: Any
    ) -> ClientResponse:
        url = f"{self.http_base_url.strip('/')}/{endpoint.strip('/')}"
        _kwargs = kwargs
        if kwargs.get("params") is not None:
            params = kwargs.get('params', {})
            _kwargs["params"] = {}
            for key in params.keys():
                if params[key] is not None:
                    _kwargs["params"][key] = to_json_serialisable(params[key])

        async with ClientSession(json_serialize=serialise_json) as session:
            async with session.__getattribute__(method.lower())(url, *args, **_kwargs) as response:
                response.raise_for_status()
                await response.read()
            return response

    async def get(self, endpoint: str, *args: Any, **kwargs: Any) -> ClientResponse:
        return await self.request("GET", endpoint, *args, **kwargs)

    async def post(self, endpoint: str, *args: Any, **kwargs: Any) -> ClientResponse:
        return await self.request("POST", endpoint, *args, **kwargs)

    async def patch(self, endpoint: str, *args: Any, **kwargs: Any) -> ClientResponse:
        return await self.request("PATCH", endpoint, *args, **kwargs)

    async def delete(self, endpoint: str, *args: Any, **kwargs: Any) -> ClientResponse:
        return await self.request("DELETE", endpoint, *args, **kwargs)

    async def listen(
        self,
        endpoint: str,
        id: str = "",
        name: str = "",
        auto_reconnect_time: int = 15,
        *,
        on_connect: Optional[EventCallback] = None,
        on_disconnect: Optional[EventCallback] = None,
        on_message: Optional[EventWithMessageCallback] = None,
        on_text: Optional[EventWithMessageCallback] = None,
        on_json: Optional[EventWithMessageCallback] = None,
        on_error: Optional[EventWithMessageCallback] = None,
        on_close: Optional[EventWithMessageCallback] = None
    ) -> Websocket:
        url = f"{self.websocket_base_url.strip('/')}/{endpoint.strip('/')}"

        manager = Websocket(
            url,
            id=id,
            name=name,
            auto_reconnect_time=auto_reconnect_time,
            on_message=on_message,
            on_connect=on_connect,
            on_disconnect=on_disconnect,
            on_text=on_text,
            on_json=on_json,
            on_error=on_error,
            on_close=on_close,
        )
        await manager.start()

        return manager
