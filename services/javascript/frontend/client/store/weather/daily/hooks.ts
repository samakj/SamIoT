/** @format */

import { RootState, useDispatch, useSelector } from 'client/store';
import { useEffect, useMemo, useState } from 'react';
import { GetDailyWeathersParamsType } from 'shared/javascript/clients/weather/daily/types';
import { DailyWeatherType } from 'shared/javascript/types/weather';
import { getDailyWeather, getDailyWeatherByTimestamp, getDailyWeathers } from './thunks';
import { DailyWeatherStateType } from './types';

type UseDailyWeatherType = {
  (id?: DailyWeatherType['id'] | null): DailyWeatherType | undefined;
  (timestamp?: DailyWeatherType['timestamp'] | null): DailyWeatherType | undefined;
};

export const useDailyWeather: UseDailyWeatherType = (identifier) => {
  const [id, setId] = useState<DailyWeatherType['id']>(
    typeof identifier === 'number' ? identifier : undefined
  );
  const dispatch = useDispatch();
  const daily = useSelector((state) => (id ? state.weather.daily.daily[id] : undefined));

  useEffect(() => {
    if (daily) return;
    if (identifier == null) return;
    if (typeof identifier === 'number')
      dispatch(getDailyWeather(identifier)).then(
        (action) =>
          action.meta.requestStatus === 'fulfilled' &&
          // @ts-ignore
          setId(action.payload?.id)
      );
    if (identifier instanceof Date) {
      dispatch(getDailyWeatherByTimestamp(identifier)).then(
        (action) =>
          action.meta.requestStatus === 'fulfilled' &&
          // @ts-ignore
          setId(action.payload?.id)
      );
    }
  }, [identifier]);

  return {
    ...daily,
    timestamp: new Date(daily.timestamp),
    sunrise: new Date(daily.sunrise),
    sunset: new Date(daily.sunset),
    moonrise: new Date(daily.moonrise),
    moonset: new Date(daily.moonset),
  };
};

export const useDailyWeathers = (
  filters?: GetDailyWeathersParamsType | null
): DailyWeatherStateType => {
  const [ids, setIds] = useState<DailyWeatherType['id'][]>();
  const dispatch = useDispatch();
  const allDailyWeathers = useSelector((state) => state.weather.daily.daily);

  useEffect(() => {
    if (filters == null) return;
    dispatch(getDailyWeathers(filters)).then(
      (action) =>
        action.meta.requestStatus === 'fulfilled' &&
        // @ts-ignore
        setIds(action.payload?.map((daily) => daily.id))
    );
  }, [filters]);

  const daily = useMemo(
    (): DailyWeatherStateType =>
      ids &&
      ids.reduce(
        (acc, id) => ({
          ...acc,
          [id]: {
            ...allDailyWeathers[id],
            timestamp: new Date(allDailyWeathers[id].timestamp),
            sunrise: new Date(allDailyWeathers[id].sunrise),
            sunset: new Date(allDailyWeathers[id].sunset),
            moonrise: new Date(allDailyWeathers[id].moonrise),
            moonset: new Date(allDailyWeathers[id].moonset),
          },
        }),
        {}
      ),
    [ids]
  );

  return daily;
};
