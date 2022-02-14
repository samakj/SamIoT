/** @format */

import React from 'react';
import { ThemeProvider } from 'styled-components';
import { GlobalStyle, theme } from 'client/style';

export const ThemeWrapper = (props: React.PropsWithChildren<{}>) => {
  return (
    <ThemeProvider theme={theme}>
      <GlobalStyle />
      {props?.children}
    </ThemeProvider>
  );
};

export const App = () => {
  return (
    <>
      <ThemeWrapper>
          <div>Hello</div>
      </ThemeWrapper>
    </>
  );
};
