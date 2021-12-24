/** @format */

import { RootState, useDispatch, useSelector } from 'client/store';
import { useEffect, useMemo, useState } from 'react';
import {
  GetAverageMeasurementsParamsType,
  GetLatestMeasurementsParamsType,
  GetMeasurementsParamsType,
} from 'shared/javascript/clients/iot/measurements/types';
import { MeasurementType } from 'shared/javascript/types/iot';
import { getLatestMeasurementsKey, getAverageMeasurementsKey } from './slice';
import {
  getAverageMeasurements,
  getLatestMeasurements,
  getMeasurement,
  getMeasurements,
} from './thunks';
import { MeasurementsStateType } from './types';

type UseMeasurementType = {
  (id?: MeasurementType['id'] | null): MeasurementType | undefined;
};

export const useMeasurement: UseMeasurementType = (identifier) => {
  const [id, setId] = useState<MeasurementType['id']>(
    typeof identifier === 'number' ? identifier : undefined
  );
  const dispatch = useDispatch();
  const measurement = useSelector((state) =>
    id ? state.iot.measurements.measurements[id] : undefined
  );

  useEffect(() => {
    if (measurement) return;
    if (identifier == null) return;
    if (typeof identifier === 'number')
      dispatch(getMeasurement(identifier)).then(
        (action) =>
          action.meta.requestStatus === 'fulfilled' &&
          // @ts-ignore
          setId(action.payload?.id)
      );
  }, [identifier]);

  return (
    measurement && {
      ...measurement,
      timestamp: measurement.timestamp && new Date(measurement.timestamp),
    }
  );
};

export const useMeasurements = (
  filters?: GetMeasurementsParamsType | null
): MeasurementsStateType => {
  const [ids, setIds] = useState<MeasurementType['id'][]>();
  const dispatch = useDispatch();
  const allMeasurements = useSelector((state) => state.iot.measurements.measurements);

  useEffect(() => {
    if (filters == null) return;
    dispatch(getMeasurements(filters)).then(
      (action) =>
        action.meta.requestStatus === 'fulfilled' &&
        // @ts-ignore
        setIds(action.payload?.map((measurement) => measurement.id))
    );
  }, [filters]);

  const measurements = useMemo(
    (): MeasurementsStateType =>
      ids &&
      ids.reduce(
        (acc, id) => ({
          ...acc,
          [id]: {
            ...allMeasurements[id],
            timestamp: allMeasurements[id].timestamp && new Date(allMeasurements[id].timestamp),
          },
        }),
        {}
      ),
    [ids]
  );

  return measurements;
};

export const useLatestMeasurements = (
  filters?: GetLatestMeasurementsParamsType | null
): MeasurementsStateType => {
  const [keys, setKeys] = useState<string[]>();
  const dispatch = useDispatch();
  const allLatest = useSelector((state) => state.iot.measurements.latest);

  useEffect(() => {
    if (filters == null) return;
    dispatch(getLatestMeasurements(filters)).then(
      (action) =>
        action.meta.requestStatus === 'fulfilled' &&
        // @ts-ignore
        setKeys(action.payload?.map(getLatestMeasurementsKey))
    );
  }, [filters]);

  const measurements = useMemo(
    (): MeasurementsStateType =>
      keys &&
      keys.reduce(
        (acc, key) => ({
          ...acc,
          [key]: {
            ...allLatest[key],
            timestamp: allLatest[key].timestamp && new Date(allLatest[key].timestamp),
          },
        }),
        {}
      ),
    [keys]
  );

  return measurements;
};
export const useAverageMeasurements = (
  locationId?: MeasurementType['locationId'] | null,
  metricId?: MeasurementType['metricId'] | null,
  tags?: MeasurementType['tags'] | null,
  filters?: GetAverageMeasurementsParamsType | null
): MeasurementsStateType => {
  const [keys, setKeys] = useState<string[]>();
  const dispatch = useDispatch();
  const allAverage = useSelector((state) => state.iot.measurements.averages);

  useEffect(() => {
    if (locationId == null || metricId == null || tags == null || filters == null) return;
    dispatch(getAverageMeasurements({ ...filters, locationId, metricId, tags })).then(
      (action) =>
        action.meta.requestStatus === 'fulfilled' &&
        // @ts-ignore
        setKeys(action.payload?.map(getAverageMeasurementsKey))
    );
  }, [filters]);

  const measurements = useMemo(
    (): MeasurementsStateType =>
      keys &&
      keys.reduce(
        (acc, key) => ({
          ...acc,
          [key]: {
            ...allAverage[key],
            start: new Date(allAverage[key].start),
            end: new Date(allAverage[key].end),
          },
        }),
        {}
      ),
    [keys]
  );

  return measurements;
};
