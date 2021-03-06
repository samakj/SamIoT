/** @format */

import React, { useCallback } from 'react';
import { H2 } from 'client/components/Headers';
import { FullWidthCard } from '../Cards';
import { useDispatch, useSelector } from 'client/store';
import { Sun, Moon } from 'react-feather';
import { transparentize } from 'polished';
import { ThemeVariants } from 'client/style/types';
import { themeSlice } from 'client/store/theme/slice';
import styled from 'styled-components';
import { HeaderBarPropsType } from './types';

export const ThemeVariantContainer = styled.div`
  display: flex;
  align-items: center;
`;

export const Divider = styled.div`
  border: 1px solid ${({ theme }) => theme.colours.foreground};
  height: 2rem;
  margin: 0 1rem;
`;

export const SunIcon = styled(Sun)`
  cursor: pointer;
`;

export const MoonIcon = styled(Moon)`
  cursor: pointer;
`;

export const HeaderCard = styled(FullWidthCard)`
  display: flex;
  align-items: center;

  ${H2} {
    flex-basis: 100%;
  }
`;

export const HeaderBar = ({ title }: HeaderBarPropsType) => {
  const theme = useSelector((state) => state.theme);
  const dispatch = useDispatch();
  const setThemeGenerator = useCallback(
    (variant: ThemeVariants) => () => dispatch(themeSlice.actions.setTheme(variant)),
    []
  );
  return (
    <HeaderCard>
      <H2>{title}</H2>
      <ThemeVariantContainer>
        <SunIcon
          color={transparentize(
            theme.variant === ThemeVariants.LIGHT ? 0 : 0.5,
            theme.colours.text[0]
          )}
          onClick={setThemeGenerator(ThemeVariants.LIGHT)}
        />
        <Divider />
        <MoonIcon
          color={transparentize(
            theme.variant === ThemeVariants.DARK ? 0 : 0.5,
            theme.colours.text[0]
          )}
          onClick={setThemeGenerator(ThemeVariants.DARK)}
        />
      </ThemeVariantContainer>
    </HeaderCard>
  );
};
