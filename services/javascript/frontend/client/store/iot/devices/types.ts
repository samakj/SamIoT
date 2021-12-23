/** @format */

import { RequestMetaStateType } from 'client/store/types';
import { GetDevicesParamsType } from 'shared/javascript/clients/iot/devices/types';
import { DeviceType } from 'shared/javascript/types/iot';

export interface SerialisedDeviceType extends Omit<DeviceType, 'lastMessage'> {
  lastMessage?: string;
}

export interface DevicesStateType {
  [id: number]: SerialisedDeviceType;
}

export interface DevicesSliceStateType
  extends RequestMetaStateType<
    DeviceType['id'] | DeviceType['ip'] | DeviceType['mac'] | GetDevicesParamsType
  > {
  devices: DevicesStateType;
}
