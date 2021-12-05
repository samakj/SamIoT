from decimal import Decimal
from typing import Any, Dict, List, Optional
from datetime import datetime

from shared.python.models.Weather import HourlyWeather
from shared.python.stores.BaseStore import BaseStore


class HourlyStore(BaseStore):
    async def upsert_hourly_weather(
        self,
        hourly: HourlyWeather
    ) -> Optional[HourlyWeather]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        INSERT INTO hourly (
                            timestamp,
                            temperature,
                            apparent_temperature,
                            pressure,
                            humidity,
                            dew_point,
                            cloud_coverage,
                            wind_speed,
                            wind_direction,
                            owm_weather_id,
                            owm_weather_title,
                            owm_weather_description,
                            chance_of_rain,
                            visibility,
                            wind_gust,
                            uv_index
                        )
                        VALUES (
                            $1, 
                            $2, 
                            $3, 
                            $4, 
                            $5, 
                            $6, 
                            $7, 
                            $8, 
                            $9, 
                            $10, 
                            $11, 
                            $12, 
                            $13, 
                            $14, 
                            $15, 
                            $16
                        )
                        ON CONFLICT (timestamp) DO
                            UPDATE
                                SET temperature=$2, 
                                SET apparent_temperature=$3, 
                                SET pressure=$4, 
                                SET humidity=$5, 
                                SET dew_point=$6, 
                                SET cloud_coverage=$7, 
                                SET wind_speed=$8, 
                                SET wind_direction=$9, 
                                SET owm_weather_id=$10, 
                                SET owm_weather_title=$11, 
                                SET owm_weather_description=$12, 
                                SET chance_of_rain=$13, 
                                SET visibility=$14, 
                                SET wind_gust=$15, 
                                SET uv_index=$16
                        RETURNING *
                    """,
                    hourly.timestamp,
                    hourly.temperature,
                    hourly.apparent_temperature,
                    hourly.pressure,
                    hourly.humidity,
                    hourly.dew_point,
                    hourly.cloud_coverage,
                    hourly.wind_speed,
                    hourly.wind_direction,
                    hourly.owm_weather_id,
                    hourly.owm_weather_title,
                    hourly.owm_weather_description,
                    hourly.chance_of_rain,
                    hourly.visibility,
                    hourly.wind_gust,
                    hourly.uv_index
                )

                result = (
                    HourlyWeather.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

                if result is not None:
                    await self.broadcast("UPDATE", result, result.id)

                return result

    async def get_hourly_weather(self, id: int) -> Optional[HourlyWeather]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    """
                        SELECT * 
                        FROM hourly
                        WHERE id=$1
                    """,
                    id
                )

                return (
                    HourlyWeather.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

    async def get_hourly_weather_by_timestamp(
        self, timestamp: datetime
    ) -> Optional[HourlyWeather]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    """
                        SELECT * 
                        FROM hourly
                        WHERE timestamp=$1
                    """,
                    timestamp
                )

                return (
                    HourlyWeather.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

    async def get_hourly_weather_by_nearest_timestamp(
        self, timestamp: datetime
    ) -> Optional[HourlyWeather]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    """
                        SELECT * 
                        FROM (
                            SELECT * 
                            FROM hourly
                            WHERE timestamp<=$1
                            UNION
                            SELECT * 
                            FROM hourly
                            WHERE timestamp>=$1
                        )
                        ORDER BY ABS(extract(epoch from timestamp) - extract(epoch from $1))
                        LIMIT 1
                    """,
                    timestamp
                )

                return (
                    HourlyWeather.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

    async def get_hourly_weathers(
        self,
        ids: Optional[List[int]] = None,
        timestamps: Optional[List[datetime]] = None,
        owm_weather_ids: Optional[List[int]] = None,
        owm_weather_titles: Optional[List[str]] = None,
        temperature_gte: Optional[Decimal] = None,
        temperature_lte: Optional[Decimal] = None,
        apparent_temperature_gte: Optional[Decimal] = None,
        apparent_temperature_lte: Optional[Decimal] = None,
        timestamp_gte: Optional[datetime] = None,
        timestamp_lte: Optional[datetime] = None,
    ) -> List[HourlyWeather]:
        values = []
        filters = []

        if ids is not None:
            filters.append(f"id=ANY(${len(values) + 1})")
            values.append(ids)
        if timestamps is not None:
            filters.append(f"timestamp=ANY(${len(values) + 1})")
            values.append(timestamps)
        if owm_weather_ids is not None:
            filters.append(f"owm_weather_id=ANY(${len(values) + 1})")
            values.append(owm_weather_ids)
        if owm_weather_titles is not None:
            filters.append(f"owm_weather_title=ANY(${len(values) + 1})")
            values.append(owm_weather_titles)
        if filters:
            filters = [f"({' OR '.join(filters)})"]
        if temperature_gte:
            filters.append(f"temperature>${len(values) + 1}")
            values.append(temperature_gte)
        if temperature_lte:
            filters.append(f"temperature>${len(values) + 1}")
            values.append(temperature_lte)
        if apparent_temperature_gte:
            filters.append(f"apparent_temperature>${len(values) + 1}")
            values.append(apparent_temperature_gte)
        if apparent_temperature_lte:
            filters.append(f"apparent_temperature>${len(values) + 1}")
            values.append(apparent_temperature_lte)
        if timestamp_gte:
            filters.append(f"timestamp>${len(values) + 1}")
            values.append(timestamp_gte)
        if timestamp_lte:
            filters.append(f"timestamp>${len(values) + 1}")
            values.append(timestamp_lte)
        if not filters:
            filters = ["TRUE"]

        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetch(
                    f"""
                        SELECT *
                        FROM hourly
                        WHERE {" AND ".join(filters)}
                    """,
                    *values
                )

                return [HourlyWeather.parse_obj(dict(row)) for row in db_response]
