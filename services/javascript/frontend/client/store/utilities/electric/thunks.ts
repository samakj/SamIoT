/** @format */

import { createAsyncThunk } from '@reduxjs/toolkit';
import { UtilitiesClient } from 'client/clients/utilities';
import { GetUtilitiesConsumptionsParamsType } from 'shared/javascript/clients/utilities/types';
import { UtilitiesConsumptionType } from 'shared/javascript/types/utilities';

export const getElectricConsumption = createAsyncThunk(
  'utilities/electric/consumption',
  async (id: UtilitiesConsumptionType['id']) => {
    const response = await UtilitiesClient.electric.getElectricConsumption(id);
    const consumption = UtilitiesClient.electric.toElectricConsumptionType(response.data);
    return {
      ...consumption,
      timestamp: consumption.timestamp?.toISOString(),
    };
  }
);

export const getElectricConsumptions = createAsyncThunk(
  'utilities/electric/consumptions',
  async (filters?: GetUtilitiesConsumptionsParamsType) => {
    const response = await UtilitiesClient.electric.getElectricConsumptions(filters);
    return response.data.map((apiConsumption) => {
      const consumption = UtilitiesClient.electric.toElectricConsumptionType(apiConsumption);
      return {
        ...consumption,
        timestamp: consumption.timestamp?.toISOString(),
      };
    });
  }
);
