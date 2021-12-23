import { APIClient } from "../../extensions/axios/api-client";
import { GasClient } from "./gas";
import { ElectricClient } from "./electric";

export class UtilitiesClient extends APIClient {
  gas: GasClient;
  electric: ElectricClient;

  constructor(host: string, port: number | string | null, ssl: boolean) {
    super(host, port, ssl);
    this.gas = new GasClient(host, port, ssl);
    this.electric = new ElectricClient(host, port, ssl);
  }
}
