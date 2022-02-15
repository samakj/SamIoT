import React from "react";
import styled from "styled-components";
import { BatteryPropsType } from "./types";

const EmptyPath = styled.path`
  fill: ${({ theme }) => theme.colours.white};
  fill-opacity: 0.25;
`;
const FullPath = styled.path`
  fill: ${({ theme }) => theme.colours.success};
`;

export const Battery: React.FunctionComponent<BatteryPropsType> = ({
  percentage = 0,
  height = 64,
  width = 40,
}) => {
  const BATTERY_OUTLINE =
    `M${height / 8} 0 ` +
    `H${width - height / 8} ` +
    `V${height / 8} ` +
    `H${width}` +
    `V${height}` +
    `H0` +
    `V${height / 8}` +
    `H${height / 8}` +
    `Z`;
  return (
    <svg
      xmlns="http://www.w3.org/2000/svg"
      width={width}
      height={height}
      viewBox={`0 0 ${width} ${height}`}
      fill="none"
    >
      <mask id="outline">
        <path d={BATTERY_OUTLINE} fill="white" />
      </mask>
      <EmptyPath d={BATTERY_OUTLINE} />
      <FullPath
        d={
          `M0 ${(1 - percentage / 100) * height} ` +
          `H${width} V${height} H0 ` +
          `V${(1 - percentage / 100) * height} Z`
        }
        mask="url(#outline)"
      />
    </svg>
  );
};
