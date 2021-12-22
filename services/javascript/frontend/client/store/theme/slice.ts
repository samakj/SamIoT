/** @format */

import { createSlice, PayloadAction } from '@reduxjs/toolkit';
import { LightTheme, DarkTheme } from 'client/style';
import { ThemeVariants } from '../../style/types';
import { ThemeStateType } from './types';

const initialState: ThemeStateType = {
  ...LightTheme,
};

export const themeSlice = createSlice({
  name: 'theme',
  initialState,
  reducers: {
    setTheme: (state: ThemeStateType, action: PayloadAction<ThemeVariants>) => {
      let theme = { ...LightTheme };
      if (action.payload === ThemeVariants.LIGHT) theme = { ...LightTheme };
      if (action.payload === ThemeVariants.DARK) theme = { ...DarkTheme };
      if (theme.variant !== state.variant)
        Object.keys(theme).map((key) => (state[key] = theme[key]));
    },
  },
});
