/** @format */

import { LocationsStateType } from 'client/store/iot/locations/types';
import { MetricsStateType } from 'client/store/iot/metrics/types';
import { StyleProps } from 'client/style/types';
import { MeasurementType } from 'shared/javascript/types/iot';

export interface HeatingSummaryCardPropsType {
  locations?: LocationsStateType;
  metrics?: MetricsStateType;
}

export interface TankTemperatureGraphPropsType extends StyleProps {
  latest?: MeasurementType;
  locations?: LocationsStateType;
  metrics?: MetricsStateType;
}
