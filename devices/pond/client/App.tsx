/** @format */

import React, { useState, useEffect, useRef, useCallback, useMemo } from 'react';
import { StaticThemeWrapper } from 'shared/javascript/themes';
import { DevicesHeader } from 'shared/javascript/react/components/Headers';
import { Wrapper2Rem } from 'shared/javascript/react/components/wrappers';
import {
  Horizontal1RemSpacer,
  Horizontal2RemSpacer,
  Horizontal3RemSpacer,
} from 'shared/javascript/react/components/spacers';
import {
  MCUCardGrid,
  MCUWideInfoGridCardIcon,
  MCUWideInfoEvenGridCard,
  MCUWideInfoGridCardValueText,
  MCUWideInfoGridCardTimeText,
  MCUWideInfoGridCardMediumText,
  MCUWideInfoGridCardValueRightText,
} from 'shared/javascript/react/components/layouts/mcu-card-grid';
import {
  MCUTerminalView,
  MCUTerminalMeasurementLine,
  MCUTerminalLineCell,
  MCUTerminalToggle,
} from 'shared/javascript/react/components/terminal';
import { Wifi } from 'shared/javascript/react/components/icons/wifi';
// import { IoTClient } from './clients/iot';
import { DeviceWebsocket, Measurement } from 'shared/javascript/clients/device-websocket';
import { prettyTimeDelta } from 'shared/javascript/formatters/time';
import { HOUR_IN_MS, SECOND_IN_MS } from 'shared/javascript/static/times';
import Icon from '@mdi/react';
import { mdiMessage, mdiPowerPlug } from '@mdi/js';
import { transparentize } from 'polished';
import { theme } from 'shared/javascript/themes/theme';

const placeholderFloat = (value?: string, dp: number = 1): string =>
  value ? parseFloat(value).toFixed(dp) : '...';

