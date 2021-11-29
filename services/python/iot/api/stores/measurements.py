from typing import List, Optional
from datetime import datetime
from decimal import Decimal

from shared.python.models.Measurement import Measurement, ValueType, ValueTypeOptions
from shared.python.stores.BaseStore import BaseStore


class MeasurementsStore(BaseStore):
    async def create_measurement(
        self,
        timestamp: datetime,
        device_id: str,
        location_id: str,
        metric_id: str,
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
                        SELECT id, 
                              timestamp,
                              device_id, 
                              location_id, 
                              metric_id, 
                              tags, 
                              value_type, 
                              float_measurements.value as float_value
                              integer_measurements.value as integer_value,
                              string_measurements.value as string_value,
                              boolean_measurements.value as boolean_value
                        FROM measurements
                        LEFT JOIN float_measurements ON measurements.id = float_measurements.measurement_id
                        LEFT JOIN integer_measurements ON measurements.id = integer_measurements.measurement_id
                        LEFT JOIN string_measurements ON measurements.id = string_measurements.measurement_id
                        LEFT JOIN boolean_measurements ON measurements.id = boolean_measurements.measurement_id
                        WHERE id=$1
                    """,
                    id,
                )

                d = dict(db_response)
                value_type = d["value_type"]
                d["value"] = d[f"{value_type}_value"]

                return (
                    Measurement.parse_obj(d)
                    if db_response is not None else
                    None
                )

    async def get_measurements(
        self,
        ids: Optional[List[str]] = None,
        device_ids: Optional[List[str]] = None,
        location_ids: Optional[List[str]] = None,
        metric_ids: Optional[List[str]] = None,
        tags: Optional[List[str]] = None,
        timestamp_gte: Optional[datetime] = None,
        timestamp_lte: Optional[datetime] = None,
        value: Optional[ValueType] = None,
        value_gte: Optional[ValueType] = None,
        value_lte: Optional[ValueType] = None,
    ) -> List[Measurement]:
        filters = []

        if ids is not None:
            filters.append("id=ANY($1)")
        if device_ids is not None:
            filters.append("device_id=ANY($2)")
        if location_ids is not None:
            filters.append("location_id=ANY($3)")
        if metric_ids is not None:
            filters.append("metric_id=ANY($4)")
        if tags is not None:
            filters.append("tags&&$5")
        if timestamp_gte is not None:
            filters.append("timestamp>=$6")
        if timestamp_lte is not None:
            filters.append("timestamp<=$7")

        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        SELECT id, 
                              timestamp,
                              device_id, 
                              location_id, 
                              metric_id, 
                              tags, 
                              value_type, 
                              float_measurements.value as float_value
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
                    ids,
                    device_ids,
                    location_ids,
                    metric_ids,
                    tags,
                    timestamp_gte,
                    timestamp_lte,
                )

                rows = []

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
