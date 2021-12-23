/** @format */

import { createAsyncThunk } from '@reduxjs/toolkit';
import { IoTClient } from 'client/clients/iot';
import { GetMetricsParamsType } from 'shared/javascript/clients/iot/metrics/types';
import { MetricType } from 'shared/javascript/types/iot';

export const getMetric = createAsyncThunk('iot/metric', async (id: MetricType['id']) => {
  const response = await IoTClient.metrics.getMetric(id);
  return IoTClient.metrics.toMetricType(response.data);
});

export const getMetricByName = createAsyncThunk(
  'iot/metric/name',
  async (name: MetricType['name']) => {
    const response = await IoTClient.metrics.getMetricByName(name);
    return IoTClient.metrics.toMetricType(response.data);
  }
);

export const getMetricByAbbreviation = createAsyncThunk(
  'iot/metric/abbreviation',
  async (abbreviation: MetricType['abbreviation']) => {
    const response = await IoTClient.metrics.getMetricByAbbreviation(abbreviation);
    return IoTClient.metrics.toMetricType(response.data);
  }
);

export const getMetrics = createAsyncThunk(
  'iot/metrics',
  async (filters?: GetMetricsParamsType) => {
    const response = await IoTClient.metrics.getMetrics(filters);
    return response.data.map(IoTClient.metrics.toMetricType);
  }
);
