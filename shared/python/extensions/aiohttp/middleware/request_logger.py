import logging
import re
import uuid
from aiohttp.web import middleware, Request, Response
from datetime import datetime
from typing import Any, Callable, Coroutine, Dict, List

LOG = logging.getLogger(__name__)

RED = "\u001b[31m"
YELLOW = "\u001b[33m"
GREEN = "\u001b[32m"
BLUE = "\u001b[34m"
MAGENTA = "\u001b[35m"
CYAN = "\u001b[36m"
RESET = "\u001b[0m"

METHOD_COLOR_MAP = {
    "GET": GREEN,
    "PATCH": CYAN,
    "POST": BLUE,
    "DELETE": MAGENTA,
}


def time_diff_str(seconds: float) -> str:
    if seconds < 0.001:
        return f"{int(seconds * 1_000_000)}μs"
    if seconds < 0.01:
        return f"{seconds * 1_000:.2f}ms"
    if seconds < 1:
        return f"{int(seconds * 1_000)}ms"
    if seconds < 10:
        return f"{seconds:.2f}s"
    return f"{int(seconds)}s"


def form_key(group: Dict[str, Any]) -> str:
    return f"{group['method']} {group['regex']}"


def request_logger(grouped_paths: List[Dict[str, Any]]):
    grouped_values = {}

    @middleware
    async def _request_logger(
        request: Request,
        handler: Callable[[Request], Coroutine[Any, Any, Response]]
    ):
        """
        This handler logs http requests made to the server and displays response
        time and code.
        """
        request["id"] = uuid.uuid4().hex
        request["received"] = datetime.utcnow()

        LOG.debug(
            f"{request['id'][:8]}: " +
            f"{METHOD_COLOR_MAP.get(request.method,'')}{request.method}{RESET} " +
            f"{YELLOW}{request.path}{RESET}"
        )

        response = await handler(request)

        request["handled"] = datetime.utcnow()
        request["duration"] = request["handled"] - request["received"]

        seconds = request["duration"].total_seconds()

        group = None

        for _group in grouped_paths:
            if (
                _group["method"] == request.method and
                re.match(f"^{_group['regex']}$", request.path) and
                response.status < 400
            ):
                group = _group

        if group is not None:
            group_key = form_key(group)
            grouped_values[group_key] = grouped_values.get(
                group_key,
                {"start": datetime.utcnow(), "count": 0, "duration": 0}
            )
            grouped_values[group_key]["count"] += 1
            grouped_values[group_key]["duration"] += seconds

            since_start = (
                datetime.utcnow() - grouped_values[group_key]["start"]
            ).total_seconds()
            if since_start > 10:
                summary = f"x{grouped_values[group_key]['count']} "
                summary += f"{round(10 * grouped_values[group_key]['count'] / since_start) / 10}/s"
                LOG.info(
                    f"{request['id'][:8]}: " +
                    f"{METHOD_COLOR_MAP.get(group['method'],'')}{group['method']:<6}{RESET} " +
                    f"{YELLOW}{group['regex'][0:32]:<32}{RESET} " +
                    f"{summary:<15}" +
                    f"⏰ {time_diff_str(grouped_values[group_key]['duration'] / grouped_values[group_key]['count'])}"
                )
                grouped_values[group_key]["start"] = datetime.utcnow()
                grouped_values[group_key]["count"] = 0
                grouped_values[group_key]["duration"] = 0
        else:
            LOG.info(
                f"{request['id'][:8]}: " +
                f"{METHOD_COLOR_MAP.get(request.method,'')}{request.method:<6}{RESET} " +
                f"{YELLOW}{request.path[0:32]:<32}{RESET} " +
                f"{RED if response.status >= 400 else YELLOW if response.status >= 300 else GREEN}" +
                f"🖥  {response.status}{RESET}         " +
                f"⏰ {time_diff_str(seconds)}"
            )

        return response
    return _request_logger
