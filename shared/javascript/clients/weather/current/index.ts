import { APIClient } from "../../../extensions/axios/api-client";
import {
  CurrentWeatherType,
  isCurrentWeatherType,
} from "../../../types/weather";
import { APICurrentWeatherType, isAPICurrentWeatherType } from "./types";

export class CurrentWeathersClient extends APIClient {
  constructor(host: string, port: number | string | null, ssl: boolean) {
    super(host, port, ssl);
  }

  static toAPICurrentWeatherType(
    weather: CurrentWeatherType | APICurrentWeatherType
  ): APICurrentWeatherType {
    if (isAPICurrentWeatherType(weather)) return { ...weather };
    return {
      id: weather.id,
      timestamp: weather.timestamp.toISOString(),
      sunrise: weather.sunrise.toISOString(),
      sunset: weather.sunset.toISOString(),
      temperature: weather.temperature.toString(),
      apparent_temperature: weather.apparentTemperature.toString(),
      pressure: weather.pressure,
      humidity: weather.humidity,
      dew_point: weather.dewPoint.toString(),
      cloud_coverage: weather.cloudCoverage,
      visibility: weather.visibility,
      wind_speed: weather.windSpeed.toString(),
      wind_direction: weather.windDirection,
      owm_weather_id: weather.owmWeatherId,
      owm_weather_title: weather.owmWeatherTitle,
      owm_weather_description: weather.owmWeatherDescription,
      wind_gust: weather.windGust && weather.windGust.toString(),
      uv_index: weather.uvIndex && weather.uvIndex.toString(),
    };
  }

  static toCurrentWeatherType(
    weather: CurrentWeatherType | APICurrentWeatherType
  ): CurrentWeatherType {
    if (isCurrentWeatherType(weather)) return { ...weather };
    return {
      id: weather.id,
      timestamp: new Date(weather.timestamp),
      sunrise: new Date(weather.sunrise),
      sunset: new Date(weather.sunset),
      temperature: parseFloat(weather.temperature),
      apparentTemperature: parseFloat(weather.apparent_temperature),
      pressure: weather.pressure,
      humidity: weather.humidity,
      dewPoint: parseFloat(weather.dew_point),
      cloudCoverage: weather.cloud_coverage,
      visibility: weather.visibility,
      windSpeed: parseFloat(weather.wind_speed),
      windDirection: weather.wind_direction,
      owmWeatherId: weather.owm_weather_id,
      owmWeatherTitle: weather.owm_weather_title,
      owmWeatherDescription: weather.owm_weather_description,
      windGust: weather.wind_gust && parseFloat(weather.wind_gust),
      uvIndex: weather.uv_index && parseFloat(weather.uv_index),
    };
  }

  getCurrentWeather() {
    return this.get<APICurrentWeatherType>(`/v0/current/`);
  }
}
