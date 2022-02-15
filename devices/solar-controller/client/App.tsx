/** @format */

import React, { useState, useEffect } from 'react';
import { StaticThemeWrapper } from 'shared/javascript/themes';
import { DevicesHeader } from 'shared/javascript/react/components/Headers';
import { Wrapper2Rem } from 'shared/javascript/react/components/wrappers';
import {
  Horizontal2RemSpacer,
  Horizontal3RemSpacer,
} from 'shared/javascript/react/components/spacers';
import {
  MCUCardGrid,
  MCUButtonCard,
  MCUButtonCardIconContainer,
  MCUButtonCardTitle,
  MCUWideInfoGridCardIcon,
  MCUWideInfoMiddleGridCard,
  MCUWideInfoEvenGridCard,
  MCUWideInfoGridCardLargeText,
  MCUWideInfoGridCardValueText,
  MCUWideInfoGridCardTimeText,
  MCUWideInfoGridCardMediumText,
} from 'shared/javascript/react/components/layouts/mcu-card-grid';
import {
  MCUTerminalView,
  MCUTerminalMeasurementLine,
  MCUTerminalLineCell,
} from 'shared/javascript/react/components/terminal';
import { Battery } from 'shared/javascript/react/components/icons/battery';
import { Wifi } from 'shared/javascript/react/components/icons/wifi';
import { IoTClient } from './clients/iot';
import { MetricsStateType } from './types';

