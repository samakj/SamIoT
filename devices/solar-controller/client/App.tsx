/** @format */

import React, { useState, useEffect, useRef, useCallback } from 'react';
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
// import { IoTClient } from './clients/iot';
import { MetricsStateType } from './types';
import { DeviceWebsocket, Measurement } from 'shared/javascript/clients/device-websocket';
import { prettyTimeDelta } from 'shared/javascript/formatters/time';

const placeholderFloat = (value?: string, dp: number = 1): string =>
  value ? parseFloat(value).toFixed(dp) : '...';
const placeholderOnOff = (value?: string): string =>
  !value ? '...' : value === 'true' ? 'ON' : 'OFF';

export const App = () => {
  const [metrics, setMetrics] = useState<MetricsStateType>();
  const [lastMessage, setLastMessage] = useState<Date>();
  const [latestBatteryPercentageMeasurement, setLatestBatteryPercentageMeasurement] =
    useState<Measurement>();
  const [latestBatteryVoltageMeasurement, setLatestBatteryVoltageMeasurement] =
    useState<Measurement>();
  const [latestBatteryCurrentMeasurement, setLatestBatteryCurrentMeasurement] =
    useState<Measurement>();
  const [latestBatteryTemperatureMeasurement, setLatestBatteryTemperatureMeasurement] =
    useState<Measurement>();
  const [latestPanelVoltageMeasurement, setLatestPanelVoltageMeasurement] = useState<Measurement>();
  const [latestPanelCurrentMeasurement, setLatestPanelCurrentMeasurement] = useState<Measurement>();
  const [latestInverterMeasurement, setLatestInverterMeasurement] = useState<Measurement>();
  const [latestPondMeasurement, setLatestPondMeasurement] = useState<Measurement>();
  const [latestSwitchedMeasurement, setLatestSwitchedMeasurement] = useState<Measurement>();
  const [latestSolarMeasurement, setLatestSolarMeasurement] = useState<Measurement>();
  const [latestExcessMeasurement, setLatestExcessMeasurement] = useState<Measurement>();
  const [latestSsidMeasurement, setLatestSsidMeasurement] = useState<Measurement>();
  const [latestWifiStrengthMeasurement, setLatestWifiStrengthMeasurement] = useState<Measurement>();

  const onMessage = useCallback((event: WebSocketEventMap['message']) => {
    let data = null;
    try {
      data = JSON.parse(event.data) as Measurement;
    } catch (_error) {}

    setLastMessage(new Date());
    if (!data || data.type != 'measurement') return;

    if (data.metric === 'temperature') {
      if (data.tags.includes('battery')) setLatestBatteryTemperatureMeasurement(data);
    }
    if (data.metric === 'current') {
      if (data.tags.includes('battery')) setLatestBatteryCurrentMeasurement(data);
      if (data.tags.includes('panel')) setLatestPanelCurrentMeasurement(data);
    }
    if (data.metric === 'voltage') {
      if (data.tags.includes('battery')) setLatestBatteryVoltageMeasurement(data);
      if (data.tags.includes('panel')) setLatestPanelVoltageMeasurement(data);
    }
    if (data.metric === 'percentage') {
      if (data.tags.includes('battery')) setLatestBatteryPercentageMeasurement(data);
    }
    if (data.metric === 'on') {
      if (data.tags.includes('inverter')) setLatestInverterMeasurement(data);
      if (data.tags.includes('relay1')) setLatestSwitchedMeasurement(data);
      if (data.tags.includes('relay2')) setLatestPondMeasurement(data);
      if (data.tags.includes('relay6')) setLatestSolarMeasurement(data);
      if (data.tags.includes('relay8')) setLatestExcessMeasurement(data);
    }
    if (data.metric === 'strength') {
      if (data.tags.includes('wifi')) setLatestWifiStrengthMeasurement(data);
    }
    if (data.metric === 'ssid') {
      if (data.tags.includes('wifi')) setLatestSsidMeasurement(data);
    }
  }, []);

  const websocket = useRef(new DeviceWebsocket('ws://192.168.1.250/ws', { message: onMessage }));

  useEffect(() => {
    websocket.current.start();
    // IoTClient.metrics.getMetrics().then((metricsResponse) =>
    //   setMetrics(
    //     metricsResponse.data.reduce(
    //       (acc, metric) => ({
    //         ...acc,
    //         [metric.id]: metric,
    //         [metric.name]: metric,
    //         [metric.abbreviation]: metric,
    //       }),
    //       {}
    //     )
    //   )
    // );
    return () => websocket.current.stop();
  }, []);

  return (
    <StaticThemeWrapper>
      <Wrapper2Rem>
        <DevicesHeader>Solar Controller</DevicesHeader>
        <MCUCardGrid>
          <MCUWideInfoMiddleGridCard>
            <MCUWideInfoGridCardIcon>
              <Battery
                percentage={
                  latestBatteryPercentageMeasurement
                    ? parseFloat(latestBatteryPercentageMeasurement.value)
                    : 0
                }
              />
            </MCUWideInfoGridCardIcon>
            <MCUWideInfoGridCardLargeText>
              {placeholderFloat(latestBatteryPercentageMeasurement?.value, 0)}%
            </MCUWideInfoGridCardLargeText>
            <MCUWideInfoGridCardValueText>
              {placeholderFloat(latestBatteryVoltageMeasurement?.value)}V
            </MCUWideInfoGridCardValueText>
            <MCUWideInfoGridCardValueText>
              {placeholderFloat(latestBatteryCurrentMeasurement?.value)}A
            </MCUWideInfoGridCardValueText>
            <MCUWideInfoGridCardTimeText>
              {prettyTimeDelta(latestBatteryPercentageMeasurement?.timestamp)}
            </MCUWideInfoGridCardTimeText>
            <MCUWideInfoGridCardValueText>
              {placeholderFloat(latestBatteryTemperatureMeasurement?.value)}c
            </MCUWideInfoGridCardValueText>
          </MCUWideInfoMiddleGridCard>
          <MCUWideInfoMiddleGridCard>
            <MCUWideInfoGridCardIcon></MCUWideInfoGridCardIcon>
            <MCUWideInfoGridCardMediumText>
              {placeholderFloat(latestPanelVoltageMeasurement?.value)}V
            </MCUWideInfoGridCardMediumText>
            <MCUWideInfoGridCardMediumText>
              {placeholderFloat(latestPanelCurrentMeasurement?.value)}A
            </MCUWideInfoGridCardMediumText>
            <MCUWideInfoGridCardTimeText>
              {prettyTimeDelta(latestPanelVoltageMeasurement?.timestamp)}
            </MCUWideInfoGridCardTimeText>
            <MCUWideInfoGridCardTimeText>
              {prettyTimeDelta(latestPanelCurrentMeasurement?.timestamp)}
            </MCUWideInfoGridCardTimeText>
          </MCUWideInfoMiddleGridCard>
        </MCUCardGrid>
        <Horizontal2RemSpacer />
        <MCUCardGrid>
          <MCUButtonCard>
            <MCUButtonCardIconContainer isActive={latestInverterMeasurement?.value === 'true'}>
              {placeholderOnOff(latestInverterMeasurement?.value)}
            </MCUButtonCardIconContainer>
            <MCUButtonCardTitle>Inverter</MCUButtonCardTitle>
          </MCUButtonCard>
          <MCUButtonCard>
            <MCUButtonCardIconContainer isActive={latestPondMeasurement?.value === 'true'}>
              {placeholderOnOff(latestPondMeasurement?.value)}
            </MCUButtonCardIconContainer>
            <MCUButtonCardTitle>Pond</MCUButtonCardTitle>
          </MCUButtonCard>
          <MCUButtonCard>
            <MCUButtonCardIconContainer isActive={latestSwitchedMeasurement?.value === 'true'}>
              {placeholderOnOff(latestSwitchedMeasurement?.value)}
            </MCUButtonCardIconContainer>
            <MCUButtonCardTitle>Switched</MCUButtonCardTitle>
          </MCUButtonCard>
          <MCUButtonCard>
            <MCUButtonCardIconContainer isActive={latestSolarMeasurement?.value === 'true'}>
              {placeholderOnOff(latestSolarMeasurement?.value)}
            </MCUButtonCardIconContainer>
            <MCUButtonCardTitle>Solar</MCUButtonCardTitle>
          </MCUButtonCard>
          <MCUButtonCard>
            <MCUButtonCardIconContainer isActive={latestExcessMeasurement?.value === 'true'}>
              {placeholderOnOff(latestExcessMeasurement?.value)}
            </MCUButtonCardIconContainer>
            <MCUButtonCardTitle>Excess</MCUButtonCardTitle>
          </MCUButtonCard>
        </MCUCardGrid>
        <Horizontal3RemSpacer />
        <MCUCardGrid>
          <MCUWideInfoEvenGridCard>
            <MCUWideInfoGridCardIcon>
              <Wifi
                percentage={
                  latestWifiStrengthMeasurement
                    ? parseFloat(latestWifiStrengthMeasurement.value)
                    : 0
                }
              />
            </MCUWideInfoGridCardIcon>
            <MCUWideInfoGridCardMediumText style={{ gridColumn: 'span 2' }}>
              {latestSsidMeasurement?.value || '...'}{' '}
              {placeholderFloat(latestWifiStrengthMeasurement?.value, 0)}%
            </MCUWideInfoGridCardMediumText>
            <MCUWideInfoGridCardTimeText>
              {prettyTimeDelta(latestWifiStrengthMeasurement?.timestamp)}
            </MCUWideInfoGridCardTimeText>
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
          {websocket.current.messageStack.reverse().map((message, index) => (
            <MCUTerminalMeasurementLine key={index}>
              <MCUTerminalLineCell>{new Date(message.date).toLocaleString()}</MCUTerminalLineCell>
              <MCUTerminalLineCell>{message.data?.tags?.join(', ')}</MCUTerminalLineCell>
              <MCUTerminalLineCell>{message.data?.metric}</MCUTerminalLineCell>
              <MCUTerminalLineCell>{message.data?.value}</MCUTerminalLineCell>
            </MCUTerminalMeasurementLine>
          ))}
        </MCUTerminalView>
      </Wrapper2Rem>
    </StaticThemeWrapper>
  );
};
