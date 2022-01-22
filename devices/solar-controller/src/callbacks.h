#ifndef callbacks_h
#define callbacks_h

#include "config.h"
#include "defs.h"
#include "endpoints.h"
#include "tags.h"
#include <AsyncOTA.h>
#include <DeviceServer.h>

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
    Log.infof("Humidity changed to %.1f%%\n", humidity);
};

void onPVVoltageChange(float pvVoltage)
{
    DeviceServer.sendMeasurement(pvVoltage, "voltage", PV_VOLTAGE_TAGS, "%.1f");
    State.set(pvVoltage, MEASUREMENT, "voltage", PV_VOLTAGE_TAGS, "%.1f");
    Log.infof("PV Voltage changed to %.1fV\n", pvVoltage);
};

void onPVCurrentChange(float pvCurrent)
{
    DeviceServer.sendMeasurement(pvCurrent, "current", PV_CURRENT_TAGS, "%.1f");
    State.set(pvCurrent, MEASUREMENT, "current", PV_CURRENT_TAGS, "%.1f");
    Log.infof("PV Current changed to %.1fA\n", pvCurrent);
};

void onPVPowerChange(float pvPower)
{
    DeviceServer.sendMeasurement(pvPower, "power", PV_POWER_TAGS, "%.1f");
    State.set(pvPower, MEASUREMENT, "power", PV_POWER_TAGS, "%.1f");
    Log.infof("PV Power changed to %.1fW\n", pvPower);
};

void onBatteryChargingVoltageChange(float batteryChargingVoltage)
{
    DeviceServer.sendMeasurement(batteryChargingVoltage, "voltage", BATTERY_CHARGING_VOLTAGE_TAGS, "%.1f");
    State.set(batteryChargingVoltage, MEASUREMENT, "voltage", BATTERY_CHARGING_VOLTAGE_TAGS, "%.1f");
    Log.infof("Battery Charging Voltage changed to %.1fV\n", batteryChargingVoltage);
};

void onBatteryChargingCurrentChange(float batteryChargingCurrent)
{
    DeviceServer.sendMeasurement(batteryChargingCurrent, "current", BATTERY_CHARGING_CURRENT_TAGS, "%.1f");
    State.set(batteryChargingCurrent, MEASUREMENT, "current", BATTERY_CHARGING_CURRENT_TAGS, "%.1f");
    Log.infof("Battery Charging Current changed to %.1fA\n", batteryChargingCurrent);
};

void onBatteryChargingPowerChange(float batteryChargingPower)
{
    DeviceServer.sendMeasurement(batteryChargingPower, "power", BATTERY_CHARGING_POWER_TAGS, "%.1f");
    State.set(batteryChargingPower, MEASUREMENT, "power", BATTERY_CHARGING_POWER_TAGS, "%.1f");
    Log.infof("Battery Charging Power changed to %.1fW\n", batteryChargingPower);
};

void onLoadVoltageChange(float loadVoltage)
{
    DeviceServer.sendMeasurement(loadVoltage, "voltage", LOAD_VOLTAGE_TAGS, "%.1f");
    State.set(loadVoltage, MEASUREMENT, "voltage", LOAD_VOLTAGE_TAGS, "%.1f");
    Log.infof("Load Voltage changed to %.1fV\n", loadVoltage);
};

void onLoadCurrentChange(float loadCurrent)
{
    DeviceServer.sendMeasurement(loadCurrent, "current", LOAD_CURRENT_TAGS, "%.1f");
    State.set(loadCurrent, MEASUREMENT, "current", LOAD_CURRENT_TAGS, "%.1f");
    Log.infof("Load Current changed to %.1fA\n", loadCurrent);
};

void onLoadPowerChange(float loadPower)
{
    DeviceServer.sendMeasurement(loadPower, "power", LOAD_POWER_TAGS, "%.1f");
    State.set(loadPower, MEASUREMENT, "power", LOAD_POWER_TAGS, "%.1f");
    Log.infof("Load Power changed to %.1fW\n", loadPower);
};

void onBatteryTemperatureChange(float batteryTemperature)
{
    DeviceServer.sendMeasurement(batteryTemperature, "temperature", BATTERY_TEMPERATURE_TAGS, "%.1f");
    State.set(batteryTemperature, MEASUREMENT, "temperature", BATTERY_TEMPERATURE_TAGS, "%.1f");
    Log.infof("Battery Temperature changed to %.1fc\n", batteryTemperature);
};

void onCaseTemperatureChange(float caseTemperature)
{
    DeviceServer.sendMeasurement(caseTemperature, "temperature", CASE_TEMPERATURE_TAGS, "%.1f");
    State.set(caseTemperature, MEASUREMENT, "temperature", CASE_TEMPERATURE_TAGS, "%.1f");
    Log.infof("Case Temperature changed to %.1fc\n", caseTemperature);
};

void onHeatSinkTemperatureChange(float heatSinkTemperature)
{
    DeviceServer.sendMeasurement(heatSinkTemperature, "temperature", HEAT_SINK_TEMPERATURE_TAGS, "%.1f");
    State.set(heatSinkTemperature, MEASUREMENT, "temperature", HEAT_SINK_TEMPERATURE_TAGS, "%.1f");
    Log.infof("Heat Sink Temperature changed to %.1fc\n", heatSinkTemperature);
};

void onBatteryPercentageChange(float batteryPercentage)
{
    DeviceServer.sendMeasurement(batteryPercentage, "percentage", BATTERY_PERCENTAGE_TAGS, "%.1f");
    State.set(batteryPercentage, MEASUREMENT, "percentage", BATTERY_PERCENTAGE_TAGS, "%.1f");
    Log.infof("Battery Percentage changed to %.1f%%\n", batteryPercentage);
};

