from typing import Any, Dict, List, Optional
from datetime import datetime

from shared.python.models.Weather import CurrentWeather
from shared.python.stores.BaseStore import BaseStore


class CurrentStore(BaseStore):
    async def upsert_current_weather(
        self,
        current: CurrentWeather
    ) -> Optional[CurrentWeather]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow("DELETE FROM current")
                db_response = await connection.fetchrow(
                    f"""
                        INSERT INTO current (
                            timestamp,
                            sunrise,
                            sunset,
                            temperature,
                            apparent_temperature,
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
                            $17
                        )
                        RETURNING *
                    """,
                    current.timestamp,
                    current.sunrise,
                    current.sunset,
                    current.temperature,
                    current.apparent_temperature,
                    current.pressure,
                    current.humidity,
                    current.dew_point,
                    current.cloud_coverage,
                    current.visibility,
                    current.wind_speed,
                    current.wind_direction,
                    current.owm_weather_id,
                    current.owm_weather_title,
                    current.owm_weather_description,
                    current.wind_gust,
                    current.uv_index
                )

                result = (
                    CurrentWeather.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )

                if result is not None:
                    await self.broadcast("UPDATE", result, result.id)

                return result

    async def get_current_weather(self) -> Optional[CurrentWeather]:
        async with self.db.acquire() as connection:
            async with connection.transaction():
                db_response = await connection.fetchrow("SELECT * FROM current")

                return (
                    CurrentWeather.parse_obj(dict(db_response))
                    if db_response is not None else
                    None
                )
