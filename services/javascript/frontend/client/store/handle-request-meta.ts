/** @format */

import { PayloadAction } from '@reduxjs/toolkit';
import { request } from 'http';
import { RequestMetaStateType, RequestStates } from './types';

type ReduxRequestStatuses = 'pending' | 'fulfilled' | 'rejected';

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
      args: action.meta.arg,
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
