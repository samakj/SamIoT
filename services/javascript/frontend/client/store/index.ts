/** @format */

import { combineReducers, configureStore } from '@reduxjs/toolkit';
import { useDispatch as _useDispatch, useSelector as _useSelector } from 'react-redux';
import { DevicesSlice } from './iot/devices/slice';
import { LocationsSlice } from './iot/locations/slice';
import { MeasurementsSlice } from './iot/measurements/slice';
import { MetricsSlice } from './iot/metrics/slice';
import { themeSlice } from './theme/slice';
import { ElectricSlice } from './utilities/electric/slice';
import { GasSlice } from './utilities/gas/slice';
import { CurrentWeatherSlice } from './weather/current/slice';
import { DailyWeatherSlice } from './weather/daily/slice';
import { HourlyWeatherSlice } from './weather/hourly/slice';

export const createStore = <S>(preloadedState: S) =>
  configureStore({
    reducer: {
      theme: themeSlice.reducer,
      iot: combineReducers({
        devices: DevicesSlice.reducer,
        locations: LocationsSlice.reducer,
        measurements: MeasurementsSlice.reducer,
        metrics: MetricsSlice.reducer,
      }),
      utilities: combineReducers({
        gas: GasSlice.reducer,
        electric: ElectricSlice.reducer,
      }),
      weather: combineReducers({
        current: CurrentWeatherSlice.reducer,
        hourly: HourlyWeatherSlice.reducer,
        daily: DailyWeatherSlice.reducer,
      }),
    },
    preloadedState,
  });

export type RootState = ReturnType<ReturnType<typeof createStore>['getState']>;
export type Dispatch = ReturnType<typeof createStore>['dispatch'];
export const useDispatch = () => _useDispatch<Dispatch>();
export const useSelector = <Selected = unknown>(
  selector: (state: RootState) => Selected,
  equalityFn?: (left: Selected, right: Selected) => boolean
): Selected => _useSelector<RootState, Selected>(selector, equalityFn);
