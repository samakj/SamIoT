import { APIClient } from "../../../extensions/axios/api-client";
import {
  MeasurementType,
  isMeasurementType,
  MeasurementValueTypes,
} from "../../../types/iot";
import {
  APIMeasurementValueTypes,
  GetAverageMeasurementsAPIParamsType,
  GetAverageMeasurementsParamsType,
  GetLatestMeasurementsAPIParamsType,
  GetLatestMeasurementsParamsType,
  GetMeasurementsAPIParamsType,
  GetMeasurementsParamsType,
} from "./types";
import { APIMeasurementType, isAPIMeasurementType } from "./types";

export class MeasurementsClient extends APIClient {
  constructor(host: string, port: number | string | null, ssl: boolean) {
    super(host, port, ssl);
  }

  static toAPIMeasurementType(
    measurement: MeasurementType | APIMeasurementType
  ): APIMeasurementType {
    if (isAPIMeasurementType(measurement)) return { ...measurement };
    return {
      id: measurement.id,
      timestamp: measurement.timestamp.toISOString(),
      device_id: measurement.deviceId,
      location_id: measurement.locationId,
      metric_id: measurement.metricId,
      tags: measurement.tags,
      // @ts-ignore
      value_type: measurement.valueType,
      value:
        measurement.valueType === MeasurementValueTypes.BOOLEAN
          ? measurement.value
          : measurement.value.toString(),
    };
  }

  static toMeasurementType(
    measurement: MeasurementType | APIMeasurementType
  ): MeasurementType {
    if (isMeasurementType(measurement)) return { ...measurement };
    return {
      id: measurement.id,
      timestamp: new Date(measurement.timestamp),
      deviceId: measurement.device_id,
      locationId: measurement.location_id,
      metricId: measurement.metric_id,
      tags: measurement.tags,
      // @ts-ignore
      valueType: measurement.value_type,
      value:
        measurement.value_type === APIMeasurementValueTypes.FLOAT
          ? parseFloat(measurement.value)
          : measurement.value_type === APIMeasurementValueTypes.INTEGER
          ? parseInt(measurement.value)
          : measurement.value,
    };
  }

  getMeasurement(id: number) {
    return this.get<APIMeasurementType>(`/v0/measurements/${id}`);
  }

  getMeasurements(params?: GetMeasurementsParamsType) {
    const _params: GetMeasurementsAPIParamsType = {};

    if (params?.id != null) _params.id = params.id;
    if (params?.deviceId != null) _params.device_id = params.deviceId;
    if (params?.locationId != null) _params.location_id = params.locationId;
    if (params?.metricId != null) _params.metric_id = params.metricId;
    if (params?.tags != null) _params.tags = params.tags;
    if (params?.timestampGte != null)
      _params.timestamp_gte = params.timestampGte.toISOString();
    if (params?.timestampLte != null)
      _params.timestamp_lte = params.timestampLte.toISOString();
    if (params?.value != null)
      _params.value =
        typeof params.value === "boolean"
          ? params.value
          : params.value.toString();
    if (params?.valueGte != null)
      _params.value_gte = params.valueGte.toString();
    if (params?.valueLte != null)
      _params.value_lte = params.valueLte.toString();

    return this.get<APIMeasurementType[]>(`/v0/measurements/`, {
      params: _params,
    });
  }

  getLatestMeasurements(params?: GetLatestMeasurementsParamsType) {
    const _params: GetLatestMeasurementsAPIParamsType = {};

    if (params?.deviceId != null) _params.device_id = params.deviceId;
    if (params?.locationId != null) _params.location_id = params.locationId;
    if (params?.metricId != null) _params.metric_id = params.metricId;
    if (params?.tags != null) _params.tags = params.tags;

    return this.get<APIMeasurementType[]>(`/v0/measurements/latest/`, {
      params: _params,
    });
  }

  getAverageMeasurements(
    locationId: MeasurementType["locationId"],
    metricId: MeasurementType["metricId"],
    tags: MeasurementType["tags"],
    params?: GetAverageMeasurementsParamsType
  ) {
    const _params: GetAverageMeasurementsAPIParamsType = {};

    if (params?.start != null) _params.start = params.start.toISOString();
    if (params?.end != null) _params.end = params.end.toISOString();
    if (params?.period != null) _params.period = params.period;

    return this.get<APIMeasurementType[]>(
      `/v0/measurements/average/${locationId}/${metricId}/${tags.join(",")}`,
      {
        params: _params,
      }
    );
  }
}
