/** @format */

import { RequestMetaStateType } from 'client/store/types';
import { GetMetricsParamsType } from 'shared/javascript/clients/iot/metrics/types';
import { MetricType } from 'shared/javascript/types/iot';

export interface MetricsStateType {
  [id: number]: MetricType;
}

export interface MetricsSliceStateType
  extends RequestMetaStateType<
    MetricType['id'] | MetricType['name'] | MetricType['abbreviation'] | GetMetricsParamsType
  > {
  metrics: MetricsStateType;
}
