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
              {latestBatteryPercentageMeasurement
                ? parseFloat(latestBatteryPercentageMeasurement.value).toFixed(0)
                : '...'}
              %
            </MCUWideInfoGridCardLargeText>
            <MCUWideInfoGridCardValueText>
              {latestBatteryVoltageMeasurement
                ? parseFloat(latestBatteryVoltageMeasurement.value).toFixed(1)
                : '...'}
              V
            </MCUWideInfoGridCardValueText>
            <MCUWideInfoGridCardValueText>
              {latestBatteryCurrentMeasurement
                ? parseFloat(latestBatteryCurrentMeasurement.value).toFixed(1)
                : '...'}
              A
            </MCUWideInfoGridCardValueText>
            <MCUWideInfoGridCardTimeText>
              {latestBatteryPercentageMeasurement
                ? Math.ceil(
                    (+new Date() - +new Date(latestBatteryPercentageMeasurement.timestamp)) / 1000
                  )
                : '...'}
              s ago
            </MCUWideInfoGridCardTimeText>
            <MCUWideInfoGridCardValueText>
              {latestBatteryTemperatureMeasurement
                ? parseFloat(latestBatteryTemperatureMeasurement.value).toFixed(1)
                : '...'}
              c
            </MCUWideInfoGridCardValueText>
          </MCUWideInfoMiddleGridCard>
          <MCUWideInfoMiddleGridCard>
            <MCUWideInfoGridCardIcon></MCUWideInfoGridCardIcon>
            <MCUWideInfoGridCardMediumText>
              {latestPanelVoltageMeasurement
                ? parseFloat(latestPanelVoltageMeasurement.value).toFixed(1)
                : '...'}
              V
            </MCUWideInfoGridCardMediumText>
            <MCUWideInfoGridCardMediumText>
              {latestPanelCurrentMeasurement
                ? parseFloat(latestPanelCurrentMeasurement.value).toFixed(1)
                : '...'}
              A
            </MCUWideInfoGridCardMediumText>
            <MCUWideInfoGridCardTimeText>
              {latestPanelVoltageMeasurement
                ? Math.ceil(
                    (+new Date() - +new Date(latestPanelVoltageMeasurement.timestamp)) / 1000
                  )
                : '...'}
              s ago
            </MCUWideInfoGridCardTimeText>
            <MCUWideInfoGridCardTimeText>
              {latestPanelCurrentMeasurement
                ? Math.ceil(
                    (+new Date() - +new Date(latestPanelCurrentMeasurement.timestamp)) / 1000
                  )
                : '...'}
              s ago
            </MCUWideInfoGridCardTimeText>
          </MCUWideInfoMiddleGridCard>
        </MCUCardGrid>
        <Horizontal2RemSpacer />
        <MCUCardGrid>
          <MCUButtonCard>
            <MCUButtonCardIconContainer isActive={latestInverterMeasurement?.value === 'true'}>
              {!latestInverterMeasurement
                ? '...'
                : latestInverterMeasurement.value === 'true'
                ? 'ON'
                : 'OFF'}
            </MCUButtonCardIconContainer>
            <MCUButtonCardTitle>Inverter</MCUButtonCardTitle>
          </MCUButtonCard>
          <MCUButtonCard>
            <MCUButtonCardIconContainer isActive={latestPondMeasurement?.value === 'true'}>
              {!latestPondMeasurement
                ? '...'
                : latestPondMeasurement.value === 'true'
                ? 'ON'
                : 'OFF'}
            </MCUButtonCardIconContainer>
            <MCUButtonCardTitle>Pond</MCUButtonCardTitle>
          </MCUButtonCard>
          <MCUButtonCard>
            <MCUButtonCardIconContainer isActive={latestSwitchedMeasurement?.value === 'true'}>
              {!latestSwitchedMeasurement
                ? '...'
                : latestSwitchedMeasurement.value === 'true'
                ? 'ON'
                : 'OFF'}
            </MCUButtonCardIconContainer>
            <MCUButtonCardTitle>Switched</MCUButtonCardTitle>
          </MCUButtonCard>
          <MCUButtonCard>
            <MCUButtonCardIconContainer isActive={latestSolarMeasurement?.value === 'true'}>
              {!latestSolarMeasurement
                ? '...'
                : latestSolarMeasurement.value === 'true'
                ? 'ON'
                : 'OFF'}
            </MCUButtonCardIconContainer>
            <MCUButtonCardTitle>Solar</MCUButtonCardTitle>
          </MCUButtonCard>
          <MCUButtonCard>
            <MCUButtonCardIconContainer isActive={latestExcessMeasurement?.value === 'true'}>
              {!latestExcessMeasurement
                ? '...'
                : latestExcessMeasurement.value === 'true'
                ? 'ON'
                : 'OFF'}
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
              {latestWifiStrengthMeasurement
                ? parseFloat(latestWifiStrengthMeasurement.value).toFixed(0)
                : '...'}
              %
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
