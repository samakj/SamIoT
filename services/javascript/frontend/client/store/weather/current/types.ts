/** @format */

import { RequestMetaStateType } from 'client/store/types';
import { CurrentWeatherType } from 'shared/javascript/types/weather';

export interface SerialisedCurrentWeatherType
  extends Omit<CurrentWeatherType, 'timestamp' | 'sunrise' | 'sunset'> {
  timestamp: string;
  sunrise: string;
  sunset: string;
}

export interface CurrentWeathersSliceStateType extends RequestMetaStateType<null> {
  current?: SerialisedCurrentWeatherType;
}
