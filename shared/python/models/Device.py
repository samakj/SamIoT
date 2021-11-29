from datetime import datetime
from typing import Optional
from pydantic import BaseModel, IPvAnyAddress


class Device(BaseModel):
    id: int
    mac: str
    ip: IPvAnyAddress
    websocket_path: str
    location_id: int
    last_message: Optional[datetime] = None
