/** @format */

import React, { useCallback, useMemo } from 'react';
import { Thermometer } from 'react-feather';
import { Card } from 'client/components/Cards';
import { H2, H3, H6 } from 'client/components/Headers';
import styled, { useTheme } from 'styled-components';
import { IconButton, IconButtonContainer } from 'client/components/icon-button';
import { TemperatureSummaryCardPropsType } from './types';
import { useLatestMeasurements } from 'client/store/iot/measurements/hooks';
import { transparentize } from 'polished';
import { LocationType } from 'shared/javascript/types/iot';
import { DAY_IN_MS, HOUR_IN_MS, MINUTE_IN_MS, SECOND_IN_MS } from 'shared/javascript/static/times';
import { Camera, Tv, Book, Tag } from 'react-feather';

const HeaderContainer = styled.div`
  display: flex;
  align-items: center;
  margin-bottom: 1.5rem;

  ${IconButtonContainer} {
    margin-right: 1rem;
  }
`;

const ContentContainer = styled.div`
  display: grid;
  grid-template-columns: 1fr 1fr;
  grid-gap: 2rem;
  padding: 1rem;
`;

const TemperatureGrid = styled.div`
  display: inline-grid;
  grid-template-columns: 3rem auto;
  grid-template-rows: auto auto;
  grid-template-areas: 'icon value' 'icon name';
  grid-column-gap: 1.5rem;
  grid-row-gap: 0.5rem;
  align-items: center;

  ${IconButtonContainer} {
    grid-area: icon;
  }

  ${H3} {
    grid-area: value;
    margin-bottom: 0;
  }

  p {
    grid-area: name;
    text-transform: capitalize;
    margin: 0;
  }
`;

const TimestampElement = styled.div`
  font-size: 0.875rem;
  opacity: 0.6;
  text-transform: none;
`;

const ICON_MAP = {
  'camera-room': Camera,
  'living-room': Tv,
  'master-bedroom': Tag,
  study: Book,
};

export const TemperatureSummaryCard = ({ locations, metrics }: TemperatureSummaryCardPropsType) => {
  const theme = useTheme();

  const heatedLocationIds = useMemo(
    () =>
      locations &&
      Object.values(locations)
        .reduce((acc, location) => {
          if (location.tags.includes('heated')) acc.push(location.id);
          return acc;
        }, [])
        .sort((locationIdA, locationIdB) => {
          if (locations[locationIdA].name > locations[locationIdB].name) return 1;
          if (locations[locationIdA].name < locations[locationIdB].name) return -1;
          return 0;
        }),
    [locations]
  );
  const temperatureMetricId = useMemo(
    () => metrics && Object.values(metrics).find((metric) => metric.name === 'temperature')?.id,
    [metrics]
  );
  const filters = useMemo(
    () =>
      temperatureMetricId && heatedLocationIds?.length
        ? { metricId: temperatureMetricId, locationId: heatedLocationIds }
        : undefined,
    [temperatureMetricId, heatedLocationIds]
  );

  const latestTemperatures = useLatestMeasurements(filters);

  const getLocationLatestTemperature = useCallback(
    (locationId: LocationType['id']) => {
      if (latestTemperatures == null) return '...';

      const value = Object.values(latestTemperatures).find(
        (measurement) => measurement.locationId === locationId
      )?.value;
      const unit = (metrics && temperatureMetricId && metrics[temperatureMetricId]?.unit) || '';
      if (value == null) return `--.-${unit}`;

      return `${value}${unit}`;
    },
    [latestTemperatures, metrics, temperatureMetricId]
  );

  const getLocationLatestTimestamp = useCallback(
    (locationId: LocationType['id']) => {
      if (latestTemperatures == null) return '...';

      const timestamp = Object.values(latestTemperatures).find(
        (measurement) => measurement.locationId === locationId
      )?.timestamp;

      if (timestamp == null) return undefined;

      const dt = +new Date() - +timestamp;
      let content = `Last change ${Math.ceil(dt / DAY_IN_MS)}h ago`;
      if (dt <= MINUTE_IN_MS - SECOND_IN_MS)
        content = `Last change ${Math.ceil(dt / SECOND_IN_MS)}s ago`;
      if (dt <= HOUR_IN_MS - MINUTE_IN_MS)
        content = `Last change ${Math.ceil(dt / MINUTE_IN_MS)}m ago`;
      if (dt <= DAY_IN_MS - HOUR_IN_MS) content = `Last change ${Math.ceil(dt / HOUR_IN_MS)}h ago`;
      return <TimestampElement title={timestamp.toISOString()}>{content}</TimestampElement>;
    },
    [latestTemperatures]
  );

  return (
    <Card>
      <HeaderContainer>
        <IconButton
          Icon={Thermometer}
          colour={theme.colours.orange}
          size={4}
          onClick={console.log}
        />
        <H2>Temperature</H2>
      </HeaderContainer>
      <ContentContainer>
        {temperatureMetricId &&
          heatedLocationIds?.map((locationId) => (
            <TemperatureGrid key={locationId}>
              <IconButton
                Icon={ICON_MAP[locations[locationId]?.name]}
                colour={theme.colours.foreground}
                size={3}
                backgroundOpacity={0.05}
              />
              <H3>{getLocationLatestTemperature(locationId)}</H3>
              <p>
                {locations[locationId]?.name?.replace('-', ' ')}
                {getLocationLatestTimestamp(locationId)}
              </p>
            </TemperatureGrid>
          ))}
      </ContentContainer>
    </Card>
  );
};
