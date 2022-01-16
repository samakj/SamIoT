#include <Arduino.h>

#include "config.h"
#include "tags.h"
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

DeviceState State;
AsyncDHT DHTSensor(15, DHT22);

void onWifiConnect(std::string ssid)
{
    State.set(ssid, MEASUREMENT, "ssid", WIFI_TAGS);
    std::string ip = AsyncWifi.getLocalIPAddressString();
    AsyncOTA.setup(HOSTNAME, OTA_PASSWORD);
};

void onWifiSsidChange(std::string ssid)
{
    DeviceServer.sendMeasurement(ssid, "ssid", WIFI_TAGS);
    State.set(ssid, MEASUREMENT, "ssid", WIFI_TAGS);
};

void onWifiStrengthChange(float strength)
{
    DeviceServer.sendMeasurement(strength * 100, "strength", WIFI_TAGS, "%.1f");
    State.set(strength * 100, MEASUREMENT, "strength", WIFI_TAGS, "%.1f");
    Log.infof("Wifi chaned to %s %.0f%%\n", AsyncWifi.ssid.c_str(), strength * 100);
};

void onNTPConnect(){};

void onTemperatureChange(float temperature)
{
    DeviceServer.sendMeasurement(temperature, "temperature", TEMPERATURE_TAGS, "%.1f");
    State.set(temperature, MEASUREMENT, "temperature", TEMPERATURE_TAGS, "%.1f");
    Log.infof("Temperature changed to %.1fc\n", temperature);
};

void onHumidityChange(float humidity)
{
    DeviceServer.sendMeasurement(humidity, "humidity", HUMIDITY_TAGS, "%.1f");
    State.set(humidity, MEASUREMENT, "humidity", HUMIDITY_TAGS, "%.1f");
    Log.infof("Humidity changed to %.1f%\n", humidity);
};

void setup()
{  
    Serial.begin(115200);

    Log.info("---------------- SETTING UP ----------------");
    AsyncWifi.addConnectCallback(onWifiConnect);
    AsyncWifi.addSsidCallback(onWifiSsidChange);
    AsyncWifi.addStrengthCallback(onWifiStrengthChange);
    AsyncNTP.addConnectCallback(onNTPConnect);
    AsyncNTP.addWifiConnectCallback();

    DeviceServer.setLocation(LOCATION);
    DeviceServer.setState(&State);
    DeviceServer.addWifiConnectCallback();

    AsyncWifi.connect({&Patty, &Selma, &TheVale}, HOSTNAME, IP_LOCATION);
    AsyncWifi.getConnectedNetworkStrength();

    DHTSensor.setTemperatureCallback(onTemperatureChange);
    DHTSensor.setHumidityCallback(onHumidityChange);
    DHTSensor.setup();
    Log.info("-------------- SETUP COMPLETE --------------");
};

void loop()
{
    ExecutionTimer.start("Loop");

    ExecutionTimer.start("Wifi");
    AsyncWifi.connect({&Patty, &Selma}, "Breadboard");
    AsyncWifi.getConnectedNetworkStrength();
    ExecutionTimer.end("Wifi");

    ExecutionTimer.start("NTP");
    AsyncNTP.loop();
    ExecutionTimer.end("NTP");

    ExecutionTimer.start("Server");
    DeviceServer.loop();
    ExecutionTimer.end("Server");

    ExecutionTimer.start("DHT");
    DHTSensor.loop();
    ExecutionTimer.end("DHT");
    
    ExecutionTimer.start("OTA");
    AsyncOTA.loop();
    ExecutionTimer.end("OTA");

    ExecutionTimer.end("Loop");
    ExecutionTimer.loop();
};