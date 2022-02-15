/** @format */

import { APIMetricType } from 'shared/javascript/clients/iot/metrics/types';

export interface MetricsStateType {
  [metricId: number]: APIMetricType;
  [name: string]: APIMetricType;
}
