import { LocationType } from "../../../types/iot";

export interface APILocationType {
  id: number;
  name: string;
  tags: string[];
}

export const isAPILocationType = (o: any): o is APILocationType =>
  o.id != null && o.name != null && o.tags != null;

export interface GetLocationsParamsType {
  id?: LocationType["id"] | LocationType["id"][];
  name?: LocationType["name"] | LocationType["name"][];
  tags?: LocationType["tags"] | LocationType["tags"][];
}
export interface GetLocationsAPIParamsType {
  id?: APILocationType["id"] | APILocationType["id"][];
  name?: APILocationType["name"] | APILocationType["name"][];
  tags?: APILocationType["tags"] | APILocationType["tags"][];
}
