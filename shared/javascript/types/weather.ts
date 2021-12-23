export interface CurrentWeatherType {
  id: number;
  timestamp: Date;
  sunrise: Date;
  sunset: Date;
  temperature: number;
  apparentTemperature: number;
  pressure: number;
  humidity: number;
  dewPoint: number;
  cloudCoverage: number;
  visibility: number;
  windSpeed: number;
  windDirection: number;
  owmWeatherId: number;
  owmWeatherTitle: string;
  owmWeatherDescription: string;
  windGust?: number;
  uvIndex?: number;
}

export const isCurrentWeatherType = (o: any): o is CurrentWeatherType =>
  o.id != null &&
  o.timestamp != null &&
  o.sunrise != null &&
  o.sunset != null &&
  o.temperature != null &&
  o.apparentTemperature != null &&
  o.pressure != null &&
  o.humidity != null &&
  o.dewPoint != null &&
  o.cloudCoverage != null &&
  o.visibility != null &&
  o.windSpeed != null &&
  o.windDirection != null &&
  o.owmWeatherId != null &&
  o.owmWeatherTitle != null &&
  o.owmWeatherDescription != null;

export interface HourlyWeatherType {
  id: number;
  timestamp: Date;
  temperature: number;
  apparentTemperature: number;
  pressure: number;
  humidity: number;
  dewPoint: number;
  cloudCoverage: number;
  windSpeed: number;
  windDirection: number;
  owmWeatherId: number;
  owmWeatherTitle: string;
  owmWeatherDescription: string;
  chanceOfRain?: number;
  visibility?: number;
  windGust?: number;
  uvIndex?: number;
}

export const isHourlyWeatherType = (o: any): o is HourlyWeatherType =>
  o.id != null &&
  o.timestamp != null &&
  o.temperature != null &&
  o.apparentTemperature != null &&
  o.pressure != null &&
  o.humidity != null &&
  o.dewPoint != null &&
  o.cloudCoverage != null &&
  o.windSpeed != null &&
  o.windDirection != null &&
  o.owmWeatherId != null &&
  o.owmWeatherTitle != null &&
  o.owmWeatherDescription != null;

export interface DailyWeatherType {
  id: number;
  timestamp: Date;
  sunrise: Date;
  sunset: Date;
  moonrise: Date;
  moonset: Date;
  moonPhase: number;
  temperature: number;
  temperatureNight: number;
  temperatureEvening: number;
  temperatureMorning: number;
  temperatureMin: number;
  temperatureMax: number;
  apparentTemperature: number;
  apparentTemperatureNight: number;
  apparentTemperatureEvening: number;
  apparentTemperatureMorning: number;
  pressure: number;
  humidity: number;
  dewPoint: number;
  cloudCoverage: number;
  windSpeed: number;
  windDirection: number;
  owmWeatherId: number;
  owmWeatherTitle: string;
  owmWeatherDescription: string;
  chanceOfRain: number;
  visibility?: number;
  windGust?: number;
  uvIndex?: number;
}

export const isDailyWeatherType = (o: any): o is DailyWeatherType =>
  o.id != null &&
  o.timestamp != null &&
  o.sunrise != null &&
  o.sunset != null &&
  o.moonrise != null &&
  o.moonset != null &&
  o.moonPhase != null &&
  o.temperature != null &&
  o.temperatureNight != null &&
  o.temperatureEvening != null &&
  o.temperatureMorning != null &&
  o.temperatureMin != null &&
  o.temperatureMax != null &&
  o.apparentTemperature != null &&
  o.apparentTemperatureNight != null &&
  o.apparentTemperatureEvening != null &&
  o.apparentTemperatureMorning != null &&
  o.pressure != null &&
  o.humidity != null &&
  o.dewPoint != null &&
  o.cloudCoverage != null &&
  o.windSpeed != null &&
  o.windDirection != null &&
  o.owmWeatherId != null &&
  o.owmWeatherTitle != null &&
  o.owmWeatherDescription != null &&
  o.chanceOfRain != null;
