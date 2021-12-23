import { DeviceType } from "../../../types/iot";
import { APILocationType } from "../locations/types";

export interface APIDeviceType {
  id: number;
  mac: string;
  ip: string;
  websocket_path: string;
  location_id: APILocationType["id"];
  last_message?: string;
}

export const isAPIDeviceType = (o: any): o is APIDeviceType =>
  o.id != null &&
  o.mac != null &&
  o.ip != null &&
  o.websocket_path != null &&
  o.location_id != null;

export interface GetDevicesParamsType {
  id?: DeviceType["id"];
  mac?: DeviceType["mac"];
  ip?: DeviceType["ip"];
  locationId?: DeviceType["locationId"];
  lastMessageGte?: DeviceType["lastMessage"];
  lastMessageLte?: DeviceType["lastMessage"];
}
export interface GetDevicesAPIParamsType {
  id?: APIDeviceType["id"];
  mac?: APIDeviceType["mac"];
  ip?: APIDeviceType["ip"];
  location_id?: APIDeviceType["location_id"];
  last_message_gte?: APIDeviceType["last_message"];
  last_message_lte?: APIDeviceType["last_message"];
}
