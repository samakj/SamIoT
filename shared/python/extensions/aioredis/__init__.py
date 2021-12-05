import logging
from typing import Any, Callable, Coroutine, Dict, Iterable, Optional
from aiohttp.abc import AbstractView
from aioredis import Redis

from shared.python.json import serialise_json, parse_json

LOG = logging.getLogger()


async def try_cache(
    key: str,
    get_value: Callable[..., Coroutine[Any, Any, Any]],
    args: Iterable[Any] = [],
    kwargs: Dict[str, Any] = {},
    cache: Optional[Redis] = None,
    expiry: int = 0,
    serialise_value: Callable[[Any], str] = serialise_json,
    parse_value: Callable[[str], Any] = parse_json,
) -> Any:
    value = None

    if cache is not None:
        cache_value = await cache.get(key)

        if cache_value is not None:
            value = parse_value(cache_value)
    else:
        LOG.warn("Cache not connected.")

    if value is None:
        value = await get_value(*args, **kwargs)

        if cache is not None and value is not None:
            await cache.set(key, serialise_value(value), ex=expiry)

    return value


async def try_route_cache(
    view: AbstractView,
    get_value: Callable[..., Coroutine[Any, Any, Any]],
    prefix: Optional[str] = None,
    args: Iterable[Any] = [],
    kwargs: Dict[str, Any] = {},
    expiry: int = 0,
    serialise_value: Callable[[Any], str] = serialise_json,
    parse_value: Callable[[str], Any] = parse_json,
) -> Any:
    _prefix = f"{prefix.strip(':')}:" if prefix else ""
    return await try_cache(
        key=f"{_prefix}route:{str(view.request.url)}",
        cache=view.request.app.cache,
        get_value=get_value,
        args=args,
        kwargs=kwargs,
        expiry=expiry,
        serialise_value=serialise_value,
        parse_value=parse_value,
    )
