/** @format */

import React from 'react';
import { store } from 'client/store';
import { Provider } from 'react-redux';
import { ThemeProvider } from 'styled-components';
import { Card, FullWidthCard, SmallCard, SmallWideCard } from './components/Cards';
import { ContentGrid } from './components/ContentGrid';
import { H2 } from './components/Headers';
import { DarkTheme, GlobalStyle, LightTheme } from './style';

export const App: React.FunctionComponent = () => (
  <>
    <Provider store={store}>
      <ThemeProvider theme={DarkTheme}>
        <GlobalStyle />
        <ContentGrid>
          <FullWidthCard>
            <H2>Header Card</H2>
            <p></p>
          </FullWidthCard>
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
      </ThemeProvider>
    </Provider>
  </>
);
