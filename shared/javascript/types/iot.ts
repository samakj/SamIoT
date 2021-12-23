/** @format */

export interface LocationType {
  id: number;
  name: string;
  tags: string[];
}

export const isLocationType = (o: any): o is LocationType =>
  o.id != null && o.name != null && o.tags != null;

export interface DeviceType {
  id: number;
  mac: string;
  ip: string;
  websocketPath: string;
  locationId: LocationType['id'];
  lastMessage?: Date;
}

export const isDeviceType = (o: any): o is DeviceType =>
  o.id != null && o.mac != null && o.ip != null && o.websocketPath != null && o.locationId != null;

export interface MetricType {
  id: number;
  name: string;
  abbreviation: string;
  unit?: string;
}

export const isMetricType = (o: any): o is MetricType =>
  o.id != null && o.name != null && o.abbreviation != null;

export enum MeasurementValueTypes {
  STRING = 'string',
  FLOAT = 'float',
  INTEGER = 'integer',
  BOOLEAN = 'boolean',
}

export type MeasurementType = {
  id: number;
  timestamp: Date;
  deviceId: DeviceType['id'];
  locationId: LocationType['id'];
  metricId: MetricType['id'];
  tags: string[];
} & (
  | { valueType: MeasurementValueTypes.FLOAT; value?: number }
  | { valueType: MeasurementValueTypes.INTEGER; value?: number }
  | { valueType: MeasurementValueTypes.STRING; value?: string }
  | { valueType: MeasurementValueTypes.BOOLEAN; value?: boolean }
);

export const isMeasurementType = (o: any): o is MeasurementType =>
  o.id != null &&
  o.timestamp != null &&
  o.deviceId != null &&
  o.locationId != null &&
  o.metricId != null &&
  o.tags != null &&
  o.valueType != null;

export type MeasurementAverageType = {
  start: Date;
  end: Date;
  locationId: LocationType['id'];
  metricId: MetricType['id'];
  tags: string[];
  value?: number;
  min?: number;
  max?: number;
};

export const isMeasurementAverageType = (o: any): o is MeasurementAverageType =>
  o.id != null &&
  o.start != null &&
  o.end != null &&
  o.locationId != null &&
  o.metricId != null &&
  o.tags != null;
