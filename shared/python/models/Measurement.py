from datetime import datetime
from decimal import Decimal
from typing import List, Literal, Optional, Union
from pydantic import BaseModel

from shared.python.models.Device import Device
from shared.python.models.Location import Location
from shared.python.models.Metric import Metric

ValueTypeOptions = Literal["string", "float", "integer", "boolean"]
ValueType = Union[int, str, Decimal, bool, None]


class Measurement(BaseModel):
    id: int
    timestamp: datetime
    device_id: Device.id
    location_id: Location.id
    metric_id: Metric.id
    tags: Optional[List[str]] = None
    value_type: ValueTypeOptions = "string"
    value: ValueType = None
