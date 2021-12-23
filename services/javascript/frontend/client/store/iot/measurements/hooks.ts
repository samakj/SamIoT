/** @format */

import { RootState, useDispatch, useSelector } from 'client/store';
import { useEffect, useMemo, useState } from 'react';
import { GetMeasurementsParamsType } from 'shared/javascript/clients/iot/measurements/types';
import { MeasurementType } from 'shared/javascript/types/iot';
import { getMeasurement, getMeasurements } from './thunks';
import { MeasurementsStateType } from './types';

type UseMeasurementType = {
  (id: MeasurementType['id']): MeasurementType | undefined;
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
    if (typeof identifier === 'number')
      dispatch(getMeasurement(identifier)).then(
        (action) =>
          action.meta.requestStatus === 'fulfilled' &&
          // @ts-ignore
          setId(action.payload?.id)
      );
  }, [identifier]);

  return { ...measurement, timestamp: measurement.timestamp && new Date(measurement.timestamp) };
};

export const useMeasurements = (filters?: GetMeasurementsParamsType): MeasurementsStateType => {
  const [ids, setIds] = useState<MeasurementType['id'][]>();
  const dispatch = useDispatch();
  const allMeasurements = useSelector((state) => state.iot.measurements.measurements);

  useEffect(() => {
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
