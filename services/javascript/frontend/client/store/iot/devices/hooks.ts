/** @format */

import { RootState, useDispatch, useSelector } from 'client/store';
import { useEffect, useMemo, useState } from 'react';
import { GetDevicesParamsType } from 'shared/javascript/clients/iot/devices/types';
import { DeviceType } from 'shared/javascript/types/iot';
import { getDevice, getDeviceByIp, getDeviceByMac, getDevices } from './thunks';
import { DevicesStateType } from './types';

type UseDeviceType = {
  (id: DeviceType['id']): DeviceType | undefined;
  (name: DeviceType['ip']): DeviceType | undefined;
  (name: DeviceType['mac']): DeviceType | undefined;
};

export const useDevice: UseDeviceType = (identifier) => {
  const [id, setId] = useState<DeviceType['id']>(
    typeof identifier === 'number' ? identifier : undefined
  );
  const dispatch = useDispatch();
  const device = useSelector((state) => (id ? state.iot.devices.devices[id] : undefined));

  useEffect(() => {
    if (device) return;
    if (typeof identifier === 'number')
      dispatch(getDevice(identifier)).then(
        (action) =>
          action.meta.requestStatus === 'fulfilled' &&
          // @ts-ignore
          setId(action.payload?.id)
      );
    if (typeof identifier === 'string') {
      if (identifier.includes('.'))
        dispatch(getDeviceByIp(identifier)).then(
          (action) =>
            action.meta.requestStatus === 'fulfilled' &&
            // @ts-ignore
            setId(action.payload?.id)
        );
      if (identifier.includes(':'))
        dispatch(getDeviceByMac(identifier)).then(
          (action) =>
            action.meta.requestStatus === 'fulfilled' &&
            // @ts-ignore
            setId(action.payload?.id)
        );
    }
  }, [identifier]);

  return { ...device, lastMessage: device.lastMessage && new Date(device.lastMessage) };
};

export const useDevices = (filters?: GetDevicesParamsType): DevicesStateType => {
  const [ids, setIds] = useState<DeviceType['id'][]>();
  const dispatch = useDispatch();
  const allDevices = useSelector((state) => state.iot.devices.devices);

  useEffect(() => {
    dispatch(getDevices(filters)).then(
      (action) =>
        action.meta.requestStatus === 'fulfilled' &&
        // @ts-ignore
        setIds(action.payload?.map((device) => device.id))
    );
  }, [filters]);

  const devices = useMemo(
    (): DevicesStateType =>
      ids &&
      ids.reduce(
        (acc, id) => ({
          ...acc,
          [id]: {
            ...allDevices[id],
            lastMessage: allDevices[id].lastMessage && new Date(allDevices[id].lastMessage),
          },
        }),
        {}
      ),
    [ids]
  );

  return devices;
};
