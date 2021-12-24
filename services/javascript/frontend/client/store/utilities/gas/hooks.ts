/** @format */

import { RootState, useDispatch, useSelector } from 'client/store';
import { useEffect, useMemo, useState } from 'react';
import { GetUtilitiesConsumptionsParamsType } from 'shared/javascript/clients/utilities/types';
import { UtilitiesConsumptionType } from 'shared/javascript/types/utilities';
import { getGasConsumption, getGasConsumptions } from './thunks';
import { GasConsumptionStateType } from './types';

type UseGasConsumptionType = {
  (id: UtilitiesConsumptionType['id']): UtilitiesConsumptionType | undefined;
};

export const useGasConsumption: UseGasConsumptionType = (identifier) => {
  const [id, setId] = useState<UtilitiesConsumptionType['id']>(
    typeof identifier === 'number' ? identifier : undefined
  );
  const dispatch = useDispatch();
  const consumption = useSelector((state) =>
    id ? state.utilities.gas.consumption[id] : undefined
  );

  useEffect(() => {
    if (consumption) return;
    if (typeof identifier === 'number')
      dispatch(getGasConsumption(identifier)).then(
        (action) =>
          action.meta.requestStatus === 'fulfilled' &&
          // @ts-ignore
          setId(action.payload?.id)
      );
  }, [identifier]);

  return { ...consumption, timestamp: consumption.timestamp && new Date(consumption.timestamp) };
};

export const useGasConsumptions = (
  filters?: GetUtilitiesConsumptionsParamsType
): GasConsumptionStateType => {
  const [ids, setIds] = useState<UtilitiesConsumptionType['id'][]>();
  const dispatch = useDispatch();
  const allGasConsumptions = useSelector((state) => state.utilities.gas.consumption);

  useEffect(() => {
    dispatch(getGasConsumptions(filters)).then(
      (action) =>
        action.meta.requestStatus === 'fulfilled' &&
        // @ts-ignore
        setIds(action.payload?.map((gasconsumption) => gasconsumption.id))
    );
  }, [filters]);

  const consumptions = useMemo(
    (): GasConsumptionStateType =>
      ids &&
      ids.reduce(
        (acc, id) => ({
          ...acc,
          [id]: {
            ...allGasConsumptions[id],
            timestamp:
              allGasConsumptions[id].timestamp && new Date(allGasConsumptions[id].timestamp),
          },
        }),
        {}
      ),
    [ids]
  );

  return consumptions;
};
