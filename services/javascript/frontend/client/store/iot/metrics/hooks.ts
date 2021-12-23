/** @format */

import { RootState, useDispatch, useSelector } from 'client/store';
import { useEffect, useMemo, useState } from 'react';
import { GetMetricsParamsType } from 'shared/javascript/clients/iot/metrics/types';
import { MetricType } from 'shared/javascript/types/iot';
import { getMetric, getMetricByAbbreviation, getMetricByName, getMetrics } from './thunks';
import { MetricsStateType } from './types';

type UseMetricType = {
  (id: MetricType['id']): MetricType | undefined;
  (name: MetricType['name']): MetricType | undefined;
  (name: MetricType['abbreviation']): MetricType | undefined;
};

export const useMetric: UseMetricType = (identifier) => {
  const [id, setId] = useState<MetricType['id']>(
    typeof identifier === 'number' ? identifier : undefined
  );
  const dispatch = useDispatch();
  const metric = useSelector((state) => (id ? state.iot.metrics.metrics[id] : undefined));

  useEffect(() => {
    if (metric) return;
    if (typeof identifier === 'number')
      dispatch(getMetric(identifier)).then(
        (action) =>
          action.meta.requestStatus === 'fulfilled' &&
          // @ts-ignore
          setId(action.payload?.id)
      );
    if (typeof identifier === 'string')
      dispatch(getMetricByName(identifier)).then((action) =>
        action.meta.requestStatus === 'fulfilled'
          ? // @ts-ignore
            setId(action.payload?.id)
          : dispatch(getMetricByAbbreviation(identifier)).then(
              (action) =>
                action.meta.requestStatus === 'fulfilled' &&
                // @ts-ignore
                setId(action.payload?.id)
            )
      );
  }, [identifier]);

  return metric;
};

export const useMetrics = (filters?: GetMetricsParamsType): MetricsStateType => {
  const [ids, setIds] = useState<MetricType['id'][]>();
  const dispatch = useDispatch();
  const allMetrics = useSelector((state) => state.iot.metrics.metrics);

  useEffect(() => {
    dispatch(getMetrics(filters)).then(
      (action) =>
        action.meta.requestStatus === 'fulfilled' &&
        // @ts-ignore
        setIds(action.payload?.map((metric) => metric.id))
    );
  }, [filters]);

  const metrics = useMemo(
    (): MetricsStateType => ids && ids.reduce((acc, id) => ({ ...acc, [id]: allMetrics[id] }), {}),
    [ids]
  );

  return metrics;
};
