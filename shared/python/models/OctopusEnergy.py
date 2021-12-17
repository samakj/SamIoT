from datetime import datetime
from decimal import Decimal
from pydantic import BaseModel
from typing import List, Optional


class OctopusConsumption(BaseModel):
    consumption: Decimal
    interval_end: datetime
    interval_start: datetime


class OctopusConsumptionResponse(BaseModel):
    count: int
    results: List[OctopusConsumption]
    next: Optional[str] = None
    prev: Optional[str] = None
