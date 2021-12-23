/** @format */

import { createAsyncThunk } from '@reduxjs/toolkit';
import { IoTClient } from 'client/clients/iot';
import { GetDevicesParamsType } from 'shared/javascript/clients/iot/devices/types';
import { DeviceType } from 'shared/javascript/types/iot';

export const getDevice = createAsyncThunk('iot/device', async (id: DeviceType['id']) => {
  const response = await IoTClient.devices.getDevice(id);
  const device = IoTClient.devices.toDeviceType(response.data);
  return {
    ...device,
    lastMessage: device.lastMessage?.toISOString(),
  };
});

export const getDeviceByIp = createAsyncThunk('iot/device/ip', async (ip: DeviceType['ip']) => {
  const response = await IoTClient.devices.getDeviceByIp(ip);
  const device = IoTClient.devices.toDeviceType(response.data);
  return {
    ...device,
    lastMessage: device.lastMessage?.toISOString(),
  };
});

export const getDeviceByMac = createAsyncThunk('iot/device/mac', async (mac: DeviceType['mac']) => {
  const response = await IoTClient.devices.getDeviceByMac(mac);
  const device = IoTClient.devices.toDeviceType(response.data);
  return {
    ...device,
    lastMessage: device.lastMessage?.toISOString(),
  };
});

export const getDevices = createAsyncThunk(
  'iot/devices',
  async (filters?: GetDevicesParamsType) => {
    const response = await IoTClient.devices.getDevices(filters);
    return response.data.map((apiDevice) => {
      const device = IoTClient.devices.toDeviceType(apiDevice);
      return {
        ...device,
        lastMessage: device.lastMessage?.toISOString(),
      };
    });
  }
);
