/** @format */

import {
  DAY_IN_MS,
  HOUR_IN_MS,
  MINUTE_IN_MS,
  MONTH_IN_MS,
  SECOND_IN_MS,
  YEAR_IN_MS,
} from '../static/times';

export const prettyTimeDelta = (
  dateA: Date | string | number,
  dateB?: Date | string | number
): string => {
  if (!dateA) return '...';

  dateA = new Date(dateA);
  dateB = dateB ? new Date(dateB) : new Date();

  const totalMs = +dateB - +dateA;
  const milliseconds = Math.floor(totalMs % SECOND_IN_MS);
  const seconds = Math.floor((totalMs % MINUTE_IN_MS) / SECOND_IN_MS);
  const minutes = Math.floor((totalMs % HOUR_IN_MS) / MINUTE_IN_MS);
  const hours = Math.floor((totalMs % DAY_IN_MS) / HOUR_IN_MS);
  const days = Math.floor((totalMs % MONTH_IN_MS) / DAY_IN_MS);
  const months = Math.floor((totalMs % YEAR_IN_MS) / MONTH_IN_MS);
  const years = Math.floor(totalMs / YEAR_IN_MS);

  if (years) return `${years}y ${months ? `${months}m` : ''}`;
  if (months) return `${months}m ${days ? `${days}d` : ''}`;
  if (days) return `${days}d ${hours ? `${hours}h` : ''}`;
  if (hours) return `${hours}h ${minutes ? `${minutes}m` : ''}`;
  if (minutes) return `${minutes}h ${seconds ? `${seconds}s` : ''}`;
  if (seconds >= 10) return `${(totalMs / 1000).toFixed(1)}s`;
  if (seconds) return `${(totalMs / 1000).toFixed(2)}s`;

  return `${milliseconds}ms`;
};
