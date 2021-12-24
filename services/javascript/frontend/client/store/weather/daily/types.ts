/** @format */

import { RequestMetaStateType } from 'client/store/types';
import { GetDailyWeathersParamsType } from 'shared/javascript/clients/weather/daily/types';
import { DailyWeatherType } from 'shared/javascript/types/weather';

export interface SerialisedDailyWeatherType
  extends Omit<DailyWeatherType, 'timestamp' | 'sunrise' | 'sunset' | 'moonrise' | 'moonset'> {
  timestamp?: string;
  sunrise?: string;
  sunset?: string;
  moonrise?: string;
  moonset?: string;
}

export interface DailyWeatherStateType {
  [id: number]: SerialisedDailyWeatherType;
}

export interface DailyWeatherSliceStateType
  extends RequestMetaStateType<
    DailyWeatherType['id'] | DailyWeatherType['timestamp'] | GetDailyWeathersParamsType
  > {
  daily: DailyWeatherStateType;
}
