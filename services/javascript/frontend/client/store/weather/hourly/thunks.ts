/** @format */

import { createAsyncThunk } from '@reduxjs/toolkit';
import { WeatherClient } from 'client/clients/weather';
import { GetHourlyWeathersParamsType } from 'shared/javascript/clients/weather/hourly/types';
import { HourlyWeatherType } from 'shared/javascript/types/weather';

export const getHourlyWeather = createAsyncThunk(
  'weather/hourly',
  async (id: HourlyWeatherType['id']) => {
    const response = await WeatherClient.hourly.getHourlyWeather(id);
    const hourly = WeatherClient.hourly.toHourlyWeatherType(response.data);
    return {
      ...hourly,
      timestamp: hourly.timestamp?.toISOString(),
    };
  }
);

export const getHourlyWeatherByTimestamp = createAsyncThunk(
  'weather/hourly/timestamp',
  async (timestamp: HourlyWeatherType['timestamp']) => {
    const response = await WeatherClient.hourly.getHourlyWeatherByTimestamp(timestamp);
    const hourly = WeatherClient.hourly.toHourlyWeatherType(response.data);
    return {
      ...hourly,
      timestamp: hourly.timestamp?.toISOString(),
    };
  }
);

export const getHourlyWeathers = createAsyncThunk(
  'iot/hourly',
  async (filters?: GetHourlyWeathersParamsType) => {
    const response = await WeatherClient.hourly.getHourlyWeathers(filters);
    return response.data.map((apiHourlyWeather) => {
      const hourly = WeatherClient.hourly.toHourlyWeatherType(apiHourlyWeather);
      return {
        ...hourly,
        timestamp: hourly.timestamp?.toISOString(),
      };
    });
  }
);
