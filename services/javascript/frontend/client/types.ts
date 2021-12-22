/** @format */

export interface ServerRoutingPropsType {
  ssr: boolean;
  url: string;
  context?: { [key: string]: unknown; url?: string };
}

export const isServerRoutingPropsType = (
  props: RoutingPropsType
  // @ts-ignore
): props is ServerRoutingPropsType => props?.ssr != null && props?.url != null;

export type RoutingPropsType = {} | ServerRoutingPropsType;

export type AppPropsType = RoutingPropsType;
