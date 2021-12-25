/** @format */

import { createSlice, PayloadAction } from '@reduxjs/toolkit';
import { LightTheme, DarkTheme } from 'client/style';
import { ThemeVariants } from '../../style/types';
import { ThemeStateType } from './types';
import Cookies from 'universal-cookie';

const cookies = new Cookies();
const initialState: ThemeStateType = {
  ...(cookies.get('theme') === ThemeVariants.LIGHT ? LightTheme : DarkTheme),
};

export const themeSlice = createSlice({
  name: 'theme',
  initialState,
  reducers: {
    setTheme: (state: ThemeStateType, action: PayloadAction<ThemeVariants>) => {
      let theme = { ...LightTheme };
      if (action.payload === ThemeVariants.LIGHT) {
        theme = { ...LightTheme };
        cookies.set('theme', ThemeVariants.LIGHT);
      }
      if (action.payload === ThemeVariants.DARK) {
        theme = { ...DarkTheme };
        cookies.set('theme', ThemeVariants.DARK);
      }
      if (theme.variant !== state.variant)
        Object.keys(theme).map((key) => (state[key] = theme[key]));
    },
  },
});
