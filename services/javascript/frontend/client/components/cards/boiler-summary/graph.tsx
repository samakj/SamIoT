/** @format */

import { useAverageMeasurements } from 'client/store/iot/measurements/hooks';
import React, { useCallback, useMemo, useState } from 'react';
import { HOUR_IN_MS, MINUTE_IN_MS } from 'shared/javascript/static/times';
import { MeasurementAverageType } from 'shared/javascript/types/iot';
import { TankTemperatureGraphPropsType } from './types';
import { scaleLinear, scaleTime } from '@visx/scale';
import { useMeasure } from 'react-use';
import styled from 'styled-components';
import { Group } from '@visx/group';
import { AreaClosed, Circle, Line, LinePath } from '@visx/shape';
import { curveMonotoneX } from '@visx/curve';
import { TooltipWithBounds, useTooltip, defaultStyles } from '@visx/tooltip';
import { localPoint } from '@visx/event';
import { Text } from '@visx/text';
import { bisector } from 'd3-array';
import { useTheme } from 'styled-components';
import { transparentize } from 'polished';
import { LinearGradient } from '@visx/gradient';

const GraphContainer = styled.div`
  position: relative;
  overflow: hidden;
`;

const GraphSVG = styled.svg`
  display: block;
  height: 100%;
  width: 100%;
`;

const bisectDate = bisector<MeasurementAverageType, Date>((point) => new Date(point.start)).left;

const TOOLTIP_TRANSITION = '100ms';

