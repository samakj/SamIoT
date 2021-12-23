export interface APICurrentWeatherType {
  id: number;
  timestamp: string;
  sunrise: string;
  sunset: string;
  temperature: string;
  apparent_temperature: string;
  pressure: number;
  humidity: number;
  dew_point: string;
  cloud_coverage: number;
  visibility: number;
  wind_speed: string;
  wind_direction: number;
  owm_weather_id: number;
  owm_weather_title: string;
  owm_weather_description: string;
  wind_gust?: string;
  uv_index?: string;
}

export const isAPICurrentWeatherType = (o: any): o is APICurrentWeatherType =>
  o.id != null &&
  o.timestamp != null &&
  o.sunrise != null &&
  o.sunset != null &&
  o.temperature != null &&
  o.apparent_temperature != null &&
  o.pressure != null &&
  o.humidity != null &&
  o.dewPoint != null &&
  o.cloud_coverage != null &&
  o.visibility != null &&
  o.wind_speed != null &&
  o.wind_direction != null &&
  o.owm_weather_id != null &&
  o.owm_weather_title != null &&
  o.owm_weather_description != null;
