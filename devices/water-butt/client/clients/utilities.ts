/** @format */

import { UtilitiesClient as UtilitiesClientClass } from 'shared/javascript/clients/utilities';

export const UtilitiesClient = new UtilitiesClientClass(
  process.env.UTILITIES_API_HOST,
  process.env.UTILITIES_API_PORT,
  false
);