export const App = () => {
  const [lastMessage, setLastMessage] = useState<Date>();
  const [showTerminal, setShowTeminal] = useState(false);
  const allTags = useRef<Set<string>>(new Set());
  const allMetrics = useRef<Set<string>>(new Set());
  const [hiddenTags, setHiddenTags] = useState<Set<string>>(new Set());
  const [hiddenMetrics, setHiddenMetrics] = useState<Set<string>>(new Set());
  const [_, setTrigger] = useState(0);
  const latestSsidMeasurement = useRef<Measurement & { date: Date }>();
  const latestWifiStrengthMeasurement = useRef<Measurement & { date: Date }>();

  const onMessage = useCallback((event: WebSocketEventMap['message']) => {
    let data = null;
    try {
      data = JSON.parse(event.data) as Measurement;
    } catch (_error) {}

    setLastMessage(new Date());
    if (!data || data.type != 'measurement') return;

    if (!allMetrics.current.has(data.metric))
      allMetrics.current = new Set([...Array.from(allMetrics.current), data.metric]);

    let changed = false;
    const newTags = new Set([...Array.from(allTags.current)]);
    data.tags.forEach((tag) => {
      if (!allTags.current.has(tag)) {
        newTags.add(tag);
        changed = true;
      }
    });

    if (changed) allTags.current = newTags;
  }, []);

  const websocket = useRef(new DeviceWebsocket('ws://192.168.1.250/ws', { message: onMessage }));

  // DOM update trigger
  useEffect(() => {
    const interval = setInterval(() => setTrigger(+new Date()), 100);
    return () => clearInterval(interval);
  }, []);

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

  const terminalLines = useMemo(
    () =>
      websocket.current.messageStack
        .reverse()
        .filter((message) => {
          if (!message.data) return true;
          if (hiddenMetrics.has(message.data.metric)) return false;
          for (const tag of message.data.tags) {
            if (hiddenTags.has(tag)) return false;
          }
          return true;
        })
        .map((message, index) => (
          <MCUTerminalMeasurementLine key={index}>
            <MCUTerminalLineCell>{new Date(message.date).toLocaleString()}</MCUTerminalLineCell>
            <MCUTerminalLineCell>{message.data?.tags?.join(', ')}</MCUTerminalLineCell>
            <MCUTerminalLineCell>{message.data?.metric}</MCUTerminalLineCell>
            <MCUTerminalLineCell>{message.data?.value}</MCUTerminalLineCell>
          </MCUTerminalMeasurementLine>
        )),
    [websocket.current?.messageStack]
  );

  return (
    <StaticThemeWrapper>
      <Wrapper2Rem>
        <DevicesHeader>Pond</DevicesHeader>
        <MCUCardGrid>
          <MCUWideInfoEvenGridCard>
            <MCUWideInfoGridCardIcon>
              <Wifi
                percentage={
                  latestWifiStrengthMeasurement
                    ? parseFloat(latestWifiStrengthMeasurement.current?.value)
                    : 0
                }
              />
            </MCUWideInfoGridCardIcon>
            <MCUWideInfoGridCardMediumText style={{ gridColumn: 'span 2' }}>
              {latestSsidMeasurement?.current?.value || '...'}{' '}
              {placeholderFloat(latestWifiStrengthMeasurement?.current?.value, 0)}%
            </MCUWideInfoGridCardMediumText>
            <MCUWideInfoGridCardTimeText>
              {prettyTimeDelta(latestWifiStrengthMeasurement?.current?.date)}
            </MCUWideInfoGridCardTimeText>
          </MCUWideInfoEvenGridCard>
        </MCUCardGrid>
        <Horizontal3RemSpacer />
        <MCUCardGrid>
          <MCUWideInfoEvenGridCard>
            <MCUWideInfoGridCardIcon>
              <Icon
                path={mdiMessage}
                size={1.75}
                color={
                  +new Date() - +websocket.current?.meta?.lastMessage < 1.5 * SECOND_IN_MS
                    ? theme.colours.success
                    : transparentize(0.75, theme.colours.white)
                }
              />
            </MCUWideInfoGridCardIcon>
            <MCUWideInfoGridCardMediumText style={{ gridColumn: 'span 2' }}>
              {websocket.current?.meta?.startDate &&
              websocket.current?.meta?.messageCount?.measurement > 50
                ? Math.ceil(
                    (HOUR_IN_MS * (websocket.current?.meta?.messageCount?.measurement || 0)) /
                      (+new Date() - +websocket.current?.meta?.startDate)
                  ).toLocaleString()
                : '...'}
              /h
            </MCUWideInfoGridCardMediumText>
            <MCUWideInfoGridCardValueText>
              {websocket.current?.meta?.messageCount?.measurement?.toLocaleString()}
            </MCUWideInfoGridCardValueText>
            <MCUWideInfoGridCardValueRightText>
              {prettyTimeDelta(websocket.current?.meta?.lastMessage)}
            </MCUWideInfoGridCardValueRightText>
          </MCUWideInfoEvenGridCard>
          <MCUWideInfoEvenGridCard>
            <MCUWideInfoGridCardIcon>
              <Icon
                path={mdiPowerPlug}
                size={1.75}
                color={
                  websocket.current?.websocket?.readyState === WebSocket.OPEN
                    ? theme.colours.success
                    : transparentize(0.75, theme.colours.white)
                }
              />
            </MCUWideInfoGridCardIcon>
            <MCUWideInfoGridCardMediumText style={{ gridColumn: 'span 2' }}>
              {prettyTimeDelta(websocket.current?.meta?.lastConnect)}
            </MCUWideInfoGridCardMediumText>
            <MCUWideInfoGridCardValueText>
              {websocket.current?.meta?.reconnectCount}
            </MCUWideInfoGridCardValueText>
            <MCUWideInfoGridCardValueRightText>
              {websocket.current?.meta?.startDate
                ? (
                    (HOUR_IN_MS * (websocket.current?.meta?.reconnectCount || 0)) /
                    (+new Date() - +websocket.current?.meta?.startDate)
                  ).toFixed(2)
                : '...'}
              /h
            </MCUWideInfoGridCardValueRightText>
          </MCUWideInfoEvenGridCard>
        </MCUCardGrid>
        <Horizontal2RemSpacer />
        <div style={{ cursor: 'pointer' }} onClick={() => setShowTeminal(!showTerminal)}>
          {showTerminal ? 'Hide' : 'Show'} Terminal
        </div>
        <Horizontal1RemSpacer />
        {showTerminal && (
          <>
            <div style={{ fontSize: '0.875rem' }}>Metrics</div>
            <div>
              {Array.from(allMetrics.current)
                .sort()
                .map((metric) => (
                  <MCUTerminalToggle
                    key={metric}
                    onClick={() => {
                      const newMetrics = new Set([...Array.from(hiddenMetrics)]);
                      if (hiddenMetrics.has(metric)) newMetrics.delete(metric);
                      else newMetrics.add(metric);
                      setHiddenMetrics(newMetrics);
                    }}
                    isActive={!hiddenMetrics.has(metric)}
                  >
                    {metric}
                  </MCUTerminalToggle>
                ))}
            </div>
            <Horizontal1RemSpacer />
            <div style={{ fontSize: '0.875rem' }}>Tags</div>
            <div>
              {Array.from(allTags.current)
                .sort()
                .map((tag) => (
                  <MCUTerminalToggle
                    key={tag}
                    onClick={() => {
                      const newTags = new Set([...Array.from(hiddenTags)]);
                      if (hiddenTags.has(tag)) newTags.delete(tag);
                      else newTags.add(tag);
                      setHiddenTags(newTags);
                    }}
                    isActive={!hiddenTags.has(tag)}
                  >
                    {tag}
                  </MCUTerminalToggle>
                ))}
            </div>
            <Horizontal1RemSpacer />
            <MCUTerminalView>{terminalLines}</MCUTerminalView>
          </>
        )}
      </Wrapper2Rem>
    </StaticThemeWrapper>
  );
};
