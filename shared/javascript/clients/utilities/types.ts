import { UtilitiesConsumptionType } from "../../types/utilities";

export interface APIUtilitiesConsumptionType {
  id: number;
  timestamp: string;
  consumption: number;
}

export const isAPIUtilitiesConsumptionType = (
  o: any
): o is APIUtilitiesConsumptionType =>
  o.id != null &&
  o.timestamp != null &&
  typeof o.timestamp === "string" &&
  o.consumption != null;

export interface GetUtilitiesConsumptionsParamsType {
  id?: UtilitiesConsumptionType["id"] | UtilitiesConsumptionType["id"][];
  timestamp?:
    | UtilitiesConsumptionType["timestamp"]
    | UtilitiesConsumptionType["timestamp"][];
  timestampGte?: UtilitiesConsumptionType["timestamp"];
  timestampLte?: UtilitiesConsumptionType["timestamp"];
  consumptionGte?: UtilitiesConsumptionType["consumption"];
  consumptionLte?: UtilitiesConsumptionType["consumption"];
}
export interface GetUtilitiesConsumptionsAPIParamsType {
  id?: APIUtilitiesConsumptionType["id"] | APIUtilitiesConsumptionType["id"][];
  timestamp?:
    | APIUtilitiesConsumptionType["timestamp"]
    | APIUtilitiesConsumptionType["timestamp"][];
  timestamp_gte?: APIUtilitiesConsumptionType["timestamp"];
  timestamp_lte?: APIUtilitiesConsumptionType["timestamp"];
  consumption_gte?: APIUtilitiesConsumptionType["consumption"];
  consumption_lte?: APIUtilitiesConsumptionType["consumption"];
}
