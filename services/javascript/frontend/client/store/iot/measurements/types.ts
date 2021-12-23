/** @format */

import { RequestMetaStateType } from 'client/store/types';
import { GetMeasurementsParamsType } from 'shared/javascript/clients/iot/measurements/types';
import { MeasurementType } from 'shared/javascript/types/iot';

export interface SerialisedMeasurementType extends Omit<MeasurementType, 'timestamp'> {
  timestamp?: string;
}

export interface MeasurementsStateType {
  [id: number]: SerialisedMeasurementType;
}

export interface MeasurementsSliceStateType
  extends RequestMetaStateType<MeasurementType['id'] | GetMeasurementsParamsType> {
  measurements: MeasurementsStateType;
}
