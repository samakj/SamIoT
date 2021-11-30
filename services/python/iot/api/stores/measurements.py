from typing import List, Optional
from datetime import datetime
from decimal import Decimal

from shared.python.models.Measurement import Measurement, ValueType, ValueTypeOptions
from shared.python.stores.BaseStore import BaseStore


class MeasurementsStore(BaseStore):
    async def create_measurement(
        self,
        timestamp: datetime,
        device_id: int,
        location_id: int,
        metric_id: int,
        tags: str,
        value: ValueType,
    ) -> Optional[Measurement]:
        value_type: ValueTypeOptions = "string"

        if isinstance(value, int):
            value_type = "integer"
        if isinstance(value, Decimal):
            value_type = "float"
        if isinstance(value, bool):
            value_type = "boolean"

        async with self.db.acquire() as connection:
            async with connection.transaction():
                measurement_response = await connection.fetchrow(
                    f"""
                        INSERT INTO measurements (
                            timestamp,
                            device_id,
                            location_id,
                            metric_id,
                            tags,
                            value_type
                        )
                        VALUES ($1, $2, $3, $4, $5, $6)
                        RETURNING id, timestamp, device_id, location_id, metric_id
                    """,
                    timestamp,
                    device_id,
                    location_id,
                    metric_id,
                    tags,
                )

                if value is not None:
                    await connection.fetchrow(
                        f"""
                            INSERT INTO {value_type}_measurements (
                                measurement_id,
                                value
                            )
                            VALUES ($1, $2)
                        """,
                        measurement_response["id"],
                        value
                    )

                result = await self.get_measurement(measurement_response["id"])

                if result is not None:
                    await self.broadcast("CREATE", result, result.id)

                return result

    async def get_measurement(self, id: int) -> Optional[Measurement]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        SELECT measurements.id, 
                               timestamp,
                               device_id, 
                               location_id, 
                               metric_id, 
                               tags, 
                               value_type, 
                               float_measurements.value as float_value,
                               integer_measurements.value as integer_value,
                               string_measurements.value as string_value,
                               boolean_measurements.value as boolean_value
                        FROM measurements
                        LEFT JOIN float_measurements ON measurements.id = float_measurements.measurement_id
                        LEFT JOIN integer_measurements ON measurements.id = integer_measurements.measurement_id
                        LEFT JOIN string_measurements ON measurements.id = string_measurements.measurement_id
                        LEFT JOIN boolean_measurements ON measurements.id = boolean_measurements.measurement_id
                        WHERE measurements.id=$1
                    """,
                    id,
                )

                if not db_response:
                    return None

                d = dict(db_response)
                value_type = d["value_type"]
                d["value"] = d[f"{value_type}_value"]

                return (
                    Measurement.parse_obj(d)
                    if d is not None else
                    None
                )

    async def get_measurements(
        self,
        ids: Optional[List[int]] = None,
        device_ids: Optional[List[int]] = None,
        location_ids: Optional[List[int]] = None,
        metric_ids: Optional[List[int]] = None,
        tags: Optional[List[str]] = None,
        timestamp_gte: Optional[datetime] = None,
        timestamp_lte: Optional[datetime] = None,
        value: Optional[ValueType] = None,
        value_gte: Optional[ValueType] = None,
        value_lte: Optional[ValueType] = None,
    ) -> List[Measurement]:
        values = []
        filters = []

        if ids is not None:
            filters.append(f"measurements.id=ANY(${len(values) + 1})")
            values.append(ids)
        if device_ids is not None:
            filters.append(f"device_id=ANY(${len(values) + 1})")
            values.append(device_ids)
        if location_ids is not None:
            filters.append(f"location_id=ANY(${len(values) + 1})")
            values.append(location_ids)
        if metric_ids is not None:
            filters.append(f"metric_id=ANY(${len(values) + 1})")
            values.append(metric_ids)
        if filters:
            filters = [f"({' OR '.join(filters)})"]
        if tags is not None:
            filters.append(f"tags@>${len(values) + 1}")
            values.append(tags)
        if timestamp_gte is not None:
            filters.append(f"timestamp>=${len(values) + 1}")
            values.append(timestamp_gte)
        if timestamp_lte is not None:
            filters.append(f"timestamp<=${len(values) + 1}")
            values.append(timestamp_lte)
        if not filters:
            filters = ["TRUE"]

        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        SELECT measurements.id, 
                               timestamp,
                               device_id, 
                               location_id, 
                               metric_id, 
                               tags, 
                               value_type, 
                               float_measurements.value as float_value,
                               integer_measurements.value as integer_value,
                               string_measurements.value as string_value,
                               boolean_measurements.value as boolean_value
                        FROM measurements
                        LEFT JOIN float_measurements ON measurements.id = float_measurements.measurement_id
                        LEFT JOIN integer_measurements ON measurements.id = integer_measurements.measurement_id
                        LEFT JOIN string_measurements ON measurements.id = string_measurements.measurement_id
                        LEFT JOIN boolean_measurements ON measurements.id = boolean_measurements.measurement_id
                        WHERE {" AND ".join(filters)}
                    """,
                    *values
                )

                rows = []

                if db_response is not None:
                    for row in db_response:
                        _row = dict(row)
                        value_type = row["value_type"]
                        row["value"] = row[f"{value_type}_value"]

                        if value is not None and _row["value"] != value:
                            continue
                        if value_gte is not None and _row["value"] < value_gte:
                            continue
                        if value_lte is not None and _row["value"] > value_lte:
                            continue

                        rows.append(_row)

                return [Measurement.parse_obj(row) for row in rows]
