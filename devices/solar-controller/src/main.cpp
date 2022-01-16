#include <Arduino.h>

#include "config.h"
#include <AsyncDHT.h>
#include <AsyncNTP.h>
#include <AsyncOTA.h>
#include <AsyncPZEM.h>
#include <AsyncSwitch.h>
#include <AsyncTracer.h>
#include <AsyncWifi.h>
#include <DeviceServer.h>
#include <ExecutionTimer.h>
#include <Log.h>
#include <TimeUtils.h>


void setup()
{  
    Serial.begin(115200);
};

void loop()
{
    Log.infof("%u\n", millis());
    delay(100);
};