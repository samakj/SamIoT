import { transparentize } from "polished";
import styled from "styled-components";

export const MCUTerminalView = styled.div`
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
  grid-template-columns: 10rem 10rem 5rem 1fr;
  margin: 0;
`;

export const MCUTerminalLineCell = styled.div`
  box-sizing: border-box;
  border-right: 1px solid
    ${({ theme }) => transparentize(0.9, theme.colours.white)};
  padding: 0.125rem 0.75rem;
  overflow: hidden;
`;
