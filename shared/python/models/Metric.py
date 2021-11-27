from typing import Optional
from pydantic import BaseModel


class Metric(BaseModel):
    id: int
    name: str
    abbreviation: str
    unit: Optional[str] = None
