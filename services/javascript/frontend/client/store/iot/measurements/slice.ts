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
import { MeasurementsSliceStateType } from './types';

export const initialState: MeasurementsSliceStateType = {
  requests: {},
  measurements: {},
  latest: {},
  averages: {},
};

export const getLatestMeasurementsKey = (measurement: MeasurementType): string =>
  `${measurement.locationId}:${measurement.deviceId}:` +
  `${measurement.metricId}:${measurement.tags.join(',')}`;

export const getAverageMeasurementsKey = (average: MeasurementAverageType): string =>
  `${average.locationId}:${average.metricId}:${average.tags.join(',')}` +
  `${average.start.toISOString()}:${average.end.toISOString()}`;

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

    builder.addCase(getLatestMeasurements.pending, handleRequestMeta);
    builder.addCase(getLatestMeasurements.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      action.payload.forEach(
        (measurement) =>
          (state.latest[getLatestMeasurementsKey(measurement)] = {
            ...measurement,
            timestamp: measurement.timestamp?.toISOString(),
          })
      );
    });
    builder.addCase(getLatestMeasurements.rejected, handleRequestMeta);

    builder.addCase(getAverageMeasurements.pending, handleRequestMeta);
    builder.addCase(getAverageMeasurements.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      action.payload.forEach(
        (average) =>
          (state.averages[getAverageMeasurementsKey(average)] = {
            ...average,
            start: average.start?.toISOString(),
            end: average.end?.toISOString(),
          })
      );
    });
    builder.addCase(getAverageMeasurements.rejected, handleRequestMeta);
  },
});