export const App = () => {
  const [metrics, setMetrics] = useState<MetricsStateType>();

  useEffect(() => {
    IoTClient.metrics.getMetrics().then((metricsResponse) =>
      setMetrics(
        metricsResponse.data.reduce(
          (acc, metric) => ({
            ...acc,
            [metric.id]: metric,
            [metric.name]: metric,
            [metric.abbreviation]: metric,
          }),
          {}
        )
      )
    );
  }, []);

  console.log(metrics);

  return (
    <StaticThemeWrapper>
      <Wrapper2Rem>
        <DevicesHeader>Solar Controller</DevicesHeader>
        <MCUCardGrid>
          <MCUWideInfoMiddleGridCard>
            <MCUWideInfoGridCardIcon>
              <Battery percentage={95} />
            </MCUWideInfoGridCardIcon>
            <MCUWideInfoGridCardLargeText>68%</MCUWideInfoGridCardLargeText>
            <MCUWideInfoGridCardValueText>28.7V</MCUWideInfoGridCardValueText>
            <MCUWideInfoGridCardValueText>1.23A</MCUWideInfoGridCardValueText>
            <MCUWideInfoGridCardTimeText>10m ago</MCUWideInfoGridCardTimeText>
            <MCUWideInfoGridCardValueText>10.2c</MCUWideInfoGridCardValueText>
          </MCUWideInfoMiddleGridCard>
          <MCUWideInfoMiddleGridCard>
            <MCUWideInfoGridCardIcon></MCUWideInfoGridCardIcon>
            <MCUWideInfoGridCardMediumText>35.9V</MCUWideInfoGridCardMediumText>
            <MCUWideInfoGridCardMediumText>1.1A</MCUWideInfoGridCardMediumText>
            <MCUWideInfoGridCardTimeText>10m ago</MCUWideInfoGridCardTimeText>
            <MCUWideInfoGridCardTimeText>10m ago</MCUWideInfoGridCardTimeText>
          </MCUWideInfoMiddleGridCard>
        </MCUCardGrid>
        <Horizontal2RemSpacer />
        <MCUCardGrid>
          <MCUButtonCard>
            <MCUButtonCardIconContainer>OFF</MCUButtonCardIconContainer>
            <MCUButtonCardTitle>Inverter</MCUButtonCardTitle>
          </MCUButtonCard>
          <MCUButtonCard>
            <MCUButtonCardIconContainer isActive>ON</MCUButtonCardIconContainer>
            <MCUButtonCardTitle>Pond</MCUButtonCardTitle>
          </MCUButtonCard>
          <MCUButtonCard>
            <MCUButtonCardIconContainer>OFF</MCUButtonCardIconContainer>
            <MCUButtonCardTitle>Switched</MCUButtonCardTitle>
          </MCUButtonCard>
          <MCUButtonCard>
            <MCUButtonCardIconContainer>OFF</MCUButtonCardIconContainer>
            <MCUButtonCardTitle>Solar</MCUButtonCardTitle>
          </MCUButtonCard>
          <MCUButtonCard>
            <MCUButtonCardIconContainer>OFF</MCUButtonCardIconContainer>
            <MCUButtonCardTitle>Excess</MCUButtonCardTitle>
          </MCUButtonCard>
        </MCUCardGrid>
        <Horizontal3RemSpacer />
        <MCUCardGrid>
          <MCUWideInfoEvenGridCard>
            <MCUWideInfoGridCardIcon>
              <Wifi percentage={68} />
            </MCUWideInfoGridCardIcon>
            <MCUWideInfoGridCardMediumText style={{ gridColumn: 'span 2' }}>
              Selma 68%
            </MCUWideInfoGridCardMediumText>
            <MCUWideInfoGridCardTimeText>10m ago</MCUWideInfoGridCardTimeText>
          </MCUWideInfoEvenGridCard>
        </MCUCardGrid>
        <Horizontal3RemSpacer />
        <MCUCardGrid>
          <MCUWideInfoEvenGridCard>
            <MCUWideInfoGridCardIcon></MCUWideInfoGridCardIcon>
            <MCUWideInfoGridCardMediumText style={{ gridColumn: 'span 2' }}>
              12,345/h
            </MCUWideInfoGridCardMediumText>
            <MCUWideInfoGridCardValueText>12,345</MCUWideInfoGridCardValueText>
            <MCUWideInfoGridCardValueText>987ms</MCUWideInfoGridCardValueText>
          </MCUWideInfoEvenGridCard>
          <MCUWideInfoEvenGridCard>
            <MCUWideInfoGridCardIcon></MCUWideInfoGridCardIcon>
            <MCUWideInfoGridCardMediumText style={{ gridColumn: 'span 2' }}>
              5m 41s
            </MCUWideInfoGridCardMediumText>
            <MCUWideInfoGridCardValueText>2</MCUWideInfoGridCardValueText>
            <MCUWideInfoGridCardValueText>0.2/h</MCUWideInfoGridCardValueText>
          </MCUWideInfoEvenGridCard>
        </MCUCardGrid>
        <Horizontal2RemSpacer />
        <MCUTerminalView>
          <MCUTerminalMeasurementLine>
            <MCUTerminalLineCell>2022-02-14 21:57:34</MCUTerminalLineCell>
            <MCUTerminalLineCell>solar, battery, remote</MCUTerminalLineCell>
            <MCUTerminalLineCell>temperature</MCUTerminalLineCell>
            <MCUTerminalLineCell>0.8</MCUTerminalLineCell>
          </MCUTerminalMeasurementLine>
          <MCUTerminalMeasurementLine>
            <MCUTerminalLineCell>2022-02-14 21:57:34</MCUTerminalLineCell>
            <MCUTerminalLineCell>solar, load</MCUTerminalLineCell>
            <MCUTerminalLineCell>power</MCUTerminalLineCell>
            <MCUTerminalLineCell>0.8</MCUTerminalLineCell>
          </MCUTerminalMeasurementLine>
          <MCUTerminalMeasurementLine>
            <MCUTerminalLineCell>2022-02-14 21:57:34</MCUTerminalLineCell>
            <MCUTerminalLineCell>solar, load</MCUTerminalLineCell>
            <MCUTerminalLineCell>power</MCUTerminalLineCell>
            <MCUTerminalLineCell>0.8</MCUTerminalLineCell>
          </MCUTerminalMeasurementLine>
          <MCUTerminalMeasurementLine>
            <MCUTerminalLineCell>2022-02-14 21:57:34</MCUTerminalLineCell>
            <MCUTerminalLineCell>solar, load</MCUTerminalLineCell>
            <MCUTerminalLineCell>power</MCUTerminalLineCell>
            <MCUTerminalLineCell>0.8</MCUTerminalLineCell>
          </MCUTerminalMeasurementLine>
          <MCUTerminalMeasurementLine>
            <MCUTerminalLineCell>2022-02-14 21:57:34</MCUTerminalLineCell>
            <MCUTerminalLineCell>solar, load</MCUTerminalLineCell>
            <MCUTerminalLineCell>power</MCUTerminalLineCell>
            <MCUTerminalLineCell>0.8</MCUTerminalLineCell>
          </MCUTerminalMeasurementLine>
        </MCUTerminalView>
      </Wrapper2Rem>
    </StaticThemeWrapper>
  );
};
