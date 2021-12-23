import { APIClient } from "../../../extensions/axios/api-client";
import { HourlyWeatherType, isHourlyWeatherType } from "../../../types/weather";
import {
  APIHourlyWeatherType,
  GetHourlyWeathersAPIParamsType,
  GetHourlyWeathersParamsType,
  isAPIHourlyWeatherType,
} from "./types";

export class HourlyWeathersClient extends APIClient {
  constructor(host: string, port: number | string | null, ssl: boolean) {
    super(host, port, ssl);
  }

  static toAPIHourlyWeatherType(
    weather: HourlyWeatherType | APIHourlyWeatherType
  ): APIHourlyWeatherType {
    if (isAPIHourlyWeatherType(weather)) return { ...weather };
    return {
      id: weather.id,
      timestamp: weather.timestamp.toISOString(),
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

  static toHourlyWeatherType(
    weather: HourlyWeatherType | APIHourlyWeatherType
  ): HourlyWeatherType {
    if (isHourlyWeatherType(weather)) return { ...weather };
    return {
      id: weather.id,
      timestamp: new Date(weather.timestamp),
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

  getHourlyWeather(id: HourlyWeatherType["id"]) {
    return this.get<APIHourlyWeatherType>(`/v0/hourly/${id}`);
  }

  getHourlyWeatherByTimestamp(timestamp: HourlyWeatherType["timestamp"]) {
    return this.get<APIHourlyWeatherType>(
      `/v0/hourly/timestamp/${timestamp.toISOString()}`
    );
  }

  getHourlyWeathers(params?: GetHourlyWeathersParamsType) {
    const _params: GetHourlyWeathersAPIParamsType = {};

    if (params?.id != null) _params.id = params.id;
    if (params?.timestamp != null)
      _params.timestamp = Array.isArray(params.timestamp)
        ? params.timestamp.map((t) => t.toISOString())
        : params.timestamp.toISOString();
    if (params?.timestampGte != null)
      _params.timestamp_gte = params.timestampGte.toISOString();
    if (params?.timestampLte != null)
      _params.timestamp_lte = params.timestampLte.toISOString();
    if (params?.temperatureGte != null)
      _params.temperature_gte = params.temperatureGte.toString();
    if (params?.temperatureLte != null)
      _params.temperature_lte = params.temperatureLte.toString();
    if (params?.apparentTemperatureGte != null)
      _params.apparent_temperature_gte =
        params.apparentTemperatureGte.toString();
    if (params?.apparentTemperatureLte != null)
      _params.apparent_temperature_lte =
        params.apparentTemperatureLte.toString();

    return this.get<APIHourlyWeatherType[]>(`/v0/hourlyweathers/`, {
      params: _params,
    });
  }
}
