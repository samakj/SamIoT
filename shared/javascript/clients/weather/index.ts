import { APIClient } from "../../extensions/axios/api-client";
import { CurrentWeatherClient } from "./current";
import { HourlyWeatherClient } from "./hourly";
import { DailyWeatherClient } from "./daily";

export class WeatherClient extends APIClient {
  current: CurrentWeatherClient;
  hourly: HourlyWeatherClient;
  daily: DailyWeatherClient;

  constructor(host: string, port: number | string | null, ssl: boolean) {
    super(host, port, ssl);
    this.current = new CurrentWeatherClient(host, port, ssl);
    this.hourly = new HourlyWeatherClient(host, port, ssl);
    this.daily = new DailyWeatherClient(host, port, ssl);
  }
}
