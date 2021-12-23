export interface APIMetricType {
  id: number;
  name: string;
  abbreviation: string;
  unit?: string;
}

export const isAPIMetricType = (o: any): o is APIMetricType =>
  o.id != null && o.name != null && o.abbreviation != null;
