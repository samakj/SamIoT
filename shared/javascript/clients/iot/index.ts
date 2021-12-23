import { APIClient } from "../../extensions/axios/api-client";
import { DevicesClient } from "./devices";
import { LocationsClient } from "./locations";
import { MetricsClient } from "./metrics";
import { MeasurementsClient } from "./measurements";

export class IoTClient extends APIClient {
  devices: DevicesClient;
  locations: LocationsClient;
  measurements: MeasurementsClient;
  metrics: MetricsClient;

  constructor(host: string, port: number | string | null, ssl: boolean) {
    super(host, port, ssl);
    this.devices = new DevicesClient(host, port, ssl);
    this.locations = new LocationsClient(host, port, ssl);
    this.measurements = new MeasurementsClient(host, port, ssl);
    this.metrics = new MetricsClient(host, port, ssl);
  }
}
