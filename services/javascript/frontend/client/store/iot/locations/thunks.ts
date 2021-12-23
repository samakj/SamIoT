/** @format */

import { createAsyncThunk } from '@reduxjs/toolkit';
import { IoTClient } from 'client/clients/iot';
import { GetLocationsParamsType } from 'shared/javascript/clients/iot/locations/types';
import { LocationType } from 'shared/javascript/types/iot';

export const getLocation = createAsyncThunk('iot/location', async (id: LocationType['id']) => {
  const response = await IoTClient.locations.getLocation(id);
  return IoTClient.locations.toLocationType(response.data);
});

export const getLocationByName = createAsyncThunk(
  'iot/location/name',
  async (name: LocationType['name']) => {
    const response = await IoTClient.locations.getLocationByName(name);
    return IoTClient.locations.toLocationType(response.data);
  }
);

export const getLocations = createAsyncThunk(
  'iot/locations',
  async (filters?: GetLocationsParamsType) => {
    const response = await IoTClient.locations.getLocations(filters);
    return response.data.map(IoTClient.locations.toLocationType);
  }
);
