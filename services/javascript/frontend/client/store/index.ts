/** @format */

import { combineReducers, configureStore } from '@reduxjs/toolkit';
import { useDispatch as _useDispatch, useSelector as _useSelector } from 'react-redux';
import { LocationsSlice } from './iot/locations/slice';
import { MetricsSlice } from './iot/metrics/slice';
import { themeSlice } from './theme/slice';

export const store = configureStore({
  reducer: {
    theme: themeSlice.reducer,
    iot: combineReducers({
      locations: LocationsSlice.reducer,
      metrics: MetricsSlice.reducer,
    }),
  },
});

export type RootState = ReturnType<typeof store.getState>;
export type Dispatch = typeof store.dispatch;
export const useDispatch = () => _useDispatch<Dispatch>();
export const useSelector = <Selected = unknown>(
  selector: (state: RootState) => Selected,
  equalityFn?: (left: Selected, right: Selected) => boolean
): Selected => _useSelector<RootState, Selected>(selector, equalityFn);
