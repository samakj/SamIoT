/** @format */

import React from 'react';
import { store } from 'client/store';
import { Provider, useSelector } from 'react-redux';
import { ThemeProvider } from 'styled-components';
import { RootState } from 'client/store';
import { Card, SmallCard, SmallWideCard } from 'client/components/Cards';
import { HeaderBar } from 'client/components/header-bar';
import { ContentGrid } from './components/ContentGrid';
import { H2 } from './components/Headers';
import { GlobalStyle } from './style';

export const ThemeWrapper = ({ children }: React.PropsWithChildren<{}>) => {
  const theme = useSelector((state: RootState) => state.theme);
  return (
    <ThemeProvider theme={theme}>
      <GlobalStyle />
      {children}
    </ThemeProvider>
  );
};

export const App = () => {
  return (
    <>
      <Provider store={store}>
        <ThemeWrapper>
          <ContentGrid>
            <HeaderBar />
            <Card>
              <H2>Normal Card</H2>
              <p>This is a test card with some content.</p>
            </Card>
            <Card>
              <H2>Normal Card</H2>
              <p>This is a test card with some content.</p>
            </Card>
            <SmallCard>
              <H2>Small Card</H2>
              <p>This is a test card with some content.</p>
            </SmallCard>
            <SmallCard>
              <H2>Small Card</H2>
              <p>This is a test card with some content.</p>
            </SmallCard>
            <Card>
              <H2>Normal Card</H2>
              <p>This is a test card with some content.</p>
            </Card>
            <SmallWideCard>
              <H2>Small Wide Card</H2>
              <p>This is a test card with some content.</p>
            </SmallWideCard>
          </ContentGrid>
        </ThemeWrapper>
      </Provider>
    </>
  );
};
