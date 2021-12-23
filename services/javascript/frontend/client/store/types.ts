/** @format */

export enum RequestStates {
  NONE = 'none',
  PENDING = 'pending',
  DONE = 'done',
  ERROR = 'error',
}

export interface RequestMetaType<Args extends unknown> {
  state: RequestStates;
  started: number;
  ended?: number;
  duration?: number;
  statusCode?: number;
  statusText?: string;
  error?: string;
  args?: Args;
}

export interface RequestMetaStateType<Arg extends unknown> {
  requests: { [id: number]: RequestMetaType<Arg> };
}
