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

const TimestampElement = styled.div`
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
  const tankTemperatureAverageFilters = useMemo(() => {
    if (!temperatureMetricId) return undefined;
    const period = 5 * MINUTE_IN_MS;
    const end = new Date(Math.floor(+new Date() / period) * period);
    const start = new Date(+end - 1.5 * HOUR_IN_MS);

    return {
      location_id: babyRoomLocationId,
      metric_id: temperatureMetricId,
      tags: ['tank'],
      start,
      end,
      period,
    };
  }, [temperatureMetricId, babyRoomLocationId]);

  const heatingState = useLatestMeasurements(heatingStateFilters);
  const hotWaterState = useLatestMeasurements(hotWaterStateFilters);
  const tankTemperatureLatest = useLatestMeasurements(tankTemperatureFilters);
  const tankTemperatureAverages = useAverageMeasurements(
    tankTemperatureAverageFilters?.location_id,
    tankTemperatureAverageFilters?.metric_id,
    tankTemperatureAverageFilters?.tags,
    tankTemperatureAverageFilters
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

  console.log(tankTemperatureLatest);

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
            colour={theme.colours.foreground}
            size={3}
            backgroundOpacity={0.05}
          />
          <H3>ON</H3>
          <p>
            Heating
            <TimestampElement>Since 9:02</TimestampElement>
          </p>
        </BoilerGrid>
        <BoilerGrid>
          <IconButton
            Icon={ICON_MAP['hot-water']}
            colour={theme.colours.foreground}
            size={3}
            backgroundOpacity={0.05}
          />
          <H3>ON</H3>
          <p>
            Hot Water
            <TimestampElement>Since 9:02</TimestampElement>
          </p>
        </BoilerGrid>
        <TemperatureGraphContainer>
          <div />
          <h2>
            {(tankTemperatureLatest &&
              Object.values(tankTemperatureLatest)[0]?.value?.toFixed(1)) ||
              '--.-'}
            {metrics?.[temperatureMetricId]?.unit}
            {getTimeDelta(
              tankTemperatureLatest && Object.values(tankTemperatureLatest)[0]?.timestamp
            )}
          </h2>
        </TemperatureGraphContainer>
      </ContentContainer>
    </Card>
  );
};
