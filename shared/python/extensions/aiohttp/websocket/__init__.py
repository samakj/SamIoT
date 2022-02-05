import asyncio
from datetime import datetime
import logging
from aiohttp import ClientSession, ClientWebSocketResponse, WSMessage, WSMsgType, ClientConnectionError, ClientConnectorError
from asyncio import Task
from typing import Any, Callable, Coroutine, Dict, Optional

from shared.python.json import serialise_json

LOG = logging.getLogger(__name__)

EventCallback = Callable[
    ["Websocket"],
    Coroutine[Any, Any, None]
]

EventWithMessageCallback = Callable[
    ["Websocket", WSMessage],
    Coroutine[Any, Any, None]
]


def is_json_string(string: str) -> bool:
    return (
        string[0] == "{" and string[-1] == "}"
    ) or (
        string[0] == "[" and string[-1] == "]"
    )


class Websocket:
    url: str
    id: str
    name: str
    last_message: Optional[datetime]
    auto_reconnect_time: int
    on_connect: Optional[EventCallback]
    on_disconnect: Optional[EventCallback]
    on_message: Optional[EventWithMessageCallback]
    on_text: Optional[EventWithMessageCallback]
    on_json: Optional[EventWithMessageCallback]
    on_error: Optional[EventWithMessageCallback]
    on_close: Optional[EventWithMessageCallback]

    websocket: Optional[ClientWebSocketResponse]
    task: Optional[Task]
    watchdog_task: Optional[Task]

    def __init__(
        self,
        url: str,
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
    ):
        self.url = url
        self.id = id
        self.name = name
        self.last_message = None
        self.auto_reconnect_time = auto_reconnect_time
        self.on_connect = on_connect
        self.on_disconnect = on_disconnect
        self.on_message = on_message
        self.on_text = on_text
        self.on_json = on_json
        self.on_error = on_error
        self.on_close = on_close

        self.websocket = None
        self.task = None
        self.watchdog_task = None

    async def serialise(self) -> Dict[str, Any]:
        return {
            "url": self.url,
            "id": self.id,
            "name": self.name,
            "last_message": self.last_message,
            "auto_reconnect_time": self.auto_reconnect_time,
            "websocket_open": self.websocket and not self.websocket.closed,
            "task_running": self.task and not self.task.cancelled(),
            "watchdog_running": self.watchdog_task and not self.watchdog_task.cancelled(),
            "websocket_open": self.websocket and not self.websocket.closed,
            "on_connect_callback": self.on_connect is not None,
            "on_disconnect_callback": self.on_disconnect is not None,
            "on_message_callback": self.on_message is not None,
            "on_text_callback": self.on_text is not None,
            "on_json_callback": self.on_json is not None,
            "on_error_callback": self.on_error is not None,
            "on_close_callback": self.on_close is not None,
        }

    async def _websocket_message_handler(self) -> None:
        if self.websocket is not None:
            async for message in self.websocket:
                self.last_message = datetime.utcnow()
                if self.on_message is not None:
                    await self.on_message(self, message)
                if message.type == WSMsgType.TEXT:
                    if self.on_json is not None and is_json_string(message.data):
                        await self.on_json(self, message)
                    elif self.on_text is not None:
                        await self.on_text(self, message)
                elif message.type == WSMsgType.CLOSE:
                    if self.on_close is not None:
                        await self.on_close(self, message)
                    break
                elif message.type == WSMsgType.ERROR:
                    if self.on_error is not None:
                        await self.on_error(self, message)
                    break

    async def _websocket_task(self) -> None:
        error = None
        async with ClientSession(json_serialize=serialise_json) as session:
            url = f"{self.url}{'&' if '?' in self.url else '?'}socket_id={self.id}"
            try:
                async with session.ws_connect(url) as websocket:
                    self.websocket = websocket

                    if self.on_connect is not None:
                        await self.on_connect(self)

                    await self._websocket_message_handler()
            except Exception as _error:
                if self.websocket is not None and not self.websocket.closed:
                    await self.websocket.close()
                self.websocket = None
                self.last_message = None
                if self.on_disconnect is not None:
                    await self.on_disconnect(self)
                error = _error

        if error is not None:
            raise error

    async def _watchdog_task(self) -> None:
        try:
            while True:
                if (
                    self.websocket and
                    self.last_message and
                    self.auto_reconnect_time > 0
                ):
                    dt = (datetime.utcnow() - self.last_message).total_seconds()
                    if dt > self.auto_reconnect_time:
                        LOG.error(
                            "No message from " +
                            (f"{self.name} " if self.name else "") +
                            f"websocket for {int(dt)}s restating in 5s"
                        )
                        await asyncio.sleep(5)
                        await self.restart()
                    elif int(dt) == self.auto_reconnect_time // 2:
                        LOG.warning(
                            "No message from " +
                            (f"{self.name} " if self.name else "") +
                            f"websocket for {int(dt)}s"
                        )
                await asyncio.sleep(0.5)
        except asyncio.CancelledError:
            pass

    async def _create_websocket(self) -> None:
        cancelled = False
        while self.task is None and not cancelled:
            self.task = asyncio.create_task(self._websocket_task())
            try:
                await self.task
            except asyncio.CancelledError:
                cancelled = True
            except Exception as error:
                if error is not None:
                    if isinstance(error, (ClientConnectorError, ClientConnectionError)):
                        LOG.error(
                            f"Failed to connect to " +
                            f"{self.name} " if self.name else "" +
                            "websocket at {self.url}, retrying in 5s..."
                        )
                    else:
                        LOG.exception(error)
                        LOG.error(
                            f"{f'{self.name} w' if self.name else 'W'}ebsocket " +
                            "closed with above exception, reconnecting in 5s..."
                        )
                else:
                    LOG.warning(
                        f"{f'{self.name} w' if self.name else 'W'}ebsocket " +
                        "closed without exception, reconnecting in 5s..."
                    )
                await asyncio.sleep(5)
            self.websocket = None
            self.task = None
            self.last_message = None

    async def _create_watchdog(self) -> None:
        cancelled = False
        while self.task is None and not cancelled:
            self.watchdog_task = asyncio.create_task(self._watchdog_task())
            try:
                await self.watchdog_task
            except asyncio.CancelledError:
                cancelled = True
            except Exception as error:
                if error is not None:
                    LOG.exception(error)
                    LOG.error(
                        f"{f'{self.name} w' if self.name else 'W'}atchdog " +
                        "closed with above exception, reconnecting in 5s..."
                    )
                else:
                    LOG.warning(
                        f"{f'{self.name} w' if self.name else 'W'}atchdog " +
                        "closed without exception, restarting in 5s..."
                    )
                await asyncio.sleep(5)
            self.watchdog_task = None

    async def start_websocket(self) -> Task:
        LOG.info(f"Starting {f'{self.name} ' if self.name else ''}websocket.")
        return (
            self.task
            if self.task is not None else
            asyncio.create_task(self._create_websocket())
        )

    async def start_watchdog(self) -> Task:
        LOG.info(f"Starting {f'{self.name} ' if self.name else ''}watchdog.")
        return (
            self.watchdog_task
            if self.watchdog_task is not None else
            asyncio.create_task(self._create_watchdog())
        )

    async def start(self) -> None:
        await self.start_websocket()

        if self.auto_reconnect_time > 0:
            await self.start_watchdog()

    async def stop_websocket(self) -> None:
        LOG.info(f"Stopping {f'{self.name} ' if self.name else ''}websocket.")

        if self.task is not None:
            try:
                self.task.cancel()
                await self.task
            except asyncio.CancelledError:
                pass
            except Exception as error:
                LOG.error(
                    f"Tried stopping {f'{self.name} ' if self.name else ''}" +
                    "websocket but got error:"
                )
                LOG.exception(error)

    async def stop_watchdog(self) -> None:
        LOG.info(f"Stopping {f'{self.name} ' if self.name else ''}watchdog.")

        if self.watchdog_task is not None:
            try:
                self.watchdog_task.cancel()
                await self.watchdog_task
            except asyncio.CancelledError:
                pass
            except Exception as error:
                LOG.error(
                    f"Tried stopping {f'{self.name} ' if self.name else ''}" +
                    "watchdog but got error:"
                )
                LOG.exception(error)

    async def stop(self) -> None:
        await self.stop_websocket()
        await self.stop_watchdog()

    async def restart(self) -> None:
        await self.stop_websocket()
        await self.start_websocket()
        await self.start_watchdog()
