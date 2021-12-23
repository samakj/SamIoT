/** @format */

import { PayloadAction } from '@reduxjs/toolkit';
import { request } from 'http';
import { RequestMetaStateType, RequestStates } from './types';

type ReduxRequestStatuses = 'pending' | 'fulfilled' | 'rejected';

const serialiseArgs = (args: unknown): unknown => {
  if (['string', 'number', 'boolean'].includes(typeof args)) return args;
  if (Array.isArray(args)) return args.map(serialiseArgs);
  if (args instanceof Date) return args.toISOString();
  if (args === undefined) return undefined;
  if (args == null) return null;
  if (typeof args === 'object')
    return Object.entries(args).reduce(
      (acc, [key, value]) => ({ ...acc, [key]: serialiseArgs(value) }),
      {}
    );
  return `${args}`;
};

export const handleRequestMeta = <Arg extends unknown>(
  state: RequestMetaStateType<unknown>,
  action: PayloadAction<
    unknown,
    string,
    { arg: Arg; requestId: string; requestStatus: ReduxRequestStatuses }
  >
) => {
  if (action.meta.requestStatus === 'pending') {
    state.requests[action.meta.requestId] = {
      state: RequestStates.PENDING,
      started: new Date().toISOString(),
      args: serialiseArgs(action.meta.arg),
    };
  }
  if (action.meta.requestStatus === 'fulfilled') {
    state.requests[action.meta.requestId] = {
      ...state.requests[action.meta.requestId],
      state: RequestStates.DONE,
      ended: new Date().toISOString(),
      duration: state.requests[action.meta.requestId]?.started
        ? +new Date() - +new Date(state.requests[action.meta.requestId].started)
        : null,
    };
  }
  if (action.meta.requestStatus === 'rejected') {
    state.requests[action.meta.requestId] = {
      ...state.requests[action.meta.requestId],
      state: RequestStates.ERROR,
      ended: new Date(),
      duration: state.requests[action.meta.requestId]?.started
        ? +new Date() - +new Date(state.requests[action.meta.requestId].started)
        : null,
      error: 'true',
    };
  }
};
