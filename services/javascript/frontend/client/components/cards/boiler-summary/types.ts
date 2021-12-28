/** @format */

import { LocationsStateType } from 'client/store/iot/locations/types';
import { MetricsStateType } from 'client/store/iot/metrics/types';

export interface HeatingSummaryCardPropsType {
  locations: LocationsStateType;
  metrics: MetricsStateType;
}
