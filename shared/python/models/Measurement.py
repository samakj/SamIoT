from datetime import datetime
from decimal import Decimal
from typing import List, Literal, Optional, Union
from pydantic import BaseModel

ValueTypeOptions = Literal["string", "float", "integer", "boolean"]
ValueType = Union[int, str, Decimal, bool, None]


class Measurement(BaseModel):
    id: int
    timestamp: datetime
    device_id: int
    location_id: int
    metric_id: int
    tags: List[str]
    value_type: ValueTypeOptions = "string"
    value: ValueType = None
