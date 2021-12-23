import { APIClient } from "../../../extensions/axios/api-client";
import { MetricType, isMetricType } from "../../../types/iot";
import { GetMetricsAPIParamsType, GetMetricsParamsType } from "./types";
import { APIMetricType, isAPIMetricType } from "./types";

export class MetricsClient extends APIClient {
  constructor(host: string, port: number | string | null, ssl: boolean) {
    super(host, port, ssl);
  }

  static toAPIMetricType(metric: MetricType | APIMetricType): APIMetricType {
    // Always the same
    // if (isAPIMetricType(metric)) return { ...metric };
    return {
      id: metric.id,
      name: metric.name,
      abbreviation: metric.abbreviation,
      unit: metric.unit,
    };
  }

  static toMetricType(metric: MetricType | APIMetricType): MetricType {
    // Always the same
    // if (isMetricType(metric)) return { ...metric };
    return {
      id: metric.id,
      name: metric.name,
      abbreviation: metric.abbreviation,
      unit: metric.unit,
    };
  }

  getMetric(id: number) {
    return this.get<APIMetricType>(`/v0/metrics/${id}`);
  }

  getMetricByName(name: string) {
    return this.get<APIMetricType>(`/v0/metrics/name/${name}`);
  }

  getMetricByAbbreviation(abbreviation: string) {
    return this.get<APIMetricType>(`/v0/metrics/abbreviation/${abbreviation}`);
  }

  getMetrics(params?: GetMetricsParamsType) {
    const _params: GetMetricsAPIParamsType = {};

    if (params?.id != null) _params.id = params.id;
    if (params?.name != null) _params.name = params.name;
    if (params?.abbreviation != null)
      _params.abbreviation = params.abbreviation;

    return this.get<APIMetricType[]>(`/v0/metrics/`, { params: _params });
  }
}
