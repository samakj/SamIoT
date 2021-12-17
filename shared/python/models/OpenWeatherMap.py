from datetime import datetime
from decimal import Decimal
from pydantic import BaseModel
from typing import List, Optional


class OpenWeatherMapWeatherResponse(BaseModel):
    id: int
    main: str
    description: str
    icon: str


class OpenWeatherMapCurrentResponse(BaseModel):
    dt: datetime
    sunrise: datetime
    sunset: datetime
    temp: Decimal
    feels_like: Decimal
    pressure: int
    humidity: int
    dew_point: Decimal
    clouds: int
    visibility: int
    wind_speed: Decimal
    wind_deg: int
    weather: List[OpenWeatherMapWeatherResponse]
    wind_gust: Optional[Decimal] = None
    uvi: Optional[Decimal] = None


class OpenWeatherMapMinutelyResponse(BaseModel):
    dt: int
    precipitation: Decimal


class OpenWeatherMapHourlyResponse(BaseModel):
    dt: datetime
    temp: Decimal
    feels_like: Decimal
    pressure: int
    humidity: int
    dew_point: Decimal
    clouds: int
    wind_speed: Decimal
    wind_deg: int
    weather: List[OpenWeatherMapWeatherResponse]
    pop: Optional[Decimal] = None
    wind_gust: Optional[Decimal] = None
    uvi: Optional[Decimal] = None
    visibility: Optional[int] = None


class OpenWeatherMapDailyTempResponse(BaseModel):
    day: Decimal
    min: Decimal
    max: Decimal
    night: Decimal
    eve: Decimal
    morn: Decimal


class OpenWeatherMapDailyFeelsLikeResponse(BaseModel):
    day: Decimal
    night: Decimal
    eve: Decimal
    morn: Decimal


class OpenWeatherMapDailyResponse(BaseModel):
    dt: datetime
    sunrise: datetime
    sunset: datetime
    moonrise: datetime
    moonset: datetime
    moon_phase: Decimal
    temp: OpenWeatherMapDailyTempResponse
    feels_like: OpenWeatherMapDailyFeelsLikeResponse
    pressure: int
    humidity: int
    dew_point: Decimal
    uvi: Decimal
    pop: Decimal
    clouds: int
    wind_speed: Decimal
    wind_deg: int
    weather: List[OpenWeatherMapWeatherResponse]
    rain: Optional[Decimal] = None
    wind_gust: Optional[Decimal] = None


class OpenWeatherMapCurrentAndForecastResponse(BaseModel):
    lat: Decimal
    lon: Decimal
    timezone: str
    timezone_offset: int
    current: OpenWeatherMapCurrentResponse
    minutely: List[OpenWeatherMapMinutelyResponse]
    hourly: List[OpenWeatherMapHourlyResponse]
    daily: List[OpenWeatherMapDailyResponse]


class OpenWeatherMapHistoricResponse(BaseModel):
    lat: Decimal
    lon: Decimal
    timezone: str
    timezone_offset: int
    current: OpenWeatherMapCurrentResponse
    hourly: List[OpenWeatherMapHourlyResponse]
