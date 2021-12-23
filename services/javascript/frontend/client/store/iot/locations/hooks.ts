/** @format */

import { RootState, useDispatch, useSelector } from 'client/store';
import { useEffect, useMemo, useState } from 'react';
import { GetLocationsParamsType } from 'shared/javascript/clients/iot/locations/types';
import { LocationType } from 'shared/javascript/types/iot';
import { getLocation, getLocationByName, getLocations } from './thunks';
import { LocationsStateType } from './types';

type UseLocationType = {
  (id: LocationType['id']): LocationType | undefined;
  (name: LocationType['name']): LocationType | undefined;
};

export const useLocation: UseLocationType = (identifier) => {
  const [id, setId] = useState<LocationType['id']>(
    typeof identifier === 'number' ? identifier : undefined
  );
  const dispatch = useDispatch();
  const location = useSelector((state) => (id ? state.iot.locations.locations[id] : undefined));

  useEffect(() => {
    if (location) return;
    if (typeof identifier === 'number')
      dispatch(getLocation(identifier)).then(
        (action) =>
          action.meta.requestStatus === 'fulfilled' &&
          // @ts-ignore
          setId(action.payload?.id)
      );
    if (typeof identifier === 'string')
      dispatch(getLocationByName(identifier)).then(
        (action) =>
          action.meta.requestStatus === 'fulfilled' &&
          // @ts-ignore
          setId(action.payload?.id)
      );
  }, [identifier]);

  return location;
};

export const useLocations = (filters?: GetLocationsParamsType): LocationsStateType => {
  const [ids, setIds] = useState<LocationType['id'][]>();
  const dispatch = useDispatch();
  const allLocations = useSelector((state) => state.iot.locations.locations);

  useEffect(() => {
    dispatch(getLocations(filters)).then(
      (action) =>
        action.meta.requestStatus === 'fulfilled' &&
        // @ts-ignore
        setIds(action.payload?.map((location) => location.id))
    );
  }, [filters]);

  const locations = useMemo(
    (): LocationsStateType =>
      ids && ids.reduce((acc, id) => ({ ...acc, [id]: allLocations[id] }), {}),
    [ids]
  );

  return locations;
};
