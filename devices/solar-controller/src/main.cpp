#include <Arduino.h>

#include "callbacks.h"
#include "config.h"
#include "defs.h"
#include "endpoints.h"
#include "tags.h"
#include <AsyncDHT.h>
#include <AsyncNTP.h>
#include <AsyncOTA.h>
#include <AsyncPZEM.h>
#include <AsyncOutput.h>
#include <AsyncTracer.h>
#include <AsyncWifi.h>
#include <DeviceServer.h>
#include <ExecutionTimer.h>
#include <Log.h>
#include <TimeUtils.h>


void setup()
{  
    Serial.begin(115200);
    // ModbusSerial.begin(115200, SERIAL_8N1, MODBUS_RX, MODBUS_TX);

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

    // Tracer.setPVVoltageCallback(onPVVoltageChange);
    // Tracer.setPVCurrentCallback(onPVCurrentChange);
    // Tracer.setPVPowerCallback(onPVPowerChange);
    // Tracer.setBatteryChargingVoltageCallback(onBatteryChargingVoltageChange);
    // Tracer.setBatteryChargingCurrentCallback(onBatteryChargingCurrentChange);
    // Tracer.setBatteryChargingPowerCallback(onBatteryChargingPowerChange);
    // Tracer.setLoadVoltageCallback(onLoadVoltageChange);
    // Tracer.setLoadCurrentCallback(onLoadCurrentChange);
    // Tracer.setLoadPowerCallback(onLoadPowerChange);
    // Tracer.setBatteryTemperatureCallback(onBatteryTemperatureChange);
    // Tracer.setCaseTemperatureCallback(onCaseTemperatureChange);
    // Tracer.setHeatSinkTemperatureCallback(onHeatSinkTemperatureChange);
    // Tracer.setBatteryPercentageCallback(onBatteryPercentageChange);
    // Tracer.setRemoteBatteryTemperatureCallback(onRemoteBatteryTemperatureChange);
    // Tracer.setSystemVoltageCallback(onSystemVoltageChange);
    // Tracer.setup();

    PZEM1.setVoltageCallback(onPZEM1VoltageChange);
    PZEM1.setCurrentCallback(onPZEM1CurrentChange);
    PZEM1.setPowerCallback(onPZEM1PowerChange);
    PZEM1.setEnergyCallback(onPZEM1EnergyChange);
    PZEM1.setFrequencyCallback(onPZEM1FrequencyChange);
    PZEM1.setPowerFactorCallback(onPZEM1PowerFactorChange);
    PZEM1.setup();

    PZEM2.setVoltageCallback(onPZEM2VoltageChange);
    PZEM2.setCurrentCallback(onPZEM2CurrentChange);
    PZEM2.setPowerCallback(onPZEM2PowerChange);
    PZEM2.setEnergyCallback(onPZEM2EnergyChange);
    PZEM2.setFrequencyCallback(onPZEM2FrequencyChange);
    PZEM2.setPowerFactorCallback(onPZEM2PowerFactorChange);
    PZEM2.setup();

    PZEM3.setVoltageCallback(onPZEM3VoltageChange);
    PZEM3.setCurrentCallback(onPZEM3CurrentChange);
    PZEM3.setPowerCallback(onPZEM3PowerChange);
    PZEM3.setEnergyCallback(onPZEM3EnergyChange);
    PZEM3.setFrequencyCallback(onPZEM3FrequencyChange);
    PZEM3.setPowerFactorCallback(onPZEM3PowerFactorChange);
    PZEM3.setup();

    PZEM4.setVoltageCallback(onPZEM4VoltageChange);
    PZEM4.setCurrentCallback(onPZEM4CurrentChange);
    PZEM4.setPowerCallback(onPZEM4PowerChange);
    PZEM4.setEnergyCallback(onPZEM4EnergyChange);
    PZEM4.setFrequencyCallback(onPZEM4FrequencyChange);
    PZEM4.setPowerFactorCallback(onPZEM4PowerFactorChange);
    PZEM4.setup();

    Relay1.setOutputCallback(onRelay1Change);
    Relay2.setOutputCallback(onRelay2Change);
    Relay3.setOutputCallback(onRelay3Change);
    Relay4.setOutputCallback(onRelay4Change);
    Relay5.setOutputCallback(onRelay5Change);
    Relay6.setOutputCallback(onRelay6Change);
    Relay7.setOutputCallback(onRelay7Change);
    Relay8.setOutputCallback(onRelay8Change);
    Relay1.setup();
    Relay2.setup();
    Relay3.setup();
    Relay4.setup();
    Relay6.setup();
    Relay7.setup();
    Relay8.setup();

    InverterRelay.setOutputCallback(onInverterRelayChange);
    InverterRelay.setup();

    State.set(getRelayOutputState(Relay1.state, RELAY_1_NC, INVERTED_RELAYS), MEASUREMENT, "on", RELAY_1_TAGS);
    State.set(getRelayOutputState(Relay2.state, RELAY_2_NC, INVERTED_RELAYS), MEASUREMENT, "on", RELAY_2_TAGS);
    State.set(getRelayOutputState(Relay3.state, RELAY_3_NC, INVERTED_RELAYS), MEASUREMENT, "on", RELAY_3_TAGS);
    State.set(getRelayOutputState(Relay4.state, RELAY_4_NC, INVERTED_RELAYS), MEASUREMENT, "on", RELAY_4_TAGS);
    State.set(getRelayOutputState(Relay5.state, RELAY_5_NC, INVERTED_RELAYS), MEASUREMENT, "on", RELAY_5_TAGS);
    State.set(getRelayOutputState(Relay6.state, RELAY_6_NC, INVERTED_RELAYS), MEASUREMENT, "on", RELAY_6_TAGS);
    State.set(getRelayOutputState(Relay7.state, RELAY_7_NC, INVERTED_RELAYS), MEASUREMENT, "on", RELAY_7_TAGS);
    State.set(getRelayOutputState(Relay8.state, RELAY_8_NC, INVERTED_RELAYS), MEASUREMENT, "on", RELAY_8_TAGS);
    State.set(getRelayOutputState(InverterRelay.state, INVERTER_RELAY_NC, false), MEASUREMENT, "on", INVERTER_RELAY_TAGS);


    DeviceServer.addHttpEndpoint( "/relay/1", HTTP_POST, DeviceServer.BAD_REQUEST, DeviceServer.BAD_UPLOAD, setRelay1);
    DeviceServer.addHttpEndpoint( "/relay/2", HTTP_POST, DeviceServer.BAD_REQUEST, DeviceServer.BAD_UPLOAD, setRelay2);
    DeviceServer.addHttpEndpoint( "/relay/3", HTTP_POST, DeviceServer.BAD_REQUEST, DeviceServer.BAD_UPLOAD, setRelay3);
    DeviceServer.addHttpEndpoint( "/relay/4", HTTP_POST, DeviceServer.BAD_REQUEST, DeviceServer.BAD_UPLOAD, setRelay4);
    DeviceServer.addHttpEndpoint( "/relay/5", HTTP_POST, DeviceServer.BAD_REQUEST, DeviceServer.BAD_UPLOAD, setRelay5);
    DeviceServer.addHttpEndpoint( "/relay/6", HTTP_POST, DeviceServer.BAD_REQUEST, DeviceServer.BAD_UPLOAD, setRelay6);
    DeviceServer.addHttpEndpoint( "/relay/7", HTTP_POST, DeviceServer.BAD_REQUEST, DeviceServer.BAD_UPLOAD, setRelay7);
    DeviceServer.addHttpEndpoint( "/relay/8", HTTP_POST, DeviceServer.BAD_REQUEST, DeviceServer.BAD_UPLOAD, setRelay8);
    DeviceServer.addHttpEndpoint( "/inverter", HTTP_POST, DeviceServer.BAD_REQUEST, DeviceServer.BAD_UPLOAD, setInverterRelay);
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

    ExecutionTimer.start("Tracer");
    // Tracer.loop();
    ExecutionTimer.end("Tracer");

    ExecutionTimer.start("PZEM1");
    PZEM1.loop();
    ExecutionTimer.end("PZEM1");

    ExecutionTimer.start("PZEM2");
    PZEM2.loop();
    ExecutionTimer.end("PZEM2");

    ExecutionTimer.start("PZEM3");
    PZEM3.loop();
    ExecutionTimer.end("PZEM3");

    ExecutionTimer.start("PZEM4");
    PZEM4.loop();
    ExecutionTimer.end("PZEM4");

    
    ExecutionTimer.start("OTA");
    AsyncOTA.loop();
    ExecutionTimer.end("OTA");

    Relay1.loop();
    Relay2.loop();
    Relay3.loop();
    Relay4.loop();
    Relay5.loop();
    Relay6.loop();
    Relay7.loop();
    Relay8.loop();
    InverterRelay.loop();

    ExecutionTimer.end("Loop");
    ExecutionTimer.loop();

};