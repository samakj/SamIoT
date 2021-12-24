/** @format */

import { RequestMetaStateType } from 'client/store/types';
import { GetHourlyWeathersParamsType } from 'shared/javascript/clients/weather/hourly/types';
import { HourlyWeatherType } from 'shared/javascript/types/weather';

export interface SerialisedHourlyWeatherType extends Omit<HourlyWeatherType, 'timestamp'> {
  timestamp?: string;
}

export interface HourlyWeatherStateType {
  [id: number]: SerialisedHourlyWeatherType;
}

export interface HourlyWeatherSliceStateType
  extends RequestMetaStateType<
    HourlyWeatherType['id'] | HourlyWeatherType['timestamp'] | GetHourlyWeathersParamsType
  > {
  hourly: HourlyWeatherStateType;
}
