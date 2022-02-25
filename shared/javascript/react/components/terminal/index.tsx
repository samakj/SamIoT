/** @format */

import { transparentize } from 'polished';
import styled from 'styled-components';

export const MCUTerminalView = styled.div`
  display: flex;
  flex-direction: column-reverse;
  box-sizing: border-box;
  height: 50vh;
  padding: 1rem;
  width: fill-available;
  background-color: ${({ theme }) => transparentize(0.9, theme.colours.white)};
  font-size: 0.75rem;
  overflow: auto;
`;

export const MCUTerminalMeasurementLine = styled.pre`
  display: grid;
  grid-template-columns: auto 10rem 5rem 1fr;
  margin: 0;
`;

export const MCUTerminalLineCell = styled.div`
  box-sizing: border-box;
  border-right: 1px solid ${({ theme }) => transparentize(0.9, theme.colours.white)};
  padding: 0.125rem 0.75rem;
  overflow: hidden;
`;

export const MCUTerminalToggle = styled.div<{ isActive?: boolean }>`
  display: inline-block;
  margin: 0.125rem 0.5rem 0 0;
  font-size: 0.75rem;
  padding: 0.125rem 0.5rem;
  height: 1rem;
  border-radius: 0.5rem;
  text-transform: uppercase;
  font-weight: bold;
  color: ${({ theme, isActive }) => (isActive ? theme.colours.black : theme.colours.white)};
  background-color: ${({ theme, isActive }) =>
    isActive ? theme.colours.white : theme.colours.black};
  transition: color 300ms, background-color 300ms;
  cursor: pointer;
`;
