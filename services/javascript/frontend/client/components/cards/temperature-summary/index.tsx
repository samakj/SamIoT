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
              <p>{locations[locationId]?.name?.replace('-', ' ')}</p>
            </TemperatureGrid>
          ))}
      </ContentContainer>
    </Card>
  );
};
