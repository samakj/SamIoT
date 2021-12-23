/** @format */

import { createSlice } from '@reduxjs/toolkit';
import { handleRequestMeta } from 'client/store/handle-request-meta';
import { getMetric, getMetricByAbbreviation, getMetricByName, getMetrics } from './thunks';
import { MetricsSliceStateType } from './types';

export const initialState: MetricsSliceStateType = {
  requests: {},
  metrics: {},
};

export const MetricsSlice = createSlice({
  name: 'metrics',
  initialState,
  reducers: {},
  extraReducers: (builder) => {
    builder.addCase(getMetric.pending, handleRequestMeta);
    builder.addCase(getMetric.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      state.metrics[action.payload.id] = action.payload;
    });
    builder.addCase(getMetric.rejected, handleRequestMeta);

    builder.addCase(getMetricByName.pending, handleRequestMeta);
    builder.addCase(getMetricByName.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      state.metrics[action.payload.id] = action.payload;
    });
    builder.addCase(getMetricByName.rejected, handleRequestMeta);

    builder.addCase(getMetricByAbbreviation.pending, handleRequestMeta);
    builder.addCase(getMetricByAbbreviation.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      state.metrics[action.payload.id] = action.payload;
    });
    builder.addCase(getMetricByAbbreviation.rejected, handleRequestMeta);

    builder.addCase(getMetrics.pending, handleRequestMeta);
    builder.addCase(getMetrics.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      action.payload.forEach((metric) => (state.metrics[metric.id] = metric));
    });
    builder.addCase(getMetrics.rejected, handleRequestMeta);
  },
});
