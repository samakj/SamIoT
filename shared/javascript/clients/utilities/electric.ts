import { APIClient } from "../../extensions/axios/api-client";
import {
  UtilitiesConsumptionType,
  isUtilitiesConsumptionType,
} from "../../types/utilities";
import {
  APIUtilitiesConsumptionType,
  isAPIUtilitiesConsumptionType,
  GetUtilitiesConsumptionsParamsType,
  GetUtilitiesConsumptionsAPIParamsType,
} from "./types";

export class ElectricClient extends APIClient {
  constructor(host: string, port: number | string | null, ssl: boolean) {
    super(host, port, ssl);
  }

  static toAPIElectricConsumptionType(
    electric: UtilitiesConsumptionType | APIUtilitiesConsumptionType
  ): APIUtilitiesConsumptionType {
    if (isAPIUtilitiesConsumptionType(electric)) return { ...electric };
    return {
      id: electric.id,
      timestamp: electric.timestamp.toISOString(),
      consumption: electric.consumption,
    };
  }

  static toElectricConsumptionType(
    electric: UtilitiesConsumptionType | APIUtilitiesConsumptionType
  ): UtilitiesConsumptionType {
    if (isUtilitiesConsumptionType(electric)) return { ...electric };
    return {
      id: electric.id,
      timestamp: new Date(electric.timestamp),
      consumption: electric.consumption,
    };
  }

  getElectricConsumption(id: number) {
    return this.get<APIUtilitiesConsumptionType>(`/v0/electric/${id}`);
  }

  getElectricConsumptions(params?: GetUtilitiesConsumptionsParamsType) {
    const _params: GetUtilitiesConsumptionsAPIParamsType = {};

    if (params?.id != null) _params.id = params.id;
    if (params?.timestamp != null)
      _params.timestamp = Array.isArray(params.timestamp)
        ? params.timestamp.map((t) => t.toISOString())
        : params.timestamp.toISOString();
    if (params?.timestampGte != null)
      _params.timestamp_gte = params.timestampGte.toISOString();
    if (params?.timestampLte != null)
      _params.timestamp_lte = params.timestampLte.toISOString();
    if (params?.consumptionGte != null)
      _params.consumption_gte = params.consumptionGte;
    if (params?.consumptionLte != null)
      _params.consumption_lte = params.consumptionLte;

    return this.get<APIUtilitiesConsumptionType[]>(`/v0/electric/`, {
      params: _params,
    });
  }
}
