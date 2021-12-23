export interface UtilitiesConsumptionType {
  id: number;
  timestamp: Date;
  consumption: number;
}

export const isUtilitiesConsumptionType = (
  o: any
): o is UtilitiesConsumptionType =>
  o.id != null &&
  o.timestamp != null &&
  typeof o.timestamp === "object" &&
  o.consumption != null;
