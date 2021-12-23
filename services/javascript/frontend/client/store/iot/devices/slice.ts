/** @format */

import { createSlice } from '@reduxjs/toolkit';
import { handleRequestMeta } from 'client/store/handle-request-meta';
import { getDevice, getDeviceByMac, getDeviceByIp, getDevices } from './thunks';
import { DevicesSliceStateType } from './types';

export const initialState: DevicesSliceStateType = {
  requests: {},
  devices: {},
};

export const DevicesSlice = createSlice({
  name: 'devices',
  initialState,
  reducers: {},
  extraReducers: (builder) => {
    builder.addCase(getDevice.pending, handleRequestMeta);
    builder.addCase(getDevice.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      state.devices[action.payload.id] = action.payload;
    });
    builder.addCase(getDevice.rejected, handleRequestMeta);

    builder.addCase(getDeviceByIp.pending, handleRequestMeta);
    builder.addCase(getDeviceByIp.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      state.devices[action.payload.id] = action.payload;
    });
    builder.addCase(getDeviceByIp.rejected, handleRequestMeta);

    builder.addCase(getDeviceByMac.pending, handleRequestMeta);
    builder.addCase(getDeviceByMac.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      state.devices[action.payload.id] = action.payload;
    });
    builder.addCase(getDeviceByMac.rejected, handleRequestMeta);

    builder.addCase(getDevices.pending, handleRequestMeta);
    builder.addCase(getDevices.fulfilled, (state, action) => {
      handleRequestMeta(state, action);
      action.payload.forEach((device) => (state.devices[device.id] = device));
    });
    builder.addCase(getDevices.rejected, handleRequestMeta);
  },
});
