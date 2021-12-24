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

export class GasClient extends APIClient {
  constructor(host: string, port: number | string | null, ssl: boolean) {
    super(host, port, ssl);
  }

  toAPIGasConsumptionType(
    gas: UtilitiesConsumptionType | APIUtilitiesConsumptionType
  ): APIUtilitiesConsumptionType {
    if (isAPIUtilitiesConsumptionType(gas)) return { ...gas };
    return {
      id: gas.id,
      timestamp: gas.timestamp.toISOString(),
      consumption: gas.consumption,
    };
  }

  toGasConsumptionType(
    gas: UtilitiesConsumptionType | APIUtilitiesConsumptionType
  ): UtilitiesConsumptionType {
    if (isUtilitiesConsumptionType(gas)) return { ...gas };
    return {
      id: gas.id,
      timestamp: new Date(gas.timestamp),
      consumption: gas.consumption,
    };
  }

  getGasConsumption(id: number) {
    return this.get<APIUtilitiesConsumptionType>(`/v0/gas/${id}`);
  }

  getGasConsumptions(params?: GetUtilitiesConsumptionsParamsType) {
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

    return this.get<APIUtilitiesConsumptionType[]>(`/v0/gas/`, {
      params: _params,
    });
  }
}
