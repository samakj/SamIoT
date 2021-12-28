/** @format */

import React from 'react';
import { Card, SmallCard, SmallWideCard } from 'client/components/Cards';
import { HeaderBar } from 'client/components/header-bar';
import { ContentGrid } from 'client/components/ContentGrid';
import { H2 } from 'client/components/Headers';
import { Route } from 'react-router-dom';
import { TemperatureSummaryCard } from 'client/components/cards/temperature-summary';
import { useLocations } from 'client/store/iot/locations/hooks';
import { useMetrics } from 'client/store/iot/metrics/hooks';
import { BoilerSummaryCard } from 'client/components/cards/boiler-summary';

const ALL_LOCATIONS_FILTERS = {};
const ALL_METRICS_FILTERS = {};

const Overview = () => {
  const locations = useLocations(ALL_LOCATIONS_FILTERS);
  const metrics = useMetrics(ALL_METRICS_FILTERS);

  return (
    <ContentGrid>
      <HeaderBar title="Overview" />
      <TemperatureSummaryCard locations={locations} metrics={metrics} />
      <BoilerSummaryCard locations={locations} metrics={metrics} />
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
};

export const OverviewRoute = <Route path="/" element={<Overview />} />;
