/** @format */

import { RequestMetaStateType } from 'client/store/types';
import { GetLocationsParamsType } from 'shared/javascript/clients/iot/locations/types';
import { LocationType } from 'shared/javascript/types/iot';

export interface LocationsStateType {
  [id: number]: LocationType;
}

export interface LocationsSliceStateType
  extends RequestMetaStateType<LocationType['id'] | LocationType['name'] | GetLocationsParamsType> {
  locations: LocationsStateType;
}
