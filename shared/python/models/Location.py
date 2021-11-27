from typing import List
from pydantic import BaseModel


class Location(BaseModel):
    id: int
    name: str
    tags: List[str]
