from aiohttp.web import Response, json_response as _json_response
from dataclasses import asdict, is_dataclass
from datetime import date, datetime, timedelta
from decimal import Decimal
from enum import Enum
from pydantic import BaseModel
from typing import Any, Dict, Tuple

TYPES_TO_STRING = (Decimal, float, timedelta)


def to_json_serialisable(object: Any) -> Any:
    if isinstance(object, (int, str)):
        return object
    if isinstance(object, (tuple, list, set)):
        return [to_json_serialisable(item) for item in object]
    if isinstance(object, dict):
        return {
            key: to_json_serialisable(value)
            for key, value in object.items()
        }
    if is_dataclass(object):
        return to_json_serialisable(asdict(object))
    if isinstance(object, BaseModel):
        return to_json_serialisable(object.dict())
    if isinstance(object, Enum):
        return object.name
    if isinstance(object, (date, datetime)):
        return object.isoformat()
    if isinstance(object, TYPES_TO_STRING):
        return str(object)
    return object


def json_response(
    obj: Any, *args: Tuple[Any], **kwargs: Dict[str, Any]
) -> Response:
    return _json_response(to_json_serialisable(obj), *args, **kwargs)
