/** @format */

import { ThemeType, ThemeVariants } from './types';
import { colours } from './colours';

export const DarkTheme: ThemeType = {
  variant: ThemeVariants.DARK,

  //   Colours
  colours: {
    primary: colours.purple,
    success: colours.green,
    info: colours.blue,
    warning: colours.orange,
    danger: colours.red,
    background: '#161d31',
    foreground: colours.grey[0],
    text: [colours.grey[7], colours.grey[5], colours.grey[3]],
    cardBackground: '#283046',
    ...colours,
  },

  //   Text
  text: {
    colour: [colours.grey[7], colours.grey[5], colours.grey[3]],
  },
};
