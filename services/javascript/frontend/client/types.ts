/** @format */

import { DeepPartial } from 'redux';
import { RootState } from './store';

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

declare global {
  interface Window {
    __PRELOADED_STATE__?: DeepPartial<RootState>;
  }
}
