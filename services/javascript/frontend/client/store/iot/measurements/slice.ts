/** @format */

import { createSlice } from '@reduxjs/toolkit';
import { handleRequestMeta } from 'client/store/handle-request-meta';
import { MeasurementAverageType, MeasurementType } from 'shared/javascript/types/iot';
import {
  getLatestMeasurements,
  getMeasurement,
  getMeasurements,
  getAverageMeasurements,
} from './thunks';
import {
  MeasurementsSliceStateType,
  SerialisedMeasurementAverageType,
  SerialisedMeasurementType,
} from './types';

export const initialState: MeasurementsSliceStateType = {
  requests: {},
  measurements: {},
  latest: {},
  averages: {},
};

export const getLatestMeasurementsKey = (measurement: SerialisedMeasurementType): string =>
  `${measurement.locationId}:${measurement.deviceId}:` +
  `${measurement.metricId}:${measurement.tags.join(',')}`;

export const getAverageMeasurementsKey = (average: SerialisedMeasurementAverageType): string =>
  `${average.locationId}:${average.metricId}:${average.tags.join(',')}` +
  `${average.start}:${average.end}`;

export const MeasurementsSlice = createSlice({
  name: 'measurements',
  initialState,
  reducers: {},
  extraReducers: (builder) => {
    builder.addCase(getMeasurement.pending, handleRequestMeta);
    builder.addCase(getMeasurement.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      state.measurements[action.payload.id] = action.payload;
    });
    builder.addCase(getMeasurement.rejected, handleRequestMeta);

    builder.addCase(getMeasurements.pending, handleRequestMeta);
    builder.addCase(getMeasurements.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      action.payload.forEach((measurement) => (state.measurements[measurement.id] = measurement));
    });
    builder.addCase(getMeasurements.rejected, handleRequestMeta);

    builder.addCase(getLatestMeasurements.pending, handleRequestMeta);
    builder.addCase(getLatestMeasurements.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      action.payload.forEach(
        (measurement) => (state.latest[getLatestMeasurementsKey(measurement)] = measurement)
      );
    });
    builder.addCase(getLatestMeasurements.rejected, handleRequestMeta);

    builder.addCase(getAverageMeasurements.pending, handleRequestMeta);
    builder.addCase(getAverageMeasurements.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      action.payload.forEach(
        (average) => (state.averages[getAverageMeasurementsKey(average)] = average)
      );
    });
    builder.addCase(getAverageMeasurements.rejected, handleRequestMeta);
  },
});
