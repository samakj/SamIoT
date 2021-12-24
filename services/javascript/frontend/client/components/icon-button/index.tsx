/** @format */

import { transparentize } from 'polished';
import React from 'react';
import styled, { css } from 'styled-components';
import { IconButtonContainerPropsType, IconButtonPropsType } from './types';

export const IconButtonContainer = styled.div<IconButtonContainerPropsType>`
  height: ${({ size }) => size.toFixed(3)}rem;
  width: ${({ size }) => size.toFixed(3)}rem;
  display: flex;
  justify-content: center;
  align-items: center;
  background-color: ${({ colour, backgroundColour, backgroundOpacity }) =>
    backgroundColour || transparentize(1 - backgroundOpacity, colour)};
  box-sizing: border-box;
  border-radius: 50%;
  cursor: ${({ onClick }) => (!!onClick ? 'pointer' : 'normal')};

  ${({ onClick, colour, backgroundColour, backgroundOpacity }) =>
    !!onClick
      ? css`
          transition: background-color 300ms;

          &:hover {
            background-color: ${backgroundColour ||
            transparentize(1 - (backgroundOpacity + 0.1), colour)};
          }
        `
      : undefined};
`;

export const IconButton = ({
  Icon,
  colour,
  size = 1,
  backgroundColour,
  backgroundOpacity = 0.15,
  ...rest
}: IconButtonPropsType) => (
  <IconButtonContainer
    colour={colour}
    size={size}
    backgroundColour={backgroundColour}
    backgroundOpacity={backgroundOpacity}
    {...rest}
  >
    <Icon color={colour} size={`${(size / 2).toFixed(3)}rem`} />
  </IconButtonContainer>
);

export const HeaderIconButton = styled(IconButton)`
  margin-bottom: 1rem;
`;
