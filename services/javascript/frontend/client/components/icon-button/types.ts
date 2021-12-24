/** @format */

import { StyleProps } from 'client/style/types';
import { Icon } from 'react-feather';

export interface IconButtonContainerPropsType {
  colour: string;
  size?: number;
  backgroundColour?: string;
  backgroundOpacity?: number;
}

export interface IconButtonPropsType extends StyleProps {
  Icon: Icon;
  colour: string;
  size?: number;
  backgroundColour?: string;
  backgroundOpacity?: number;
  onClick?: React.MouseEventHandler;
}
