/** @format */

import { createAsyncThunk } from '@reduxjs/toolkit';
import { WeatherClient } from 'client/clients/weather';
import { GetDailyWeathersParamsType } from 'shared/javascript/clients/weather/daily/types';
import { DailyWeatherType } from 'shared/javascript/types/weather';

export const getDailyWeather = createAsyncThunk(
  'weather/daily',
  async (id: DailyWeatherType['id']) => {
    const response = await WeatherClient.daily.getDailyWeather(id);
    const daily = WeatherClient.daily.toDailyWeatherType(response.data);
    return {
      ...daily,
      timestamp: daily.timestamp?.toISOString(),
      sunrise: daily.sunrise?.toISOString(),
      sunset: daily.sunset?.toISOString(),
      moonrise: daily.moonrise?.toISOString(),
      moonset: daily.moonset?.toISOString(),
    };
  }
);

export const getDailyWeatherByTimestamp = createAsyncThunk(
  'weather/daily/timestamp',
  async (timestamp: DailyWeatherType['timestamp']) => {
    const response = await WeatherClient.daily.getDailyWeatherByTimestamp(timestamp);
    const daily = WeatherClient.daily.toDailyWeatherType(response.data);
    return {
      ...daily,
      timestamp: daily.timestamp?.toISOString(),
      sunrise: daily.sunrise?.toISOString(),
      sunset: daily.sunset?.toISOString(),
      moonrise: daily.moonrise?.toISOString(),
      moonset: daily.moonset?.toISOString(),
    };
  }
);

export const getDailyWeathers = createAsyncThunk(
  'iot/daily',
  async (filters?: GetDailyWeathersParamsType) => {
    const response = await WeatherClient.daily.getDailyWeathers(filters);
    return response.data.map((apiDailyWeather) => {
      const daily = WeatherClient.daily.toDailyWeatherType(apiDailyWeather);
      return {
        ...daily,
        timestamp: daily.timestamp?.toISOString(),
        sunrise: daily.sunrise?.toISOString(),
        sunset: daily.sunset?.toISOString(),
        moonrise: daily.moonrise?.toISOString(),
        moonset: daily.moonset?.toISOString(),
      };
    });
  }
);
