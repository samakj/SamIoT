/** @format */

import { createSlice } from '@reduxjs/toolkit';
import { handleRequestMeta } from 'client/store/handle-request-meta';
import { getElectricConsumption, getElectricConsumptions } from './thunks';
import { ElectricSliceStateType } from './types';

export const initialState: ElectricSliceStateType = {
  requests: {},
  consumption: {},
};

export const ElectricSlice = createSlice({
  name: 'electric',
  initialState,
  reducers: {},
  extraReducers: (builder) => {
    builder.addCase(getElectricConsumption.pending, handleRequestMeta);
    builder.addCase(getElectricConsumption.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      state.consumption[action.payload.id] = action.payload;
    });
    builder.addCase(getElectricConsumption.rejected, handleRequestMeta);

    builder.addCase(getElectricConsumptions.pending, handleRequestMeta);
    builder.addCase(getElectricConsumptions.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      action.payload.forEach((consumption) => (state.consumption[consumption.id] = consumption));
    });
    builder.addCase(getElectricConsumptions.rejected, handleRequestMeta);
  },
});
