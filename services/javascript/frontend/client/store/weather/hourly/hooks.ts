/** @format */

import { RootState, useDispatch, useSelector } from 'client/store';
import { useEffect, useMemo, useState } from 'react';
import { GetHourlyWeathersParamsType } from 'shared/javascript/clients/weather/hourly/types';
import { HourlyWeatherType } from 'shared/javascript/types/weather';
import { getHourlyWeather, getHourlyWeatherByTimestamp, getHourlyWeathers } from './thunks';
import { HourlyWeatherStateType } from './types';

type UseHourlyWeatherType = {
  (id: HourlyWeatherType['id']): HourlyWeatherType | undefined;
  (name: HourlyWeatherType['timestamp']): HourlyWeatherType | undefined;
};

export const useHourlyWeather: UseHourlyWeatherType = (identifier) => {
  const [id, setId] = useState<HourlyWeatherType['id']>(
    typeof identifier === 'number' ? identifier : undefined
  );
  const dispatch = useDispatch();
  const hourly = useSelector((state) => (id ? state.weather.hourly.hourly[id] : undefined));

  useEffect(() => {
    if (hourly) return;
    if (typeof identifier === 'number')
      dispatch(getHourlyWeather(identifier)).then(
        (action) =>
          action.meta.requestStatus === 'fulfilled' &&
          // @ts-ignore
          setId(action.payload?.id)
      );
    if (identifier instanceof Date) {
      dispatch(getHourlyWeatherByTimestamp(identifier)).then(
        (action) =>
          action.meta.requestStatus === 'fulfilled' &&
          // @ts-ignore
          setId(action.payload?.id)
      );
    }
  }, [identifier]);

  return { ...hourly, timestamp: new Date(hourly.timestamp) };
};

export const useHourlyWeathers = (
  filters?: GetHourlyWeathersParamsType
): HourlyWeatherStateType => {
  const [ids, setIds] = useState<HourlyWeatherType['id'][]>();
  const dispatch = useDispatch();
  const allHourlyWeathers = useSelector((state) => state.weather.hourly.hourly);

  useEffect(() => {
    dispatch(getHourlyWeathers(filters)).then(
      (action) =>
        action.meta.requestStatus === 'fulfilled' &&
        // @ts-ignore
        setIds(action.payload?.map((hourly) => hourly.id))
    );
  }, [filters]);

  const hourly = useMemo(
    (): HourlyWeatherStateType =>
      ids &&
      ids.reduce(
        (acc, id) => ({
          ...acc,
          [id]: {
            ...allHourlyWeathers[id],
            timestamp: new Date(allHourlyWeathers[id].timestamp),
          },
        }),
        {}
      ),
    [ids]
  );

  return hourly;
};
