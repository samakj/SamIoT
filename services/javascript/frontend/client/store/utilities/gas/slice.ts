/** @format */

import { createSlice } from '@reduxjs/toolkit';
import { handleRequestMeta } from 'client/store/handle-request-meta';
import { getGasConsumption, getGasConsumptions } from './thunks';
import { GasSliceStateType } from './types';

export const initialState: GasSliceStateType = {
  requests: {},
  consumption: {},
};

export const GasSlice = createSlice({
  name: 'gas',
  initialState,
  reducers: {},
  extraReducers: (builder) => {
    builder.addCase(getGasConsumption.pending, handleRequestMeta);
    builder.addCase(getGasConsumption.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      state.consumption[action.payload.id] = action.payload;
    });
    builder.addCase(getGasConsumption.rejected, handleRequestMeta);

    builder.addCase(getGasConsumptions.pending, handleRequestMeta);
    builder.addCase(getGasConsumptions.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      action.payload.forEach((consumption) => (state.consumption[consumption.id] = consumption));
    });
    builder.addCase(getGasConsumptions.rejected, handleRequestMeta);
  },
});
