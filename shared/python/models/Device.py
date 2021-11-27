from datetime import datetime
from typing import Optional
from pydantic import BaseModel, IPvAnyAddress

from shared.python.models.Location import Location


class Device(BaseModel):
    id: int
    mac: str
    ip: IPvAnyAddress
    websocket_path: str
    location_id: Location.id
    last_message: Optional[datetime] = None
