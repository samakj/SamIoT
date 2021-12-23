import { APIClient } from "../../../extensions/axios/api-client";
import { DeviceType, isDeviceType } from "../../../types/iot";
import { GetDevicesAPIParamsType, GetDevicesParamsType } from "./types";
import { APIDeviceType, isAPIDeviceType } from "./types";

export class DevicesClient extends APIClient {
  constructor(host: string, port: number | string | null, ssl: boolean) {
    super(host, port, ssl);
  }

  static toAPIDeviceType(device: DeviceType | APIDeviceType): APIDeviceType {
    if (isAPIDeviceType(device)) return { ...device };
    return {
      id: device.id,
      mac: device.mac,
      ip: device.ip,
      websocket_path: device.websocketPath,
      location_id: device.locationId,
      last_message: device.lastMessage.toISOString(),
    };
  }

  static toDeviceType(device: DeviceType | APIDeviceType): DeviceType {
    if (isDeviceType(device)) return { ...device };
    return {
      id: device.id,
      mac: device.mac,
      ip: device.ip,
      websocketPath: device.websocket_path,
      locationId: device.location_id,
      lastMessage: new Date(device.last_message),
    };
  }

  getDevice(id: number) {
    return this.get<APIDeviceType>(`/v0/devices/${id}`);
  }

  getDeviceByIp(ip: string) {
    return this.get<APIDeviceType>(`/v0/devices/ip/${ip}`);
  }

  getDeviceByMac(mac: string) {
    return this.get<APIDeviceType>(`/v0/devices/mac/${mac}`);
  }

  getDevices(params?: GetDevicesParamsType) {
    const _params: GetDevicesAPIParamsType = {};

    if (params?.id != null) _params.id = params.id;
    if (params?.mac != null) _params.mac = params.mac;
    if (params?.ip != null) _params.ip = params.ip;
    if (params?.locationId != null) _params.location_id = params.locationId;
    if (params?.lastMessageGte != null)
      _params.last_message_gte = params.lastMessageGte.toISOString();
    if (params?.lastMessageLte != null)
      _params.last_message_lte = params.lastMessageLte.toISOString();

    return this.get<APIDeviceType[]>(`/v0/devices/`, { params: _params });
  }

  updateDevice(device: DeviceType) {
    return this.patch(`/v0/devices/${device.id}`, device);
  }
}
