/** @format */

import { createAsyncThunk } from '@reduxjs/toolkit';
import { WeatherClient } from 'client/clients/weather';

export const getCurrentWeather = createAsyncThunk('weather/current', async () => {
  const response = await WeatherClient.current.getCurrentWeather();
  const current = WeatherClient.current.toCurrentWeatherType(response.data);
  return {
    ...current,
    timestamp: current.timestamp?.toISOString(),
    sunrise: current.sunrise?.toISOString(),
    sunset: current.sunset?.toISOString(),
  };
});
