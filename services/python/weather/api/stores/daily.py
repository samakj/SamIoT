from decimal import Decimal
from typing import Any, Dict, List, Optional
from datetime import datetime

from shared.python.models.Weather import DailyWeather
from shared.python.stores.BaseStore import BaseStore


class DailyStore(BaseStore):
    async def upsert_daily_weather(
        self,
        daily: DailyWeather
    ) -> Optional[DailyWeather]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    f"""
                        INSERT INTO daily (timestamp
                            sunrise,
                            sunset,
                            moonrise,
                            moonset,
                            moon_phase,
                            temperature,
                            temperature_night,
                            temperature_evening,
                            temperature_morning,
                            temperature_min,
                            temperature_max,
                            apparent_temperature,
                            apparent_temperature_night,
                            apparent_temperature_evening,
                            apparent_temperature_morning,
                            pressure,
                            humidity,
                            dew_point,
                            cloud_coverage,
                            visibility,
                            wind_speed,
                            wind_direction,
                            owm_weather_id,
                            owm_weather_title,
                            owm_weather_description,
                            chance_of_rain,
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
                            $16,
                            $17,
                            $18,
                            $19,
                            $20,
                            $21,
                            $22,
                            $23,
                            $24,
                            $25,
                            $26,
                            $27,
                            $28,
                            $29
                        )
                        ON CONFLICT (timestamp) DO
                            UPDATE
                                SET timestamp=$1, 
                                SET sunrise=$2, 
                                SET sunset=$3, 
                                SET moonrise=$4, 
                                SET moonset=$5, 
                                SET moon_phase=$6, 
                                SET temperature=$7, 
                                SET temperature_night=$8, 
                                SET temperature_evening=$9, 
                                SET temperature_morning=$10, 
                                SET temperature_min=$11, 
                                SET temperature_max=$12, 
                                SET apparent_temperature=$13, 
                                SET apparent_temperature_night=$14, 
                                SET apparent_temperature_evening=$15, 
                                SET apparent_temperature_morning=$16,
                                SET pressure=$17,
                                SET humidity=$18,
                                SET dew_point=$19,
                                SET cloud_coverage=$20,
                                SET visibility=$21,
                                SET wind_speed=$22,
                                SET wind_direction=$23,
                                SET owm_weather_id=$24,
                                SET owm_weather_title=$25,
                                SET owm_weather_description=$26,
                                SET chance_of_rain=$27,
                                SET wind_gust=$28,
                                SET uv_index=$29,
                        RETURNING *
                    """,
                    daily.timestamp,
                    daily.temperature,
                    daily.apparent_temperature,
                    daily.pressure,
                    daily.humidity,
                    daily.dew_point,
                    daily.cloud_coverage,
                    daily.wind_speed,
                    daily.wind_direction,
                    daily.owm_weather_id,
                    daily.owm_weather_title,
                    daily.owm_weather_description,
                    daily.chance_of_rain,
                    daily.visibility,
                    daily.wind_gust,
                    daily.uv_index
                )

                result = (
                    DailyWeather.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

                if result is not None:
                    await self.broadcast("UPDATE", result, result.id)

                return result

    async def get_daily_weather(self, id: int) -> Optional[DailyWeather]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    """
                        SELECT * 
                        FROM daily
                        WHERE id=$1
                    """,
                    id
                )

                return (
                    DailyWeather.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

    async def get_daily_weather_by_timestamp(
        self, timestamp: datetime
    ) -> Optional[DailyWeather]:
        _timestamp = timestamp.replace(
            hour=12, minute=0, second=0, microsecond=0
        )
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow(
                    """
                        SELECT * 
                        FROM daily
                        WHERE timestamp=$1
                    """,
                    _timestamp
                )

                return (
                    DailyWeather.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

    async def get_daily_weathers(
        self,
        ids: Optional[List[int]] = None,
        timestamps: Optional[List[datetime]] = None,
        owm_weather_ids: Optional[List[int]] = None,
        owm_weather_titles: Optional[List[str]] = None,
        temperature_gte: Optional[Decimal] = None,
        temperature_lte: Optional[Decimal] = None,
        temperature_min_gte: Optional[Decimal] = None,
        temperature_min_lte: Optional[Decimal] = None,
        temperature_max_gte: Optional[Decimal] = None,
        temperature_max_lte: Optional[Decimal] = None,
        apparent_temperature_gte: Optional[Decimal] = None,
        apparent_temperature_lte: Optional[Decimal] = None,
        timestamp_gte: Optional[datetime] = None,
        timestamp_lte: Optional[datetime] = None,
    ) -> List[DailyWeather]:
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
        if temperature_min_gte:
            filters.append(f"temperature_min>${len(values) + 1}")
            values.append(temperature_min_gte)
        if temperature_min_lte:
            filters.append(f"temperature_min>${len(values) + 1}")
            values.append(temperature_min_lte)
        if temperature_max_gte:
            filters.append(f"temperature_max>${len(values) + 1}")
            values.append(temperature_max_gte)
        if temperature_max_lte:
            filters.append(f"temperature_max>${len(values) + 1}")
            values.append(temperature_max_lte)
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
                        FROM daily
                        WHERE {" AND ".join(filters)}
                    """,
                    *values
                )

                return [DailyWeather.parse_obj(dict(row)) for row in db_response]
