/** @format */

import React, { CSSProperties } from 'react';
import 'styled-components';

export enum ThemeVariants {
  LIGHT = 'light',
  DARK = 'dark',
}

export interface ColoursType {
  white: string;
  black: string;
  blue: string;
  indigo: string;
  purple: string;
  pink: string;
  red: string;
  orange: string;
  yellow: string;
  green: string;
  teal: string;
  cyan: string;
  grey: string[];
}

export interface ThemeType {
  variant: ThemeVariants;

  colours: ColoursType & {
    primary: string;
    success: string;
    info: string;
    warning: string;
    danger: string;
    background: string;
    foreground: string;
    text: string[];
    cardBackground: string;
  };

  text: {
    colour: string[];
  };
}

export interface StyleProps {
  className?: string;
  style?: CSSProperties;
}

declare module 'styled-components' {
  export interface DefaultTheme extends ThemeType {}
}
