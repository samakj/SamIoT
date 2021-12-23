import asyncio
import math
from typing import Any, Dict, List, Optional
from datetime import datetime, timedelta, timezone
from decimal import Decimal

from aiohttp.web_exceptions import HTTPBadRequest

from shared.python.models.Measurement import Measurement, ValueType, ValueTypeOptions
from shared.python.stores.BaseStore import BaseStore


class MeasurementsStore(BaseStore):
    async def create_measurement(
        self,
        timestamp: datetime,
        device_id: int,
        location_id: int,
        metric_id: int,
        tags: List[str],
        value: ValueType,
    ) -> Optional[Measurement]:
        value_type: ValueTypeOptions = "string"

        if isinstance(value, int):
            value_type = "integer"
        if isinstance(value, Decimal):
            value_type = "float"
        if isinstance(value, bool):
            value_type = "boolean"

        if timestamp.tzinfo is not None and timestamp.tzinfo != timezone.utc:
            raise HTTPBadRequest(reason="None UTC date timezone detected")

        timestamp = timestamp.replace(tzinfo=None)

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
                    value_type
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
                db_response = await connection.fetch(
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
                        value_type = _row["value_type"]
                        _row["value"] = _row[f"{value_type}_value"]

                        if value is not None and _row["value"] != value:
                            continue
                        if value_gte is not None and _row["value"] < value_gte:
                            continue
                        if value_lte is not None and _row["value"] > value_lte:
                            continue

                        rows.append(_row)

                return [Measurement.parse_obj(row) for row in rows]

    async def get_latest_measurements(
        self,
        device_ids: Optional[List[int]] = None,
        location_ids: Optional[List[int]] = None,
        metric_ids: Optional[List[int]] = None,
        tags: Optional[List[str]] = None,
    ) -> List[Measurement]:
        values = []
        filters = []

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
        if not filters:
            filters = ["TRUE"]

        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetch(
                    f"""
                        SELECT DISTINCT ON (location_id, metric_id, tags)
                               measurements.id, 
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
                        ORDER BY location_id, metric_id, tags, timestamp DESC
                    """,
                    *values
                )

                rows = []

                if db_response is not None:
                    for row in db_response:
                        _row = dict(row)
                        value_type = _row["value_type"]
                        _row["value"] = _row[f"{value_type}_value"]
                        rows.append(_row)

                return [Measurement.parse_obj(row) for row in rows]

    async def get_time_weighted_average(
        self,
        location_id: int,
        metric_id: int,
        tags: List[str],
        start: datetime,
        end: datetime
    ) -> Dict[str, Any]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                response = await connection.fetchrow(
                    f"""
                    WITH args (_start, _end, _location_id, _metric_id, _tags) AS (
                        VALUES (
                            $1::TIMESTAMP,
                            $2::TIMESTAMP,
                            $3::INTEGER,
                            $4::INTEGER,
                            $5::TEXT[]
                        )
                    ), m AS (
                        SELECT _m.timestamp,
                               COALESCE(f.value, i.value::NUMERIC(8,4), b.value::INTEGER::NUMERIC(8,4)) as value
                          FROM ((
                                  SELECT id, timestamp 
                                    FROM measurements, args
                                  WHERE metric_id = args._metric_id AND
                                        location_id = args._location_id AND 
                                        tags @> args._tags AND
                                        timestamp > args._start AND
                                        timestamp < args._end
                              )
                              UNION ALL
                              (
                                  (
                                      SELECT id, args._start 
                                        FROM measurements, args
                                       WHERE metric_id = args._metric_id AND
                                             location_id = args._location_id AND 
                                             tags @> args._tags AND
                                             timestamp < args._start
                                       LIMIT 1
                                  )
                                  UNION ALL 
                                  (
                                      SELECT id, args._start 
                                        FROM measurements, args
                                       WHERE metric_id = args._metric_id AND
                                             location_id = args._location_id AND 
                                             tags @> args._tags AND
                                             timestamp > args._start AND
                                             timestamp < args._end
                                       LIMIT 1
                                  ) LIMIT 1
                              )
                              UNION ALL
                              (
                                  SELECT id, args._end 
                                    FROM measurements, args
                                   WHERE metric_id = args._metric_id AND
                                         location_id = args._location_id AND 
                                         tags @> args._tags AND
                                         timestamp < args._end
                                   LIMIT 1
                              )) AS _m
                    LEFT JOIN float_measurements AS f ON _m.id = f.measurement_id
                    LEFT JOIN integer_measurements AS i ON _m.id = i.measurement_id
                    LEFT JOIN boolean_measurements AS b ON _m.id = b.measurement_id
                     ORDER BY timestamp ASC 
                    ), l as (
                        SELECT timestamp, 
                               lead(timestamp, 1) OVER (ORDER BY timestamp) AS next_timestamp, 
                               value, 
                               lead(value, 1) OVER (ORDER BY timestamp) AS next_value 
                          FROM m
                    ), d as (
                        SELECT l.timestamp, 
                               l.next_timestamp, 
                               EXTRACT(epoch from (l.next_timestamp - l.timestamp)) AS dt, 
                               l.value, 
                               l.next_value, 
                               l.next_value - l.value AS dv 
                          FROM l
                    )
                          SELECT args._start as start, 
                                 args._end as end,
                                 args._metric_id as metric_id,
                                 args._location_id as location_id,
                                 args._tags as tags,
                                 sum(d.value * d.dt) / EXTRACT(epoch from (args._end - args._start)) as average,
                                 min(d.value) as min,
                                 max(d.value) as max
                            FROM d, args
                        GROUP BY args._start, args._end,args._metric_id, args._location_id, args._tags;
                    """,
                    start, end, location_id, metric_id, tags
                )

                return {
                    "start": response["start"] if response else start,
                    "end": response["end"] if response else end,
                    "metric_id": response["metric_id"] if response else metric_id,
                    "location_id": response["location_id"] if response else location_id,
                    "tags": response["tags"] if response else tags,
                    "average": round(Decimal(response.get("average")), 4) if response else None,
                    "min": Decimal(response.get("min")) if response else None,
                    "max": Decimal(response.get("max")) if response else None,
                }

    async def get_time_weighted_average_range(
        self,
        location_id: int,
        metric_id: int,
        tags: List[str],
        start: datetime,
        end: datetime,
        period: timedelta
    ) -> List[Dict[str, Any]]:
        period_count = math.ceil((end - start) / period)
        periods = []

        for i in range(period_count):
            _start = start + i * period
            _end = start + (i + 1) * period
            if _end > end:
                _end = end
            periods.append([_start, _end])

        return list(
            await asyncio.gather(*[
                self.get_time_weighted_average(
                    location_id,
                    metric_id,
                    tags,
                    _period[0],
                    _period[1]
                ) for _period in periods
            ])
        )
