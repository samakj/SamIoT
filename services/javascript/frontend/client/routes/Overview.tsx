/** @format */

import React from 'react';
import { Card, SmallCard, SmallWideCard } from 'client/components/Cards';
import { HeaderBar } from 'client/components/header-bar';
import { ContentGrid } from 'client/components/ContentGrid';
import { H2 } from 'client/components/Headers';
import { Route } from 'react-router-dom';

const Overview = () => (
  <ContentGrid>
    <HeaderBar title="Overview" />
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
);

export const OverviewRoute = <Route path="/" element={<Overview />} />;
