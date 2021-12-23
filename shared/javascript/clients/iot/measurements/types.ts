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
