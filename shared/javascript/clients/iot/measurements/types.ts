import { MeasurementType } from "../../../types/iot";
import { APIDeviceType } from "../devices/types";
import { APILocationType } from "../locations/types";
import { APIMetricType } from "../metrics/types";

export enum APIMeasurementValueTypes {
  STRING = "string",
  FLOAT = "float",
  INTEGER = "integer",
  BOOLEAN = "boolean",
}

export type APIMeasurementType = {
  id: number;
  timestamp: string;
  device_id: APIDeviceType["id"];
  location_id: APILocationType["id"];
  metric_id: APIMetricType["id"];
  tags: string[];
} & (
  | { value_type: APIMeasurementValueTypes.FLOAT; value?: string }
  | { value_type: APIMeasurementValueTypes.INTEGER; value?: string }
  | { value_type: APIMeasurementValueTypes.STRING; value?: string }
  | { value_type: APIMeasurementValueTypes.BOOLEAN; value?: boolean }
);

export const isAPIMeasurementType = (o: any): o is APIMeasurementType =>
  o.id != null &&
  o.timestamp != null &&
  o.device_id != null &&
  o.location_id != null &&
  o.metric_id != null &&
  o.tags != null &&
  o.value_type != null;

export interface GetMeasurementsParamsType {
  id?: MeasurementType["id"] | MeasurementType["id"][];
  deviceId?: MeasurementType["deviceId"] | MeasurementType["deviceId"][];
  locationId?: MeasurementType["locationId"] | MeasurementType["locationId"][];
  metricId?: MeasurementType["metricId"] | MeasurementType["metricId"][];
  tags?: MeasurementType["tags"] | MeasurementType["tags"][];
  timestampGte?: MeasurementType["timestamp"];
  timestampLte?: MeasurementType["timestamp"];
  value?: MeasurementType["value"];
  valueGte?: MeasurementType["value"];
  valueLte?: MeasurementType["value"];
}
export interface GetMeasurementsAPIParamsType {
  id?: APIMeasurementType["id"] | APIMeasurementType["id"][];
  device_id?:
    | APIMeasurementType["device_id"]
    | APIMeasurementType["device_id"][];
  location_id?:
    | APIMeasurementType["location_id"]
    | APIMeasurementType["location_id"][];
  metric_id?:
    | APIMeasurementType["metric_id"]
    | APIMeasurementType["metric_id"][];
  tags?: APIMeasurementType["tags"] | APIMeasurementType["tags"][];
  timestamp_gte?: APIMeasurementType["timestamp"];
  timestamp_lte?: APIMeasurementType["timestamp"];
  value?: APIMeasurementType["value"];
  value_gte?: APIMeasurementType["value"];
  value_lte?: APIMeasurementType["value"];
}
export interface GetLatestMeasurementsParamsType {
  deviceId?: MeasurementType["deviceId"] | MeasurementType["deviceId"][];
  locationId?: MeasurementType["locationId"] | MeasurementType["locationId"][];
  metricId?: MeasurementType["metricId"] | MeasurementType["metricId"][];
  tags?: MeasurementType["tags"] | MeasurementType["tags"][];
}
export interface GetLatestMeasurementsAPIParamsType {
  device_id?:
    | APIMeasurementType["device_id"]
    | APIMeasurementType["device_id"][];
  location_id?:
    | APIMeasurementType["location_id"]
    | APIMeasurementType["location_id"][];
  metric_id?:
    | APIMeasurementType["metric_id"]
    | APIMeasurementType["metric_id"][];
  tags?: APIMeasurementType["tags"] | APIMeasurementType["tags"][];
}
export interface GetAverageMeasurementsParamsType {
  start?: Date;
  end?: Date;
  period?: number;
}
export interface GetAverageMeasurementsAPIParamsType {
  start?: string;
  end?: string;
  period?: number;
}
