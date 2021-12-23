import axios, { AxiosInstance, AxiosRequestConfig, AxiosResponse } from "axios";

export class APIClient {
  host: string;
  port?: number | string;
  ssl: boolean;
  httpBaseUrl: string;
  websocketBaseUrl: string;
  axiosInstance: AxiosInstance;

  constructor(host: string, port: number | string | null, ssl: boolean) {
    this.host = host;
    this.port = port;
    this.ssl = ssl;
    this.httpBaseUrl =
      `http${this.ssl ? "s" : ""}://` +
      `${this.host}${this.port != null ? `:${this.port}` : ""}`;
    this.websocketBaseUrl =
      `ws${this.ssl ? "s" : ""}://` +
      `${this.host}${this.port != null ? `:${this.port}` : ""}`;
    this.axiosInstance = axios.create({
      baseURL: this.httpBaseUrl,
      timeout: 1000,
    });
  }

  get<T = any, R = AxiosResponse<T>, D = any>(
    endpoint: string,
    config?: AxiosRequestConfig<D>
  ): Promise<R> {
    return this.axiosInstance.get(endpoint, config);
  }

  post<T = any, R = AxiosResponse<T>, D = any>(
    endpoint: string,
    data?: D,
    config?: AxiosRequestConfig<D>
  ): Promise<R> {
    return this.axiosInstance.post(endpoint, data, config);
  }

  patch<T = any, R = AxiosResponse<T>, D = any>(
    endpoint: string,
    data?: D,
    config?: AxiosRequestConfig<D>
  ): Promise<R> {
    return this.axiosInstance.patch(endpoint, data, config);
  }

  delete<T = any, R = AxiosResponse<T>, D = any>(
    endpoint: string,
    config?: AxiosRequestConfig<D>
  ): Promise<R> {
    return this.axiosInstance.delete(endpoint, config);
  }
}
