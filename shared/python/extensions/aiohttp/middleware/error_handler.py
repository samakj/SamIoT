import logging

from aiohttp.web import middleware, Request, Response, HTTPException, json_response
from typing import Any, Callable, Coroutine


LOG = logging.getLogger(__name__)


@middleware
async def error_handler(
    request: Request,
    handler: Callable[[Request], Coroutine[Any, Any, Response]]
):
    """
    This handler catches exceptions and if they are HTTP exceptions it will 
    generate an appropriate json response to use as body data, if not it will 
    generate a 500 and return some information about the error.
    """
    try:
        response = await handler(request)
    except HTTPException as error:
        response = json_response(
            data={
                "status": error.status,
                "reason": error.reason
            },
            status=error.status
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
