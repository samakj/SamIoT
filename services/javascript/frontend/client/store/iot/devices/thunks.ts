/** @format */

import { createAsyncThunk } from '@reduxjs/toolkit';
import { IoTClient } from 'client/clients/iot';
import { GetDevicesParamsType } from 'shared/javascript/clients/iot/devices/types';
import { DeviceType } from 'shared/javascript/types/iot';

export const getDevice = createAsyncThunk('iot/device', async (id: DeviceType['id']) => {
  const response = await IoTClient.devices.getDevice(id);
  return IoTClient.devices.toDeviceType(response.data);
});

export const getDeviceByIp = createAsyncThunk('iot/device/ip', async (ip: DeviceType['ip']) => {
  const response = await IoTClient.devices.getDeviceByIp(ip);
  return IoTClient.devices.toDeviceType(response.data);
});

export const getDeviceByMac = createAsyncThunk('iot/device/mac', async (mac: DeviceType['mac']) => {
  const response = await IoTClient.devices.getDeviceByMac(mac);
  return IoTClient.devices.toDeviceType(response.data);
});

export const getDevices = createAsyncThunk(
  'iot/devices',
  async (filters?: GetDevicesParamsType) => {
    const response = await IoTClient.devices.getDevices(filters);
    return response.data.map(IoTClient.devices.toDeviceType);
  }
);
