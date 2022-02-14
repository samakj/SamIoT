/** @format */

import { createGlobalStyle, css } from 'styled-components';
import { theme, ThemeType } from 'shared/javascript/themes/devices';

declare module 'styled-components' {
  export interface DefaultTheme extends ThemeType {}
}

export const sansSerifFont = css`
  font-family: 'Roboto', Helvetica, Arial, sans-serif;
`;
export const monospaceFont = css`
  font-family: 'Inconsolata', monospace;
`;

export const GlobalStyle = createGlobalStyle`
  body {
    ${sansSerifFont}
    background-color: ${({ theme }) => theme.colours.black};
    color: ${({ theme }) => theme.colours.white};
    font-size: 1rem;
    font-weight: 400;
    line-height: 1.45;
    margin: 0;
  }
`;

export { theme };
