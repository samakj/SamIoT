import logging
from typing import Any, Callable, Coroutine, Dict, Iterable, Optional
from aiohttp.abc import AbstractView
from aiohttp.web import Response, HTTPNotFound
from aioredis import Redis

from shared.python.json import serialise_json, parse_json
from shared.python.extensions.aiohttp.responses.json import json_response

LOG = logging.getLogger()


class CachedValue:
    key: str
    generate_value: Callable[..., Coroutine[Any, Any, Any]]
    args: Iterable[Any]
    kwargs: Dict[str, Any]
    cache: Optional[Redis]
    expiry: int
    serialise_value: Callable[[Any], str]
    parse_value: Callable[[str], Any]

    def __init__(
        self,
        key: str,
        generate_value: Callable[..., Coroutine[Any, Any, Any]],
        args: Iterable[Any] = [],
        kwargs: Dict[str, Any] = {},
        cache: Optional[Redis] = None,
        expiry: int = 0,
        serialise_value: Callable[[Any], str] = serialise_json,
        parse_value: Callable[[str], Any] = parse_json,
    ) -> None:
        self.key = key
        self.generate_value = generate_value
        self.args = args
        self.kwargs = kwargs
        self.cache = cache
        self.expiry = expiry
        self.serialise_value = serialise_value
        self.parse_value = parse_value

    async def get(self) -> Any:
        value = None

        if self.cache is not None:
            cache_value = await self.cache.get(self.key)

            if cache_value is not None:
                value = self.parse_value(cache_value)
        else:
            LOG.warn("Cache not connected.")

        if value is None:
            value = await self.set()

        return value

    async def set(self, value: Optional[Any] = None) -> Any:
        value = value if value is not None else await self.generate_value(
            *self.args,
            **self.kwargs,
        )

        if self.cache is not None:
            if value is not None:
                await self.cache.set(
                    self.key,
                    self.serialise_value(value),
                    ex=self.expiry
                )
            else:
                await self.clear()

        return value

    async def clear(self) -> None:
        if self.cache is not None:
            self.cache.delete(self.key)


class CachedJSONResponse(CachedValue):
    def __init__(
        self,
        view: AbstractView,
        generate_value: Callable[..., Coroutine[Any, Any, Any]],
        args: Iterable[Any] = [],
        kwargs: Dict[str, Any] = {},
        expiry: int = 0,
        serialise_value: Callable[[Any], str] = serialise_json,
        parse_value: Callable[[str], Any] = parse_json,
        prefix: Optional[str] = None,
        suffix: Optional[str] = None,
    ) -> None:
        super().__init__(
            key=(
                (f"{prefix}:" if prefix is not None else "") +
                f"route:{str(view.request.url)}" +
                (f":{suffix}" if suffix is not None else "")
            ),
            cache=view.request.app.cache,
            generate_value=generate_value,
            args=args,
            kwargs=kwargs,
            expiry=expiry,
            serialise_value=serialise_value,
            parse_value=parse_value,
        )

    async def get_response(self, raise_for_none: bool = True) -> Response:
        value = await self.get()

        if value is None and raise_for_none:
            raise HTTPNotFound()

        return json_response(value)
