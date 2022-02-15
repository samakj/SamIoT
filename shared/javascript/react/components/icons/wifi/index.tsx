import React from "react";
import styled from "styled-components";
import { WifiPropsType } from "./types";

const EmptyPath = styled.path`
  fill: ${({ theme }) => theme.colours.white};
  fill-opacity: 0.25;
`;
const FullPath = styled.path`
  fill: ${({ theme }) => theme.colours.success};
`;

export const Wifi: React.FunctionComponent<WifiPropsType> = ({
  percentage = 0,
  height = 32,
  width = 40,
}) => {
  const inversePercentage = percentage / 100;
  const fullWidth = inversePercentage * width;
  const fullHeight = inversePercentage * height;
  const widthOffset = (width - fullWidth) / 2;
  const heightOffset = height - fullHeight;
  const WIFI_OUTLINE =
    `M0 ${height / 4}` +
    `L${width / 2} ${height} ` +
    `L${width} ${height / 4} ` +
    `Q${width / 2} ${-height / 4} 0 ${height / 4} ` +
    `Z`;
  return (
    <svg
      xmlns="http://www.w3.org/2000/svg"
      width={width}
      height={height}
      viewBox={`0 0 ${width} ${height}`}
      fill="none"
    >
      <EmptyPath
        d={
          `M0 ${height / 4}` +
          `L${width / 2} ${height} ` +
          `L${width} ${height / 4} ` +
          `Q${width / 2} ${-height / 4} 0 ${height / 4} ` +
          `Z`
        }
      />
      <FullPath
        d={
          `M${widthOffset} ${heightOffset + fullHeight / 4}` +
          `L${widthOffset + fullWidth / 2} ${heightOffset + fullHeight} ` +
          `L${widthOffset + fullWidth} ${heightOffset + fullHeight / 4} ` +
          `Q${widthOffset + fullWidth / 2} ${heightOffset - fullHeight / 4} ` +
          `${widthOffset} ${heightOffset + fullHeight / 4} ` +
          `Z`
        }
      />
    </svg>
  );
};
