import logging
from typing import Any, Dict, List, Literal, Optional, Tuple, Union
from aiohttp.http_websocket import WSMsgType
from asyncpg import Pool
from aiohttp.web import WebSocketResponse

from shared.python.json import serialise_json

LOG = logging.getLogger(__name__)
SocketMessageMethod = Literal["CREATE", "UPDATE", "DELETE"]


class BaseStore:
    db: Pool
    sockets: Dict[str, Dict[Union[str, int], WebSocketResponse]]

    def __init__(self, db: Pool) -> None:
        self.db = db
        self.sockets = {}

    async def broadcast(
        self,
        method: SocketMessageMethod,
        data: Any,
        key: str = "",
    ) -> None:
        resource_key = f"/{str(key).strip('/')}"
        json_data = serialise_json({"method": method, "data": data})

        dead_sockets: List[Tuple[Union[str, int], str]] = []
        for key in self.sockets.keys():
            if key in resource_key:
                for socket_id, socket in self.sockets[key].items():
                    if socket.closed:
                        dead_sockets.append((socket_id, key))
                        continue
                    await socket.send_str(json_data)

        for args in dead_sockets:
            await self.unlisten(args[0], args[1])

    async def listen(
        self,
        socket_id: Union[str, int],
        socket: WebSocketResponse,
        resource_id: Optional[Union[str, int]] = ''
    ) -> WebSocketResponse:
        resource_key = f"/{str(resource_id).strip('/')}"
        socket_key = socket_id

        self.sockets[resource_key] = self.sockets.get(resource_key, {})

        if socket_key in self.sockets[resource_key]:
            LOG.warn(
                f"Socket for {socket_key} already exists on {resource_key}, " +
                "closing."
            )
            await self.unlisten(socket_key, resource_id)

        self.sockets[resource_key][socket_key] = socket

        async for message in socket:
            if message.type == WSMsgType.ERROR:
                LOG.warn(
                    f"Socket for {socket_key} on {resource_key} errored," +
                    " closing."
                )
                await self.unlisten(socket_key, resource_id)
            elif message.type == WSMsgType.CLOSE:
                LOG.warn(
                    f"Socket for {socket_key} on {resource_key} closed by " +
                    "client, closing on here."
                )
                await self.unlisten(socket_key, resource_id)

        return socket

    async def unlisten(
        self,
        socket_id: Union[str, int],
        resource_id: Optional[Union[str, int]] = ''
    ) -> None:
        resource_key = f"/{str(resource_id).strip('/')}"
        socket_key = str(socket_id)

        if (
            self.sockets.get(resource_key) is not None and
            self.sockets[resource_key].get(socket_key) is not None
        ):
            await self.sockets[resource_key][socket_key].close()
            del self.sockets[resource_key][socket_key]

    async def unlisten_all(self) -> None:
        for resource in self.sockets:
            for socket in resource:
                await self.unlisten(socket, resource)
