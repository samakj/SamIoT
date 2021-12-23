/** @format */

import { createSlice } from '@reduxjs/toolkit';
import { handleRequestMeta } from 'client/store/handle-request-meta';
import { getMeasurement, getMeasurements } from './thunks';
import { MeasurementsSliceStateType } from './types';

export const initialState: MeasurementsSliceStateType = {
  requests: {},
  measurements: {},
};

export const MeasurementsSlice = createSlice({
  name: 'measurements',
  initialState,
  reducers: {},
  extraReducers: (builder) => {
    builder.addCase(getMeasurement.pending, handleRequestMeta);
    builder.addCase(getMeasurement.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      state.measurements[action.payload.id] = {
        ...action.payload,
        timestamp: action.payload.timestamp?.toISOString(),
      };
    });
    builder.addCase(getMeasurement.rejected, handleRequestMeta);

    builder.addCase(getMeasurements.pending, handleRequestMeta);
    builder.addCase(getMeasurements.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      action.payload.forEach(
        (measurement) =>
          (state.measurements[measurement.id] = {
            ...measurement,
            timestamp: measurement.timestamp?.toISOString(),
          })
      );
    });
    builder.addCase(getMeasurements.rejected, handleRequestMeta);
  },
});
