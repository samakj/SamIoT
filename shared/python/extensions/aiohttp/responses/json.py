from aiohttp.web import Response, json_response as _json_response
from typing import Any, Dict, Tuple

from shared.python.json import to_json_serialisable


def json_response(
    obj: Any, *args: Tuple[Any], **kwargs: Dict[str, Any]
) -> Response:
    return _json_response(to_json_serialisable(obj), *args, **kwargs)
