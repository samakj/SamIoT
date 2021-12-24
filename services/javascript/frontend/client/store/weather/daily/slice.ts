/** @format */

import { createSlice } from '@reduxjs/toolkit';
import { handleRequestMeta } from 'client/store/handle-request-meta';
import { getDailyWeather, getDailyWeatherByTimestamp, getDailyWeathers } from './thunks';
import { DailyWeatherSliceStateType } from './types';

export const initialState: DailyWeatherSliceStateType = {
  requests: {},
  daily: {},
};

export const DailyWeatherSlice = createSlice({
  name: 'daily',
  initialState,
  reducers: {},
  extraReducers: (builder) => {
    builder.addCase(getDailyWeather.pending, handleRequestMeta);
    builder.addCase(getDailyWeather.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      state.daily[action.payload.id] = action.payload;
    });
    builder.addCase(getDailyWeather.rejected, handleRequestMeta);

    builder.addCase(getDailyWeatherByTimestamp.pending, handleRequestMeta);
    builder.addCase(getDailyWeatherByTimestamp.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      state.daily[action.payload.id] = action.payload;
    });
    builder.addCase(getDailyWeatherByTimestamp.rejected, handleRequestMeta);

    builder.addCase(getDailyWeathers.pending, handleRequestMeta);
    builder.addCase(getDailyWeathers.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      action.payload.forEach((device) => (state.daily[device.id] = device));
    });
    builder.addCase(getDailyWeathers.rejected, handleRequestMeta);
  },
});