export const TankTemperatureGraph = ({
  locations,
  metrics,
  latest,
  ...rest
}: TankTemperatureGraphPropsType) => {
  const [ref, { height = 0, width = 0 }] = useMeasure();
  const theme = useTheme();
  const [margin, _setMargin] = useState({ top: 0, bottom: 0, left: 0, right: 0 });
  const [tooltipStyles, _setTooltipStyles] = useState({
    ...defaultStyles,
    backgroundColor: theme.colours.background,
    color: theme.colours.foreground,
    transition: TOOLTIP_TRANSITION,
  });
  const { tooltipData, showTooltip, hideTooltip } = useTooltip<MeasurementAverageType>();

  const babyRoomLocationId = useMemo(
    () =>
      locations && Object.values(locations).find((location) => location.name === 'baby-room')?.id,
    [locations]
  );

  const temperatureMetricId = useMemo(
    () => metrics && Object.values(metrics).find((metric) => metric.name === 'temperature')?.id,
    [metrics]
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

  const tankTemperatureAverages = useAverageMeasurements(
    tankTemperatureAverageFilters?.location_id,
    tankTemperatureAverageFilters?.metric_id,
    tankTemperatureAverageFilters?.tags,
    tankTemperatureAverageFilters
  );

  const sortedTankTemperatureAverages = useMemo(() => {
    if (!tankTemperatureAverages || !latest) return [];
    const averages = Object.values(tankTemperatureAverages);
    return [
      ...averages.sort((measurementA, measurementB) => {
        if (measurementA.timestamp > measurementB.timestamp) return 1;
        if (measurementA.timestamp < measurementB.timestamp) return -1;
        return 0;
      }),
      { start: new Date(), average: latest.value },
    ];
  }, [tankTemperatureAverages, latest]);

  const getDate = useCallback((average: MeasurementAverageType) => new Date(average.start), []);
  const getValue = useCallback((average: MeasurementAverageType) => average.average, []);

  const domain = useMemo(
    () =>
      sortedTankTemperatureAverages.reduce(
        (acc, average) => {
          if (average.average && average.average < acc.temperature[0])
            acc.temperature[0] = average.average;
          if (average.average && average.average > acc.temperature[1])
            acc.temperature[1] = average.average;
          if (average.start < acc.timestamp[0]) acc.timestamp[0] = average.start;
          if (average.start > acc.timestamp[1]) acc.timestamp[1] = average.start;
          return acc;
        },
        { temperature: [100, 0], timestamp: [1e15, 0] }
      ),
    [sortedTankTemperatureAverages]
  );

  const range = useMemo(
    () => ({
      top: margin.top,
      left: margin.left,
      right: width - margin.right,
      bottom: height - margin.bottom,
    }),
    [height, width, margin]
  );

  const timestampScale = useMemo(
    () => scaleTime({ domain: domain.timestamp, range: [range.left, range.right - 24] }),
    [domain, range]
  );
  const temperatureScale = useMemo(
    () =>
      scaleLinear({
        domain: domain.temperature,
        range: [range.bottom - 24, range.top + 16],
        nice: true,
      }),
    [domain, range]
  );

  const onPointerMove = (event: React.PointerEvent<SVGElement>) => {
    const { x } = localPoint(event.currentTarget, event);
    const x0 = timestampScale.invert(x);
    const index = bisectDate(sortedTankTemperatureAverages, x0, 1);
    const point0 = sortedTankTemperatureAverages[index - 1];
    const point1 = sortedTankTemperatureAverages[index];
    let point = point0;
    if (point1 && getDate(point1)) {
      point =
        x0.valueOf() - getDate(point0).valueOf() > getDate(point1).valueOf() - x0.valueOf()
          ? point1
          : point0;
    }
    showTooltip({
      tooltipLeft: x,
      tooltipData: point,
    });
  };

  return (
    <GraphContainer ref={ref} {...rest}>
      <GraphSVG
        viewBox={`0 0 ${Math.floor(width)} ${Math.floor(height)}`}
        onPointerEnter={onPointerMove}
        onPointerMove={onPointerMove}
        onPointerLeave={hideTooltip}
      >
        <LinearGradient
          id="gradient"
          from={theme.colours.foreground}
          fromOpacity={0.5}
          to={theme.colours.foreground}
          toOpacity={0}
          toOffset={0.9}
        />
        {sortedTankTemperatureAverages.length && (
          <>
            <AreaClosed<MeasurementAverageType>
              data={sortedTankTemperatureAverages}
              x={(point) => timestampScale(getDate(point))}
              y={(point) => temperatureScale(getValue(point))}
              yScale={temperatureScale}
              curve={curveMonotoneX}
              fill="url(#gradient)"
            />
            <LinePath<MeasurementAverageType>
              data={sortedTankTemperatureAverages}
              x={(point) => timestampScale(getDate(point))}
              y={(point) => temperatureScale(getValue(point))}
              curve={curveMonotoneX}
              stroke={theme.colours.foreground}
              strokeWidth={2}
            />
          </>
        )}
        {tooltipData && (
          <>
            <Line
              from={{ x: 0, y: range.top }}
              to={{ x: 0, y: range.bottom - 20 }}
              stroke={transparentize(0.8, theme.colours.foreground)}
              strokeWidth={2}
              strokeDasharray="5 5"
              pointerEvents="none"
              style={{
                transition: TOOLTIP_TRANSITION,
                transform: `translateX(${timestampScale(tooltipData.start)}px)`,
              }}
            />
            <Circle
              cx={0}
              cy={0}
              r={4}
              fill={theme.colours.foreground}
              pointerEvents="none"
              style={{
                transition: TOOLTIP_TRANSITION,
                transform: `translate(${timestampScale(tooltipData.start)}px, ${temperatureScale(
                  tooltipData.average
                )}px)`,
              }}
            />
            <Text
              x={0}
              y={range.bottom - 8}
              fill={theme.colours.foreground}
              textAnchor="middle"
              verticalAnchor="end"
              pointerEvents="none"
              style={{
                transition: TOOLTIP_TRANSITION,
                transform: `translateX(${
                  timestampScale(tooltipData.start) +
                  (tooltipData.start === sortedTankTemperatureAverages[0]?.start ? 16 : 0)
                }px)`,
              }}
            >
              {tooltipData?.start?.toLocaleTimeString([], {
                hour: '2-digit',
                minute: '2-digit',
                hour12: false,
              })}
            </Text>
          </>
        )}
      </GraphSVG>
      {tooltipData && (
        <>
          <TooltipWithBounds
            top={temperatureScale(tooltipData.average) - 24}
            left={timestampScale(tooltipData.start)}
            style={tooltipStyles}
          >
            {tooltipData?.average?.toFixed(1)}
            {metrics?.[temperatureMetricId].unit}
          </TooltipWithBounds>
        </>
      )}
    </GraphContainer>
  );
};
