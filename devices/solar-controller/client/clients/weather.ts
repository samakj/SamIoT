/** @format */

import { WeatherClient as WeatherClientClass } from 'shared/javascript/clients/weather';

export const WeatherClient = new WeatherClientClass(
  process.env.WEATHER_API_HOST,
  process.env.WEATHER_API_PORT,
  false
);
