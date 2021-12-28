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
    const measurement = IoTClient.measurements.toMeasurementType(response.data);
    return {
      ...measurement,
      timestamp: measurement.timestamp.toISOString(),
    };
  }
);

export const getMeasurements = createAsyncThunk(
  'iot/measurements',
  async (filters?: GetMeasurementsParamsType) => {
    const response = await IoTClient.measurements.getMeasurements(filters);
    return response.data.map((apiMeasurement) => {
      const measurement = IoTClient.measurements.toMeasurementType(apiMeasurement);
      return {
        ...measurement,
        timestamp: measurement.timestamp.toISOString(),
      };
    });
  }
);

export const getLatestMeasurements = createAsyncThunk(
  'iot/measurements/latest',
  async (filters?: GetLatestMeasurementsParamsType) => {
    const response = await IoTClient.measurements.getLatestMeasurements(filters);
    return response.data.map((apiMeasurement) => {
      const measurement = IoTClient.measurements.toMeasurementType(apiMeasurement);
      return {
        ...measurement,
        timestamp: measurement.timestamp.toISOString(),
      };
    });
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
    const _filters: GetAverageMeasurementsParamsType = { ...filters };

    if (filters.period) _filters.period = filters.period / 1000;

    const response = await IoTClient.measurements.getAverageMeasurements(
      filters.locationId,
      filters.metricId,
      filters.tags,
      _filters
    );
    return response.data.map((apiAverage) => {
      const average = IoTClient.measurements.toMeasurementAverageType(apiAverage);
      return {
        ...average,
        start: average.start?.toISOString(),
        end: average.end?.toISOString(),
      };
    });
  }
);
