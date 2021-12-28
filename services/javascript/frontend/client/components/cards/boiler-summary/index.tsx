/** @format */

import React, { useCallback, useMemo } from 'react';
import { Thermometer } from 'react-feather';
import { Card } from 'client/components/Cards';
import { H2, H3, H6 } from 'client/components/Headers';
import styled, { useTheme } from 'styled-components';
import { IconButton, IconButtonContainer } from 'client/components/icon-button';
import { HeatingSummaryCardPropsType } from './types';
import { useAverageMeasurements, useLatestMeasurements } from 'client/store/iot/measurements/hooks';
import { transparentize } from 'polished';
import { LocationType } from 'shared/javascript/types/iot';
import { DAY_IN_MS, HOUR_IN_MS, MINUTE_IN_MS, SECOND_IN_MS } from 'shared/javascript/static/times';
import { mdiCampfire, mdiFire, mdiWaterOutline } from '@mdi/js';
import { TankTemperatureGraph } from './graph';

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

const BoilerGrid = styled.div`
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

const BoilerStateHeader = styled(H3)<{ state: boolean }>`
  color: ${({ state, theme }) => (state ? theme.colours.success : undefined)};
`;

const TimestampElement = styled.span`
  display: block;
  font-size: 0.875rem;
  opacity: 0.6;
  text-transform: none;
  font-weight: normal;
`;

const TemperatureGraphContainer = styled.div`
  display: grid;
  grid-template-columns: 1fr auto;
  grid-template-areas: 'graph latest';
  grid-column: 1/-1;
  grid-gap: 2rem;
  margin: 0 -1rem -2.5rem -2.5rem;
  align-items: center;
`;

const ICON_MAP = {
  heating: mdiFire,
  'hot-water': mdiWaterOutline,
};

export const BoilerSummaryCard = ({ locations, metrics }: HeatingSummaryCardPropsType) => {
  const theme = useTheme();

  const kitchenLocationId = useMemo(
    () => locations && Object.values(locations).find((location) => location.name === 'kitchen')?.id,
    [locations]
  );
  const babyRoomLocationId = useMemo(
    () =>
      locations && Object.values(locations).find((location) => location.name === 'baby-room')?.id,
    [locations]
  );
  const onMetricId = useMemo(
    () => metrics && Object.values(metrics).find((metric) => metric.name === 'on')?.id,
    [metrics]
  );
  const temperatureMetricId = useMemo(
    () => metrics && Object.values(metrics).find((metric) => metric.name === 'temperature')?.id,
    [metrics]
  );
  const heatingStateFilters = useMemo(
    () =>
      kitchenLocationId && onMetricId
        ? { metricId: onMetricId, locationId: kitchenLocationId, tags: ['heating'] }
        : undefined,
    [kitchenLocationId, kitchenLocationId]
  );
  const hotWaterStateFilters = useMemo(
    () =>
      kitchenLocationId && onMetricId
        ? { metricId: onMetricId, locationId: kitchenLocationId, tags: ['hot-water'] }
        : undefined,
    [kitchenLocationId, kitchenLocationId]
  );
  const tankTemperatureFilters = useMemo(
    () => (temperatureMetricId ? { metric_id: temperatureMetricId, tags: ['tank'] } : undefined),
    [temperatureMetricId]
  );

  const heatingLatestState = useLatestMeasurements(heatingStateFilters);
  const hotWaterLatestState = useLatestMeasurements(hotWaterStateFilters);
  const tankTemperatureLatestState = useLatestMeasurements(tankTemperatureFilters);

  const tankTemperatureLatest = useMemo(
    () =>
      (tankTemperatureLatestState &&
        Object.values(tankTemperatureLatestState).length === 1 &&
        Object.values(tankTemperatureLatestState)[0]) ||
      undefined,
    [tankTemperatureLatestState]
  );

  const hotWaterLatest = useMemo(
    () =>
      (hotWaterLatestState &&
        Object.values(hotWaterLatestState).length === 1 &&
        Object.values(hotWaterLatestState)[0]) ||
      undefined,
    [hotWaterLatestState]
  );

  const heatingLatest = useMemo(
    () =>
      (heatingLatestState &&
        Object.values(heatingLatestState).length === 1 &&
        Object.values(heatingLatestState)[0]) ||
      undefined,
    [heatingLatestState]
  );

  const getTimeDelta = useCallback((date?: Date) => {
    if (!date) return '';
    const dt = +new Date() - +date;
    let content = `${Math.ceil(dt / DAY_IN_MS)}d`;
    if (dt <= MINUTE_IN_MS - SECOND_IN_MS) content = `${Math.ceil(dt / SECOND_IN_MS)}s`;
    if (dt <= HOUR_IN_MS - MINUTE_IN_MS) content = `${Math.ceil(dt / MINUTE_IN_MS)}m`;
    if (dt <= DAY_IN_MS - HOUR_IN_MS) content = `${Math.ceil(dt / HOUR_IN_MS)}h`;
    return <TimestampElement title={date.toISOString()}>{content}</TimestampElement>;
  }, []);

  return (
    <Card>
      <HeaderContainer>
        <IconButton
          Icon={mdiCampfire}
          colour={theme.colours.yellow}
          size={4}
          onClick={console.log}
        />
        <H2>Boiler</H2>
      </HeaderContainer>
      <ContentContainer>
        <BoilerGrid>
          <IconButton
            Icon={ICON_MAP['heating']}
            colour={heatingLatest?.value ? theme.colours.red : theme.colours.foreground}
            size={3}
            backgroundOpacity={0.05}
          />
          <BoilerStateHeader state={heatingLatest?.value}>
            {heatingLatest?.value ? 'ON' : 'OFF'}
          </BoilerStateHeader>
          <p>
            Heating
            <TimestampElement>
              Since{' '}
              {heatingLatest?.timestamp?.toLocaleTimeString([], {
                hour: '2-digit',
                minute: '2-digit',
                hour12: false,
              }) || '--:--'}
            </TimestampElement>
          </p>
        </BoilerGrid>
        <BoilerGrid>
          <IconButton
            Icon={ICON_MAP['hot-water']}
            colour={hotWaterLatest?.value ? theme.colours.blue : theme.colours.foreground}
            size={3}
            backgroundOpacity={0.05}
          />
          <BoilerStateHeader state={hotWaterLatest?.value}>
            {hotWaterLatest?.value ? 'ON' : 'OFF'}
          </BoilerStateHeader>
          <p>
            Hot Water
            <TimestampElement>
              Since{' '}
              {hotWaterLatest?.timestamp?.toLocaleTimeString([], {
                hour: '2-digit',
                minute: '2-digit',
                hour12: false,
              }) || '--:--'}
            </TimestampElement>
          </p>
        </BoilerGrid>
        <TemperatureGraphContainer>
          <TankTemperatureGraph
            locations={locations}
            metrics={metrics}
            latest={tankTemperatureLatest}
          />
          <h2>
            {tankTemperatureLatest?.value?.toFixed(1) || '--.-'}
            {metrics?.[temperatureMetricId]?.unit}
            {getTimeDelta(tankTemperatureLatest?.timestamp)}
          </h2>
        </TemperatureGraphContainer>
      </ContentContainer>
    </Card>
  );
};
