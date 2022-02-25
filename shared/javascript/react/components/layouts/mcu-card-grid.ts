/** @format */

import styled from 'styled-components';
import { transparentize } from 'polished';

export const MCUCardGrid = styled.div`
  display: grid;
  grid-template-columns: repeat(auto-fill, 7.75rem);
  grid-gap: 2rem;
  width: fill-available;
`;

export const MCUCard = styled.div`
  box-sizing: border-box;
  height: 6rem;
  padding: 1rem;
  width: fill-available;
  background-color: ${({ theme }) => transparentize(0.9, theme.colours.white)};
`;

export const MCUWideCard = styled(MCUCard)`
  grid-column: span 2;
`;

export const MCUButtonCard = styled(MCUCard)`
  display: grid;
  grid-template-columns: auto;
  grid-template-rows: 1fr auto;
  cursor: pointer;
  transition: background-color 300ms;
  grid-template-areas: 'icon' 'title';

  &:hover {
    background-color: ${({ theme }) => transparentize(0.8, theme.colours.white)};
  }
`;

export const MCUButtonCardIconContainer = styled.div<{ isActive?: boolean }>`
  grid-area: icon;
  font-weight: 600;
  font-size: 1.75rem;
  line-height: 1.75rem;
  color: ${({ theme, isActive }) =>
    isActive ? theme.colours.success : transparentize(0.5, theme.colours.white)};
`;

export const MCUButtonCardTitle = styled.div`
  grid-area: title;
  font-weight: 600;
  font-size: 0.875rem;
  line-height: 0.875rem;
  text-transform: uppercase;
`;

export const MCUWideInfoEvenGridCard = styled(MCUWideCard)`
  display: grid;
  grid-template-columns: 1fr 1fr 1fr;
  grid-template-rows: 1fr 1fr 1fr;
  align-items: center;
`;

export const MCUWideInfoMiddleGridCard = styled(MCUWideCard)`
  display: grid;
  grid-template-columns: auto 1fr auto;
  grid-template-rows: 1fr 1fr 1fr;
  align-items: center;
  grid-column-gap: 2rem;
`;

export const MCUWideInfoGridCardIcon = styled.div`
  grid-row: span 3;
  min-width: 40px;
`;

export const MCUWideInfoGridCardLargeText = styled.div`
  grid-row: span 2;
  font-weight: 600;
  font-size: 3rem;
  line-height: 3rem;
`;

export const MCUWideInfoGridCardMediumText = styled.div`
  grid-row: span 2;
  font-weight: 600;
  font-size: 1.75rem;
  line-height: 1.75rem;
`;

export const MCUWideInfoGridCardMediumRightText = styled(MCUWideInfoGridCardMediumText)`
  text-align: right;
`;

export const MCUWideInfoGridCardValueText = styled.div`
  color: ${({ theme }) => transparentize(0.25, theme.colours.white)};
`;

export const MCUWideInfoGridCardValueRightText = styled(MCUWideInfoGridCardValueText)`
  text-align: right;
`;

export const MCUWideInfoGridCardTimeText = styled.div`
  color: ${({ theme }) => transparentize(0.5, theme.colours.white)};
  font-size: 0.875rem;
`;

export const MCUWideInfoGridCardTimeRightText = styled(MCUWideInfoGridCardTimeText)`
  text-align: right;
`;
