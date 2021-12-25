/** @format */

import React from 'react';
import ReactDOM from 'react-dom';
import { App } from 'client/App';
import { Provider } from 'react-redux';
import { createStore } from 'client/store';

const store = createStore(window.__PRELOADED_STATE__);
console.log(window.__PRELOADED_STATE__.theme.variant, store.getState().theme.variant);
delete window.__PRELOADED_STATE__;

ReactDOM.hydrate(
  <Provider store={store}>
    <App />
  </Provider>,
  document.getElementById('root')
);
