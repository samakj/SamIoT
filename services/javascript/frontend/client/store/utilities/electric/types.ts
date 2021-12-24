/** @format */

import { RequestMetaStateType } from 'client/store/types';
import { GetDevicesParamsType } from 'shared/javascript/clients/iot/devices/types';
import { UtilitiesConsumptionType } from 'shared/javascript/types/utilities';

export interface SerialisedUtilitiesConsumptionType
  extends Omit<UtilitiesConsumptionType, 'timestamp'> {
  timestamp?: string;
}

export interface ElectricConsumptionStateType {
  [id: number]: SerialisedUtilitiesConsumptionType;
}

export interface ElectricSliceStateType
  extends RequestMetaStateType<UtilitiesConsumptionType['id'] | GetDevicesParamsType> {
  consumption: ElectricConsumptionStateType;
}
