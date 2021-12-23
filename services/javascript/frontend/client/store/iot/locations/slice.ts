/** @format */

import { createSlice } from '@reduxjs/toolkit';
import { handleRequestMeta } from 'client/store/handle-request-meta';
import { getLocation, getLocationByName, getLocations } from './thunks';
import { LocationsSliceStateType } from './types';

export const initialState: LocationsSliceStateType = {
  requests: {},
  locations: {},
};

export const LocationsSlice = createSlice({
  name: 'locations',
  initialState,
  reducers: {},
  extraReducers: (builder) => {
    builder.addCase(getLocation.pending, handleRequestMeta);
    builder.addCase(getLocation.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      state.locations[action.payload.id] = action.payload;
    });
    builder.addCase(getLocation.rejected, handleRequestMeta);

    builder.addCase(getLocationByName.pending, handleRequestMeta);
    builder.addCase(getLocationByName.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      state.locations[action.payload.id] = action.payload;
    });
    builder.addCase(getLocationByName.rejected, handleRequestMeta);

    builder.addCase(getLocations.pending, handleRequestMeta);
    builder.addCase(getLocations.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      action.payload.forEach((location) => (state.locations[location.id] = location));
    });
    builder.addCase(getLocations.rejected, handleRequestMeta);
  },
});
