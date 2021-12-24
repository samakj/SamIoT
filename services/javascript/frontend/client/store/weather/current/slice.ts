/** @format */

import { createSlice } from '@reduxjs/toolkit';
import { handleRequestMeta } from 'client/store/handle-request-meta';
import { getCurrentWeather } from './thunks';
import { CurrentWeathersSliceStateType } from './types';

export const initialState: CurrentWeathersSliceStateType = {
  requests: {},
  current: null,
};

export const CurrentWeatherSlice = createSlice({
  name: 'devices',
  initialState,
  reducers: {},
  extraReducers: (builder) => {
    builder.addCase(getCurrentWeather.pending, handleRequestMeta);
    builder.addCase(getCurrentWeather.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      state.current = action.payload;
    });
    builder.addCase(getCurrentWeather.rejected, handleRequestMeta);
  },
});
