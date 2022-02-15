/** @format */

import { IoTClient as IoTClientClass } from 'shared/javascript/clients/iot';

export const IoTClient = new IoTClientClass(
  process.env.IOT_API_HOST,
  process.env.IOT_API_PORT,
  false
);
