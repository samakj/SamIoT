import asyncio
from aiohttp.web import WebSocketResponse
from typing import Any, List

from shared.python.json import serialise_json


class Queue:
    items: List[Any]
    sockets: List[WebSocketResponse]

    def __init__(self) -> None:
        self.items = []

    def __aiter__(self) -> "Queue":
        return self

    async def __anext__(self) -> Any:
        while len(self.items) == 0:
            await asyncio.sleep(0.1)

        item = self.items.pop(0)
        await self.broadcast({"length": self.size()})
        return item

    def size(self) -> int:
        return len(self.items)

    async def add(self, item: Any) -> None:
        self.items.append(item)
        await self.broadcast({"length": self.size()})

    def attach_socket(self, socket: WebSocketResponse) -> None:
        self.sockets.append(socket)

    async def broadcast(self, data: Any) -> None:
        json_data = serialise_json(data)
        dead_sockets: List[int] = []
        for index, socket in enumerate(self.sockets):
            if socket.closed:
                dead_sockets.append(index)
                continue
            await socket.send_str(json_data)

        for index in dead_sockets:
            await self.sockets[index].close()
