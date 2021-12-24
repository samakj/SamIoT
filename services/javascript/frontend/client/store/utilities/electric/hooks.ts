/** @format */

import { RootState, useDispatch, useSelector } from 'client/store';
import { useEffect, useMemo, useState } from 'react';
import { GetUtilitiesConsumptionsParamsType } from 'shared/javascript/clients/utilities/types';
import { UtilitiesConsumptionType } from 'shared/javascript/types/utilities';
import { getElectricConsumption, getElectricConsumptions } from './thunks';
import { ElectricConsumptionStateType } from './types';

type UseElectricConsumptionType = {
  (id?: UtilitiesConsumptionType['id'] | null): UtilitiesConsumptionType | undefined;
};

export const useElectricConsumption: UseElectricConsumptionType = (identifier) => {
  const [id, setId] = useState<UtilitiesConsumptionType['id']>(
    typeof identifier === 'number' ? identifier : undefined
  );
  const dispatch = useDispatch();
  const consumption = useSelector((state) =>
    id ? state.utilities.electric.consumption[id] : undefined
  );

  useEffect(() => {
    if (consumption) return;
    if (identifier == null) return;
    if (typeof identifier === 'number')
      dispatch(getElectricConsumption(identifier)).then(
        (action) =>
          action.meta.requestStatus === 'fulfilled' &&
          // @ts-ignore
          setId(action.payload?.id)
      );
  }, [identifier]);

  return { ...consumption, timestamp: consumption.timestamp && new Date(consumption.timestamp) };
};

export const useElectricConsumptions = (
  filters?: GetUtilitiesConsumptionsParamsType | null
): ElectricConsumptionStateType => {
  const [ids, setIds] = useState<UtilitiesConsumptionType['id'][]>();
  const dispatch = useDispatch();
  const allElectricConsumptions = useSelector((state) => state.utilities.electric.consumption);

  useEffect(() => {
    if (filters == null) return;
    dispatch(getElectricConsumptions(filters)).then(
      (action) =>
        action.meta.requestStatus === 'fulfilled' &&
        // @ts-ignore
        setIds(action.payload?.map((electricconsumption) => electricconsumption.id))
    );
  }, [filters]);

  const consumptions = useMemo(
    (): ElectricConsumptionStateType =>
      ids &&
      ids.reduce(
        (acc, id) => ({
          ...acc,
          [id]: {
            ...allElectricConsumptions[id],
            timestamp:
              allElectricConsumptions[id].timestamp &&
              new Date(allElectricConsumptions[id].timestamp),
          },
        }),
        {}
      ),
    [ids]
  );

  return consumptions;
};
