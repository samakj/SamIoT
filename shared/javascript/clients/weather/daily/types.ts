import { DailyWeatherType } from "../../../types/weather";

export interface APIDailyWeatherType {
  id: number;
  timestamp: string;
  sunrise: string;
  sunset: string;
  moonrise: string;
  moonset: string;
  moon_phase: number;
  temperature: string;
  temperature_night: string;
  temperature_evening: string;
  temperature_morning: string;
  temperature_min: string;
  temperature_max: string;
  apparent_temperature: string;
  apparent_temperature_night: string;
  apparent_temperature_evening: string;
  apparent_temperature_morning: string;
  pressure: number;
  humidity: number;
  dew_point: string;
  cloud_coverage: number;
  wind_speed: string;
  wind_direction: number;
  owm_weather_id: number;
  owm_weather_title: string;
  owm_weather_description: string;
  chance_of_rain: string;
  visibility?: number;
  wind_gust?: string;
  uv_index?: string;
}

export const isAPIDailyWeatherType = (o: any): o is APIDailyWeatherType =>
  o.id != null &&
  o.timestamp != null &&
  o.sunrise != null &&
  o.sunset != null &&
  o.moonrise != null &&
  o.moonset != null &&
  o.moon_phase != null &&
  o.temperature != null &&
  o.temperature_night != null &&
  o.temperature_evening != null &&
  o.temperature_morning != null &&
  o.temperature_min != null &&
  o.temperature_max != null &&
  o.apparent_temperature != null &&
  o.apparent_temperature_night != null &&
  o.apparent_temperature_evening != null &&
  o.apparent_temperature_morning != null &&
  o.pressure != null &&
  o.humidity != null &&
  o.dew_point != null &&
  o.cloud_coverage != null &&
  o.wind_speed != null &&
  o.wind_direction != null &&
  o.owm_weather_id != null &&
  o.owm_weather_title != null &&
  o.owm_weather_description != null &&
  o.chance_of_rain != null;

export interface GetDailyWeathersParamsType {
  id?: DailyWeatherType["id"] | DailyWeatherType["id"];
  timestamp?: DailyWeatherType["timestamp"] | DailyWeatherType["timestamp"];
  owmWeatherId?:
    | DailyWeatherType["owmWeatherId"]
    | DailyWeatherType["owmWeatherId"];
  owmWeatherTitle?:
    | DailyWeatherType["owmWeatherTitle"]
    | DailyWeatherType["owmWeatherTitle"];
  temperatureGte?: DailyWeatherType["temperature"];
  temperatureLte?: DailyWeatherType["temperature"];
  temperatureMinGte?: DailyWeatherType["temperatureMin"];
  temperatureMinLte?: DailyWeatherType["temperatureMin"];
  temperatureMaxGte?: DailyWeatherType["temperatureMax"];
  temperatureMaxLte?: DailyWeatherType["temperatureMax"];
  apparentTemperatureGte?: DailyWeatherType["apparentTemperature"];
  apparentTemperatureLte?: DailyWeatherType["apparentTemperature"];
  timestampGte?: DailyWeatherType["timestamp"];
  timestampLte?: DailyWeatherType["timestamp"];
}
export interface GetDailyWeathersAPIParamsType {
  id?: APIDailyWeatherType["id"] | APIDailyWeatherType["id"];
  timestamp?:
    | APIDailyWeatherType["timestamp"]
    | APIDailyWeatherType["timestamp"][];
  owm_weather_id?:
    | APIDailyWeatherType["owm_weather_id"]
    | APIDailyWeatherType["owm_weather_id"];
  owm_weather_title?:
    | APIDailyWeatherType["owm_weather_title"]
    | APIDailyWeatherType["owm_weather_title"];
  temperature_gte?: APIDailyWeatherType["temperature"];
  temperature_lte?: APIDailyWeatherType["temperature"];
  temperature_min_gte?: APIDailyWeatherType["temperature_min"];
  temperature_min_lte?: APIDailyWeatherType["temperature_min"];
  temperature_max_gte?: APIDailyWeatherType["temperature_max"];
  temperature_max_lte?: APIDailyWeatherType["temperature_max"];
  apparent_temperature_gte?: APIDailyWeatherType["apparent_temperature"];
  apparent_temperature_lte?: APIDailyWeatherType["apparent_temperature"];
  timestamp_gte?: APIDailyWeatherType["timestamp"];
  timestamp_lte?: APIDailyWeatherType["timestamp"];
}
