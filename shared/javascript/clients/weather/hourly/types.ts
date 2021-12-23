import { HourlyWeatherType } from "../../../types/weather";

export interface APIHourlyWeatherType {
  id: number;
  timestamp: string;
  temperature: string;
  apparent_temperature: string;
  pressure: number;
  humidity: number;
  dew_point: string;
  cloud_coverage: number;
  wind_speed: string;
  wind_direction: number;
  owm_weather_id: number;
  owm_weather_title: string;
  owm_weather_description: string;
  visibility?: number;
  wind_gust?: string;
  uv_index?: string;
}

export const isAPIHourlyWeatherType = (o: any): o is APIHourlyWeatherType =>
  o.id != null &&
  o.timestamp != null &&
  o.temperature != null &&
  o.apparent_temperature != null &&
  o.pressure != null &&
  o.humidity != null &&
  o.dewPoint != null &&
  o.cloud_coverage != null &&
  o.wind_speed != null &&
  o.wind_direction != null &&
  o.owm_weather_id != null &&
  o.owm_weather_title != null &&
  o.owm_weather_description != null;

export interface GetHourlyWeathersParamsType {
  id?: HourlyWeatherType["id"] | HourlyWeatherType["id"];
  timestamp?: HourlyWeatherType["timestamp"] | HourlyWeatherType["timestamp"];
  owmWeatherId?:
    | HourlyWeatherType["owmWeatherId"]
    | HourlyWeatherType["owmWeatherId"];
  owmWeatherTitle?:
    | HourlyWeatherType["owmWeatherTitle"]
    | HourlyWeatherType["owmWeatherTitle"];
  temperatureGte?: HourlyWeatherType["temperature"];
  temperatureLte?: HourlyWeatherType["temperature"];
  apparentTemperatureGte?: HourlyWeatherType["apparentTemperature"];
  apparentTemperatureLte?: HourlyWeatherType["apparentTemperature"];
  timestampGte?: HourlyWeatherType["timestamp"];
  timestampLte?: HourlyWeatherType["timestamp"];
}
export interface GetHourlyWeathersAPIParamsType {
  id?: APIHourlyWeatherType["id"] | APIHourlyWeatherType["id"];
  timestamp?:
    | APIHourlyWeatherType["timestamp"]
    | APIHourlyWeatherType["timestamp"][];
  owm_weather_id?:
    | APIHourlyWeatherType["owm_weather_id"]
    | APIHourlyWeatherType["owm_weather_id"];
  owm_weather_title?:
    | APIHourlyWeatherType["owm_weather_title"]
    | APIHourlyWeatherType["owm_weather_title"];
  temperature_gte?: APIHourlyWeatherType["temperature"];
  temperature_lte?: APIHourlyWeatherType["temperature"];
  apparent_temperature_gte?: APIHourlyWeatherType["apparent_temperature"];
  apparent_temperature_lte?: APIHourlyWeatherType["apparent_temperature"];
  timestamp_gte?: APIHourlyWeatherType["timestamp"];
  timestamp_lte?: APIHourlyWeatherType["timestamp"];
}