void onRemoteBatteryTemperatureChange(float remoteBatteryTemperature)
{
    DeviceServer.sendMeasurement(remoteBatteryTemperature, "temperature", REMOTE_BATTERY_TEMPERATURE_TAGS, "%.1f");
    State.set(remoteBatteryTemperature, MEASUREMENT, "temperature", REMOTE_BATTERY_TEMPERATURE_TAGS, "%.1f");
    Log.infof("Remote Battery Temperature changed to %.1fc\n", remoteBatteryTemperature);
};

void onSystemVoltageChange(float systemVoltage)
{
    DeviceServer.sendMeasurement(systemVoltage, "voltage", SYSTEM_VOLTAGE_TAGS, "%.1f");
    State.set(systemVoltage, MEASUREMENT, "voltage", SYSTEM_VOLTAGE_TAGS, "%.1f");
    Log.infof("System Voltage changed to %.1fV\n", systemVoltage);
};

void onRelay1Change(bool state)
{
    DeviceServer.sendMeasurement(getRelayOutputState(state, RELAY_1_NC, INVERTED_RELAYS), "on", RELAY_1_TAGS);
    State.set(getRelayOutputState(state, RELAY_1_NC, INVERTED_RELAYS), MEASUREMENT, "on", RELAY_1_TAGS);
    Log.infof("Relay1 changed to %s\n", getRelayOutputState(state, RELAY_1_NC, INVERTED_RELAYS) ? "HIGH" : "LOW");
};

void onRelay2Change(bool state)
{
    DeviceServer.sendMeasurement(getRelayOutputState(state, RELAY_2_NC, INVERTED_RELAYS), "on", RELAY_2_TAGS);
    State.set(getRelayOutputState(state, RELAY_2_NC, INVERTED_RELAYS), MEASUREMENT, "on", RELAY_2_TAGS);
    Log.infof("Relay2 changed to %s\n", getRelayOutputState(state, RELAY_2_NC, INVERTED_RELAYS) ? "HIGH" : "LOW");
};

void onRelay3Change(bool state)
{
    DeviceServer.sendMeasurement(getRelayOutputState(state, RELAY_3_NC, INVERTED_RELAYS), "on", RELAY_3_TAGS);
    State.set(getRelayOutputState(state, RELAY_3_NC, INVERTED_RELAYS), MEASUREMENT, "on", RELAY_3_TAGS);
    Log.infof("Relay3 changed to %s\n", getRelayOutputState(state, RELAY_3_NC, INVERTED_RELAYS) ? "HIGH" : "LOW");
};

void onRelay4Change(bool state)
{
    DeviceServer.sendMeasurement(getRelayOutputState(state, RELAY_4_NC, INVERTED_RELAYS), "on", RELAY_4_TAGS);
    State.set(getRelayOutputState(state, RELAY_4_NC, INVERTED_RELAYS), MEASUREMENT, "on", RELAY_4_TAGS);
    Log.infof("Relay4 changed to %s\n", getRelayOutputState(state, RELAY_4_NC, INVERTED_RELAYS) ? "HIGH" : "LOW");
};

void onRelay5Change(bool state)
{
    DeviceServer.sendMeasurement(getRelayOutputState(state, RELAY_5_NC, INVERTED_RELAYS), "on", RELAY_5_TAGS);
    State.set(getRelayOutputState(state, RELAY_5_NC, INVERTED_RELAYS), MEASUREMENT, "on", RELAY_5_TAGS);
    Log.infof("Relay5 changed to %s\n", getRelayOutputState(state, RELAY_5_NC, INVERTED_RELAYS) ? "HIGH" : "LOW");
};

void onRelay6Change(bool state)
{
    DeviceServer.sendMeasurement(getRelayOutputState(state, RELAY_6_NC, INVERTED_RELAYS), "on", RELAY_6_TAGS);
    State.set(getRelayOutputState(state, RELAY_6_NC, INVERTED_RELAYS), MEASUREMENT, "on", RELAY_6_TAGS);
    Log.infof("Relay6 changed to %s\n", getRelayOutputState(state, RELAY_6_NC, INVERTED_RELAYS) ? "HIGH" : "LOW");
};

void onRelay7Change(bool state)
{
    DeviceServer.sendMeasurement(getRelayOutputState(state, RELAY_7_NC, INVERTED_RELAYS), "on", RELAY_7_TAGS);
    State.set(getRelayOutputState(state, RELAY_7_NC, INVERTED_RELAYS), MEASUREMENT, "on", RELAY_7_TAGS);
    Log.infof("Relay7 changed to %s\n", getRelayOutputState(state, RELAY_7_NC, INVERTED_RELAYS) ? "HIGH" : "LOW");
};

void onRelay8Change(bool state)
{
    DeviceServer.sendMeasurement(getRelayOutputState(state, RELAY_8_NC, INVERTED_RELAYS), "on", RELAY_8_TAGS);
    State.set(getRelayOutputState(state, RELAY_8_NC, INVERTED_RELAYS), MEASUREMENT, "on", RELAY_8_TAGS);
    Log.infof("Relay8 changed to %s\n", getRelayOutputState(state, RELAY_8_NC, INVERTED_RELAYS) ? "HIGH" : "LOW");
};

void onInverterRelayChange(bool state)
{
    DeviceServer.sendMeasurement(getRelayOutputState(state, INVERTER_RELAY_NC, false), "on", INVERTER_RELAY_TAGS);
    State.set(getRelayOutputState(state, INVERTER_RELAY_NC, false), MEASUREMENT, "on", INVERTER_RELAY_TAGS);
    Log.infof("Invterer relay changed to %s\n", getRelayOutputState(state, INVERTER_RELAY_NC, false) ? "HIGH" : "LOW");
};

#endif