import { MetricType } from "../../../types/iot";

export interface APIMetricType {
  id: number;
  name: string;
  abbreviation: string;
  unit?: string;
}

export const isAPIMetricType = (o: any): o is APIMetricType =>
  o.id != null && o.name != null && o.abbreviation != null;

export interface GetMetricsParamsType {
  id?: MetricType["id"] | MetricType["id"][];
  name?: MetricType["name"] | MetricType["name"][];
  abbreviation?: MetricType["abbreviation"] | MetricType["abbreviation"][];
}
export interface GetMetricsAPIParamsType {
  id?: APIMetricType["id"] | APIMetricType["id"][];
  name?: APIMetricType["name"] | APIMetricType["name"][];
  abbreviation?:
    | APIMetricType["abbreviation"]
    | APIMetricType["abbreviation"][];
}
