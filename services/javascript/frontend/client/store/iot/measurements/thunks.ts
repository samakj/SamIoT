/** @format */

import { createAsyncThunk } from '@reduxjs/toolkit';
import { IoTClient } from 'client/clients/iot';
import {
  GetAverageMeasurementsParamsType,
  GetLatestMeasurementsParamsType,
  GetMeasurementsParamsType,
} from 'shared/javascript/clients/iot/measurements/types';
import { MeasurementType } from 'shared/javascript/types/iot';

export const getMeasurement = createAsyncThunk(
  'iot/measurement',
  async (id: MeasurementType['id']) => {
    const response = await IoTClient.measurements.getMeasurement(id);
    return IoTClient.measurements.toMeasurementType(response.data);
  }
);

export const getMeasurements = createAsyncThunk(
  'iot/measurements',
  async (filters?: GetMeasurementsParamsType) => {
    const response = await IoTClient.measurements.getMeasurements(filters);
    return response.data.map(IoTClient.measurements.toMeasurementType);
  }
);

export const getLatestMeasurements = createAsyncThunk(
  'iot/measurements/latest',
  async (filters?: GetLatestMeasurementsParamsType) => {
    const response = await IoTClient.measurements.getLatestMeasurements(filters);
    return response.data.map(IoTClient.measurements.toMeasurementType);
  }
);

export const getAverageMeasurements = createAsyncThunk(
  'iot/measurements/average',
  async (
    filters: GetAverageMeasurementsParamsType & {
      locationId: MeasurementType['locationId'];
      metricId: MeasurementType['metricId'];
      tags: MeasurementType['tags'];
    }
  ) => {
    const response = await IoTClient.measurements.getAverageMeasurements(
      filters.locationId,
      filters.metricId,
      filters.tags,
      filters
    );
    return response.data.map(IoTClient.measurements.toMeasurementAverageType);
  }
);
