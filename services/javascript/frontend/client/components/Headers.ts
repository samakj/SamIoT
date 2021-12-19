/** @format */

import styled, { css } from 'styled-components';

export const Headers = css`
  color: ${({ theme }) => theme.text.colour[0]};
  margin-top: 0;
  margin-bottom: 0.5rem;
  font-weight: 600;
  line-height: 1.2;
`;

export const H1 = styled.h1`
  ${Headers}
  font-size: 1.625rem;
`;

export const H2 = styled.h2`
  ${Headers}
  font-size: 1.5rem;
`;

export const H3 = styled.h3`
  ${Headers}
  font-size: 1.375rem;
`;

export const H4 = styled.h4`
  ${Headers}
  font-size: 1.25rem;
`;

export const H5 = styled.h5`
  ${Headers}
  font-size: 1.125rem;
`;

export const H6 = styled.h6`
  ${Headers}
  font-size: 1rem;
`;
