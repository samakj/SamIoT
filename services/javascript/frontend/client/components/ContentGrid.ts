/** @format */

import styled from 'styled-components';

export const ContentGrid = styled.div`
  display: grid;
  grid-template-columns: repeat(4, 1fr);
  grid-gap: 1rem;
  grid-auto-flow: row dense;
  margin: 1.5rem;
`;
