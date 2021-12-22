/** @format */

import styled from 'styled-components';
import { transparentize } from 'polished';

export const Card = styled.div`
  box-shadow: 0 0.25rem 1.5rem 0 ${({ theme }) => transparentize(0.9, theme.colours.grey[1])};
  background-color: ${({ theme }) => theme.colours.cardBackground};
  border: 1px solid ${({ theme }) => transparentize(0.875, theme.colours.grey[1])};
  padding: 1.5rem;
  border-radius: 0.5rem;
  box-sizing: border-box;
  grid-column: span 2;
  grid-row: span 2;
`;

export const SmallCard = styled(Card)`
  grid-column: span 1;
  grid-row: span 1;
`;

export const SmallWideCard = styled(Card)`
  grid-column: span 2;
  grid-row: span 1;
`;

export const FullWidthCard = styled(Card)`
  grid-column: 1 / -1;
  grid-row: span 1;
`;
