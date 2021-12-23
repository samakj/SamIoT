/** @format */

import { RequestMetaStateType } from 'client/store/types';
import {
  GetAverageMeasurementsParamsType,
  GetLatestMeasurementsParamsType,
  GetMeasurementsParamsType,
} from 'shared/javascript/clients/iot/measurements/types';
import { MeasurementAverageType, MeasurementType } from 'shared/javascript/types/iot';

export interface SerialisedMeasurementType extends Omit<MeasurementType, 'timestamp'> {
  timestamp?: string;
}

export interface SerialisedMeasurementAverageType
  extends Omit<MeasurementAverageType, 'start' | 'end'> {
  start?: string;
  end?: string;
}

export interface MeasurementsStateType {
  [id: number]: SerialisedMeasurementType;
}

export interface LatestMeasurementsStateType {
  [key: string]: SerialisedMeasurementType;
}

export interface AverageMeasurementsStateType {
  [key: string]: SerialisedMeasurementAverageType;
}

export interface MeasurementsSliceStateType
  extends RequestMetaStateType<
    | MeasurementType['id']
    | GetMeasurementsParamsType
    | GetLatestMeasurementsParamsType
    | GetAverageMeasurementsParamsType
  > {
  measurements: MeasurementsStateType;
  latest: LatestMeasurementsStateType;
  averages: AverageMeasurementsStateType;
}
