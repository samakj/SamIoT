from datetime import datetime
from decimal import Decimal
from pydantic import BaseModel
from typing import List, Optional

from shared.python.models.OpenWeatherMap import OpenWeatherMapCurrentResponse, OpenWeatherMapDailyResponse, OpenWeatherMapHourlyResponse


class CurrentWeather(BaseModel):
    timestamp: datetime
    sunrise: datetime
    sunset: datetime
    temperature: Decimal
    apparent_temperature: Decimal
    pressure: int
    humidity: int
    dew_point: Decimal
    cloud_coverage: int
    visibility: int
    wind_speed: Decimal
    wind_direction: int
    owm_weather_id: int
    owm_weather_title: str
    owm_weather_description: str
    wind_gust: Optional[Decimal] = None
    uv_index: Optional[Decimal] = None

    def from_owm_current_response(
        self, response: OpenWeatherMapCurrentResponse
    ) -> 'CurrentWeather':
        return CurrentWeather(
            timestamp=response.dt,
            sunrise=response.sunrise,
            sunset=response.sunset,
            temperature=response.temp,
            apparent_temperature=response.feels_like,
            pressure=response.pressure,
            humidity=response.humidity,
            dew_point=response.dew_point,
            cloud_coverage=response.clouds,
            visibility=response.visibility,
            wind_speed=response.wind_speed,
            wind_direction=response.wind_deg,
            owm_weather_id=response.weather[0].id,
            owm_weather_title=response.weather[0].main,
            owm_weather_description=response.weather[0].description,
            wind_gust=response.wind_gust,
            uv_index=response.uvi,
        )


class HourlyWeather(BaseModel):
    timestamp: datetime
    temperature: Decimal
    apparent_temperature: Decimal
    pressure: int
    humidity: int
    dew_point: Decimal
    cloud_coverage: int
    wind_speed: Decimal
    wind_direction: int
    owm_weather_id: int
    owm_weather_title: str
    owm_weather_description: str
    chance_of_rain: Decimal
    visibility: Optional[int] = None
    wind_gust: Optional[Decimal] = None
    uv_index: Optional[Decimal] = None

    def from_owm_hourly_response(
        self, response: OpenWeatherMapHourlyResponse
    ) -> 'HourlyWeather':
        return HourlyWeather(
            timestamp=response.dt,
            temperature=response.temp,
            apparent_temperature=response.feels_like,
            pressure=response.pressure,
            humidity=response.humidity,
            dew_point=response.dew_point,
            cloud_coverage=response.clouds,
            visibility=response.visibility,
            wind_speed=response.wind_speed,
            wind_direction=response.wind_deg,
            owm_weather_id=response.weather[0].id,
            owm_weather_title=response.weather[0].main,
            owm_weather_description=response.weather[0].description,
            chance_of_rain=response.pop,
            wind_gust=response.wind_gust,
            uv_index=response.uvi,
        )


class DailyWeather(BaseModel):
    timestamp: datetime
    sunrise: datetime
    sunset: datetime
    moonrise: datetime
    moonset: datetime
    moon_phase: Decimal
    temperature: Decimal
    temperature_night: Decimal
    temperature_evening: Decimal
    temperature_morning: Decimal
    temperature_min: Decimal
    temperature_max: Decimal
    apparent_temperature: Decimal
    apparent_temperature_night: Decimal
    apparent_temperature_evening: Decimal
    apparent_temperature_morning: Decimal
    pressure: int
    humidity: int
    dew_point: Decimal
    cloud_coverage: int
    visibility: int
    wind_speed: Decimal
    wind_direction: int
    owm_weather_id: int
    owm_weather_title: str
    owm_weather_description: str
    chance_of_rain: Decimal
    wind_gust: Optional[Decimal] = None
    uv_index: Optional[Decimal] = None

    def from_owm_daily_response(
        self, response: OpenWeatherMapDailyResponse
    ) -> 'DailyWeather':
        return DailyWeather(
            timestamp=response.dt,
            sunrise=response.sunrise,
            sunset=response.sunset,
            moonrise=response.moonrise,
            moonset=response.moonset,
            moon_phase=response.moon_phase,
            temperature=response.temp.day,
            temperature_night=response.temp.night,
            temperature_evening=response.temp.eve,
            temperature_morning=response.temp.morn,
            temperature_min=response.temp.min,
            temperature_max=response.temp.max,
            apparent_temperature=response.temp.day,
            apparent_temperature_night=response.temp.night,
            apparent_temperature_evening=response.temp.eve,
            apparent_temperature_morning=response.temp.morn,
            pressure=response.pressure,
            humidity=response.humidity,
            dew_point=response.dew_point,
            cloud_coverage=response.clouds,
            chance_of_rain=response.pop,
            wind_speed=response.wind_speed,
            wind_direction=response.wind_deg,
            owm_weather_id=response.weather[0].id,
            owm_weather_title=response.weather[0].main,
            owm_weather_description=response.weather[0].description,
            wind_gust=response.wind_gust,
            uv_index=response.uvi,
        )
