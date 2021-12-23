import { APIClient } from "../../../extensions/axios/api-client";
import { DailyWeatherType, isDailyWeatherType } from "../../../types/weather";
import {
  APIDailyWeatherType,
  GetDailyWeathersAPIParamsType,
  GetDailyWeathersParamsType,
  isAPIDailyWeatherType,
} from "./types";

export class DailyWeathersClient extends APIClient {
  constructor(host: string, port: number | string | null, ssl: boolean) {
    super(host, port, ssl);
  }

  static toAPIDailyWeatherType(
    weather: DailyWeatherType | APIDailyWeatherType
  ): APIDailyWeatherType {
    if (isAPIDailyWeatherType(weather)) return { ...weather };
    return {
      id: weather.id,
      timestamp: weather.timestamp.toISOString(),
      sunrise: weather.sunrise.toISOString(),
      sunset: weather.sunset.toISOString(),
      moonrise: weather.moonrise.toISOString(),
      moonset: weather.moonset.toISOString(),
      moon_phase: weather.moonPhase,
      temperature: weather.temperature.toString(),
      temperature_night: weather.temperatureNight.toString(),
      temperature_evening: weather.temperatureEvening.toString(),
      temperature_morning: weather.temperatureMorning.toString(),
      temperature_min: weather.temperatureMin.toString(),
      temperature_max: weather.temperatureMax.toString(),
      apparent_temperature: weather.apparentTemperature.toString(),
      apparent_temperature_night: weather.apparentTemperatureNight.toString(),
      apparent_temperature_evening:
        weather.apparentTemperatureEvening.toString(),
      apparent_temperature_morning:
        weather.apparentTemperatureMorning.toString(),
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
      chance_of_rain: weather.chanceOfRain.toString(),
      wind_gust: weather.windGust && weather.windGust.toString(),
      uv_index: weather.uvIndex && weather.uvIndex.toString(),
    };
  }

  static toDailyWeatherType(
    weather: DailyWeatherType | APIDailyWeatherType
  ): DailyWeatherType {
    if (isDailyWeatherType(weather)) return { ...weather };
    return {
      id: weather.id,
      timestamp: new Date(weather.timestamp),
      sunrise: new Date(weather.sunrise),
      sunset: new Date(weather.sunset),
      moonrise: new Date(weather.moonrise),
      moonset: new Date(weather.moonset),
      moonPhase: weather.moon_phase,
      temperature: parseFloat(weather.temperature),
      temperatureNight: parseFloat(weather.temperature_night),
      temperatureEvening: parseFloat(weather.temperature_evening),
      temperatureMorning: parseFloat(weather.temperature_morning),
      temperatureMin: parseFloat(weather.temperature_min),
      temperatureMax: parseFloat(weather.temperature_max),
      apparentTemperature: parseFloat(weather.apparent_temperature),
      apparentTemperatureNight: parseFloat(weather.apparent_temperature_night),
      apparentTemperatureEvening: parseFloat(
        weather.apparent_temperature_evening
      ),
      apparentTemperatureMorning: parseFloat(
        weather.apparent_temperature_morning
      ),
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
      chanceOfRain: parseFloat(weather.chance_of_rain),
      windGust: weather.wind_gust && parseFloat(weather.wind_gust),
      uvIndex: weather.uv_index && parseFloat(weather.uv_index),
    };
  }

  getDailyWeather(id: DailyWeatherType["id"]) {
    return this.get<APIDailyWeatherType>(`/v0/daily/${id}`);
  }

  getDailyWeatherByTimestamp(timestamp: DailyWeatherType["timestamp"]) {
    return this.get<APIDailyWeatherType>(
      `/v0/daily/timestamp/${timestamp.toISOString()}`
    );
  }

  getDailyWeathers(params?: GetDailyWeathersParamsType) {
    const _params: GetDailyWeathersAPIParamsType = {};

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
    if (params?.temperatureMinGte != null)
      _params.temperature_min_gte = params.temperatureMinGte.toString();
    if (params?.temperatureMinLte != null)
      _params.temperature_min_lte = params.temperatureMinLte.toString();
    if (params?.temperatureMaxGte != null)
      _params.temperature_max_gte = params.temperatureMaxGte.toString();
    if (params?.temperatureMaxLte != null)
      _params.temperature_max_lte = params.temperatureMaxLte.toString();
    if (params?.apparentTemperatureGte != null)
      _params.apparent_temperature_gte =
        params.apparentTemperatureGte.toString();
    if (params?.apparentTemperatureLte != null)
      _params.apparent_temperature_lte =
        params.apparentTemperatureLte.toString();

    return this.get<APIDailyWeatherType[]>(`/v0/dailyweathers/`, {
      params: _params,
    });
  }
}
