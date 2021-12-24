/** @format */

import { RootState, useDispatch, useSelector } from 'client/store';
import { useEffect } from 'react';
import { CurrentWeatherType } from 'shared/javascript/types/weather';
import { getCurrentWeather } from './thunks';

type UseCurrentWeatherType = {
  (): CurrentWeatherType | undefined;
};

export const useCurrentWeather: UseCurrentWeatherType = () => {
  const dispatch = useDispatch();
  const current = useSelector((state) => state.weather.current.current);

  useEffect(() => {
    if (current) return;
    dispatch(getCurrentWeather());
  }, []);

  return {
    ...current,
    timestamp: new Date(current.timestamp),
    sunrise: new Date(current.sunrise),
    sunset: new Date(current.sunset),
  };
};
