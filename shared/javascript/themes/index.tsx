/** @format */

import React from "react";
import { createGlobalStyle, css, ThemeProvider } from "styled-components";
import { theme, ThemeType } from "./theme";

declare module "styled-components" {
  export interface DefaultTheme extends ThemeType {}
}

export const sansSerifFont = css`
  font-family: "Roboto", Helvetica, Arial, sans-serif;
`;
export const monospaceFont = css`
  font-family: "Inconsolata", monospace;
`;

export const GlobalStyle = createGlobalStyle`
  body {
    ${sansSerifFont}
    box-sizing: border-box;
    background-color: ${({ theme }) => theme.colours.black};
    color: ${({ theme }) => theme.colours.white};
    font-size: 1rem;
    font-weight: 400;
    line-height: 1.45;
    margin: 0;
    padding: 0 calc((100vw - 1440px) / 2);
    min-height: 100vh;
  }
`;

export const StaticThemeWrapper = (props: React.PropsWithChildren<{}>) => {
  return (
    <ThemeProvider theme={theme}>
      <GlobalStyle />
      {props?.children}
    </ThemeProvider>
  );
};
