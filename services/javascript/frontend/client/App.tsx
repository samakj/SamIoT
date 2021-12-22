/** @format */

import React from 'react';
import { store } from 'client/store';
import { Provider, useSelector } from 'react-redux';
import { ThemeProvider } from 'styled-components';
import { RootState } from 'client/store';
import { GlobalStyle } from 'client/style';
import { BrowserRouter, Routes } from 'react-router-dom';
import { StaticRouter } from 'react-router-dom/server';
import { WeatherRoute } from 'client/routes/Weather';
import { OverviewRoute } from 'client/routes/Overview';
import { AppPropsType, RoutingPropsType, isServerRoutingPropsType } from './types';

export const ThemeWrapper = (props: React.PropsWithChildren<{}>) => {
  const theme = useSelector((state: RootState) => state.theme);
  return (
    <ThemeProvider theme={theme}>
      <GlobalStyle />
      {props?.children}
    </ThemeProvider>
  );
};

export const Router = (props: React.PropsWithChildren<RoutingPropsType>) =>
  isServerRoutingPropsType(props) ? (
    <StaticRouter location={props.url}>{props.children}</StaticRouter>
  ) : (
    <BrowserRouter>{props.children}</BrowserRouter>
  );

export const App = (props: AppPropsType) => {
  return (
    <>
      <Provider store={store}>
        <ThemeWrapper>
          <Router {...props}>
            <Routes>
              {OverviewRoute}
              {WeatherRoute}
            </Routes>
          </Router>
        </ThemeWrapper>
      </Provider>
    </>
  );
};
