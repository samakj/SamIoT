import { APIClient } from "../../../extensions/axios/api-client";
import { LocationType, isLocationType } from "../../../types/iot";
import { GetLocationsAPIParamsType, GetLocationsParamsType } from "./types";
import { APILocationType, isAPILocationType } from "./types";

export class LocationsClient extends APIClient {
  constructor(host: string, port: number | string | null, ssl: boolean) {
    super(host, port, ssl);
  }

  static toAPILocationType(
    location: LocationType | APILocationType
  ): APILocationType {
    // Always the same
    // if (isAPILocationType(location)) return {...location};
    return {
      id: location.id,
      name: location.name,
      tags: location.tags,
    };
  }

  static toLocationType(
    location: LocationType | APILocationType
  ): LocationType {
    // Always the same
    // if (isLocationType(location)) return {...location};
    return {
      id: location.id,
      name: location.name,
      tags: location.tags,
    };
  }

  getLocation(id: number) {
    return this.get<APILocationType>(`/v0/locations/${id}`);
  }

  getLocationByName(name: string) {
    return this.get<APILocationType>(`/v0/locations/name/${name}`);
  }

  getLocations(params?: GetLocationsParamsType) {
    const _params: GetLocationsAPIParamsType = {};

    if (params?.id != null) _params.id = params.id;
    if (params?.name != null) _params.name = params.name;
    if (params?.tags != null) _params.tags = params.tags;

    return this.get<APILocationType[]>(`/v0/locations/`, { params: _params });
  }
}
