/** @format */

import { ThemeType, ThemeVariants } from './types';
import { colours } from './colours';

export const LightTheme: ThemeType = {
  variant: ThemeVariants.LIGHT,

  //   Colours
  colours: {
    primary: colours.purple,
    success: colours.green,
    info: colours.blue,
    warning: colours.orange,
    danger: colours.red,
    background: colours.grey[8],
    foreground: colours.grey[0],
    text: [colours.grey[2], colours.grey[3], colours.grey[5]],
    cardBackground: colours.white,
    ...colours,
  },

  //   Text
  text: {
    colour: [colours.grey[2], colours.grey[3], colours.grey[5]],
  },
};
