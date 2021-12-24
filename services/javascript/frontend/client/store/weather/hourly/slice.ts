/** @format */

import { createSlice } from '@reduxjs/toolkit';
import { handleRequestMeta } from 'client/store/handle-request-meta';
import { getHourlyWeather, getHourlyWeatherByTimestamp, getHourlyWeathers } from './thunks';
import { HourlyWeatherSliceStateType } from './types';

export const initialState: HourlyWeatherSliceStateType = {
  requests: {},
  hourly: {},
};

export const HourlyWeatherSlice = createSlice({
  name: 'hourly',
  initialState,
  reducers: {},
  extraReducers: (builder) => {
    builder.addCase(getHourlyWeather.pending, handleRequestMeta);
    builder.addCase(getHourlyWeather.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      state.hourly[action.payload.id] = action.payload;
    });
    builder.addCase(getHourlyWeather.rejected, handleRequestMeta);

    builder.addCase(getHourlyWeatherByTimestamp.pending, handleRequestMeta);
    builder.addCase(getHourlyWeatherByTimestamp.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      state.hourly[action.payload.id] = action.payload;
    });
    builder.addCase(getHourlyWeatherByTimestamp.rejected, handleRequestMeta);

    builder.addCase(getHourlyWeathers.pending, handleRequestMeta);
    builder.addCase(getHourlyWeathers.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      action.payload.forEach((device) => (state.hourly[device.id] = device));
    });
    builder.addCase(getHourlyWeathers.rejected, handleRequestMeta);
  },
});
