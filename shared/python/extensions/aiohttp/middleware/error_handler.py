import logging

from aiohttp.web import middleware, Request, HTTPException, json_response
from typing import Any, Awaitable, Callable

from aiohttp.web_response import StreamResponse


LOG = logging.getLogger(__name__)


@middleware
async def error_handler(
    request: Request,
    handler: Callable[[Request], Awaitable[StreamResponse]]
):
    """
    This handler catches exceptions and if they are HTTP exceptions it will 
    generate an appropriate json response to use as body data, if not it will 
    generate a 500 and return some information about the error.
    """
    try:
        response = await handler(request)
    except HTTPException as error:
        headers = error.headers
        del headers["Content-Type"]
        response = json_response(
            data={
                "status": error.status,
                "reason": error.reason
            },
            status=error.status,
            headers=headers
        )
    except Exception as error:
        response = json_response(
            data={
                "status": "Internal Server Error",
                "reason": 500,
                "message": str(error)
            },
            status=500
        )
        LOG.exception(error)

    return response
