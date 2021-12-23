/** @format */

import { combineReducers, configureStore } from '@reduxjs/toolkit';
import { LocationsSlice } from './iot/locations/slice';
import { themeSlice } from './theme/slice';

export const store = configureStore({
  reducer: {
    theme: themeSlice.reducer,
    iot: combineReducers({
      locations: LocationsSlice.reducer,
    }),
  },
});

export type RootState = ReturnType<typeof store.getState>;
export type AppDispatch = typeof store.dispatch;
