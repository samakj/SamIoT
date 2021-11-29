from typing import Dict, List, Optional
from datetime import datetime

from shared.python.models.Device import Device
from shared.python.stores.BaseStore import BaseStore


class DevicesStore(BaseStore):
    async def create_device(
        self,
        mac: str,
        ip: str,
        websocket_path: str,
        location_id: str,
        last_message: Optional[datetime] = None,
    ) -> Optional[Device]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        INSERT INTO devices (mac, ip, websocket_path, location_id, last_message)
                        VALUES ($1, $2, $3, $4, $5)
                        RETURNING id, mac, ip, websocket_path, location_id, last_message
                    """,
                    mac,
                    ip,
                    websocket_path,
                    location_id,
                    last_message
                )

                result = (
                    Device.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

                if result is not None:
                    await self.broadcast("CREATE", result, result.id)

                return result

    async def get_device(self, id: int) -> Optional[Device]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        SELECT id, mac, ip, websocket_path, location_id, last_message
                        FROM devices
                        WHERE id=$1
                    """,
                    id,
                )

                return (
                    Device.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

    async def get_device_by_ip(self, ip: str) -> Optional[Device]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        SELECT id, mac, ip, websocket_path, location_id, last_message
                        FROM devices
                        WHERE ip=$1
                    """,
                    ip,
                )

                return (
                    Device.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

    async def get_device_by_abbreviation(self, mac: str) -> Optional[Device]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        SELECT id, mac, ip, websocket_path, location_id, last_message
                        FROM devices
                        WHERE mac=$1
                    """,
                    mac,
                )

                return (
                    Device.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

    async def get_devices(
        self,
        ids: Optional[List[int]] = None,
        macs: Optional[List[str]] = None,
        ips: Optional[List[str]] = None,
        location_ids: Optional[List[int]] = None,
        last_message_gte: Optional[datetime] = None,
        last_message_lte: Optional[datetime] = None
    ) -> List[Device]:
        filters = []

        if ids is not None:
            filters.append("id=ANY($1)")
        if macs is not None:
            filters.append("macs=ANY($2)")
        if ips is not None:
            filters.append("ips=ANY($3)")
        if location_ids is not None:
            filters.append("location_ids=ANY($4)")
        if last_message_gte or last_message_lte:
            filters = [f"({' OR '.join(filters)})"]
        if last_message_gte:
            filters.append("last_message>$5")
        if last_message_lte:
            filters.append("last_message>$6")
        if not filters:
            filters = ["TRUE"]

        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        SELECT id, mac, ip, websocket_path, location_id, last_message
                        FROM devices
                        WHERE {" AND ".join(filters)}
                    """,
                    ids,
                    macs,
                    ips,
                    location_ids,
                    last_message_gte,
                    last_message_lte,
                )

                return [Device.parse_obj(dict(row)) for row in db_response]

    async def update_device(
        self,
        id: int,
        mac: Optional[str] = None,
        ip: Optional[str] = None,
        websocket_path: Optional[str] = None,
        location_id: Optional[int] = None,
        last_message: Optional[datetime] = None
    ) -> Optional[Device]:
        updates = []

        if mac is not None:
            updates.append("mac=$2")
        if ip is not None:
            updates.append("ip=$3")
        if websocket_path is not None:
            updates.append("websocket_path=$4")
        if location_id is not None:
            updates.append("location_id=$5")
        if last_message is not None:
            updates.append("last_message=$6")
        if not updates:
            return await self.get_device(id)

        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetch(
                    f"""
                        UPDATE devices
                        SET {", ".join(updates)}
                        WHERE id=$1
                        RETURNING id, mac, ip, websocket_path, location_id, last_message
                    """,
                    id,
                    mac,
                    ip,
                    websocket_path,
                    location_id,
                    last_message,
                )

                result = (
                    Device.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

                if result is not None:
                    await self.broadcast("UPDATE", result, id)

                return result

    async def delete_device(
        self,
        id: int,
    ) -> Optional[Dict[str, int]]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        DELETE FROM devices
                        WHERE id=$1
                        RETURN TRUE
                    """,
                    id
                )

                result = (
                    {"id": id}
                    if db_response else
                    None
                )

                if result is not None:
                    await self.broadcast("DELETE", result, id)

                return result
