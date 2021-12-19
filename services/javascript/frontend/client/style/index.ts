/** @format */

import { createGlobalStyle, css } from 'styled-components';

export const sansSerifFont = css`
  font-family: 'Montserrat', Helvetica, Arial, sans-serif;
`;
export const monospaceFont = css`
  font-family: 'Inconsolata', monospace;
`;

export const GlobalStyle = createGlobalStyle`
  body {
    ${sansSerifFont}
    background-color: ${({ theme }) => theme.colours.background};
    color: ${({ theme }) => theme.colours.text[1]};
    font-size: 1rem;
    font-weight: 400;
    line-height: 1.45;
    margin: 0;
  }
`;

export { LightTheme } from './light-theme';
export { DarkTheme } from './dark-theme';
