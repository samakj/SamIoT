/** @format */

import { createAsyncThunk } from '@reduxjs/toolkit';
import { UtilitiesClient } from 'client/clients/utilities';
import { GetUtilitiesConsumptionsParamsType } from 'shared/javascript/clients/utilities/types';
import { UtilitiesConsumptionType } from 'shared/javascript/types/utilities';

export const getGasConsumption = createAsyncThunk(
  'utilities/gas/consumption',
  async (id: UtilitiesConsumptionType['id']) => {
    const response = await UtilitiesClient.gas.getGasConsumption(id);
    const consumption = UtilitiesClient.gas.toGasConsumptionType(response.data);
    return {
      ...consumption,
      timestamp: consumption.timestamp?.toISOString(),
    };
  }
);

export const getGasConsumptions = createAsyncThunk(
  'utilities/gas/consumptions',
  async (filters?: GetUtilitiesConsumptionsParamsType) => {
    const response = await UtilitiesClient.gas.getGasConsumptions(filters);
    return response.data.map((apiConsumption) => {
      const consumption = UtilitiesClient.gas.toGasConsumptionType(apiConsumption);
      return {
        ...consumption,
        timestamp: consumption.timestamp?.toISOString(),
      };
    });
  }
);
