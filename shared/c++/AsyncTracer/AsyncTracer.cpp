#include <AsyncTracer.h>

AsyncTracer::AsyncTracer(
    HardwareSerial *_serial,
    uint8_t _tx,
    uint8_t _rx,
    uint8_t _address) : serial(_serial),
                        tx(_tx),
                        rx(_rx),
                        address(_address)
{
}

void AsyncTracer::setPVVoltageCallback(
    PVVoltageCallback _pvVoltageCallback)
{
    pvVoltageCallback = _pvVoltageCallback;
};
void AsyncTracer::setPVCurrentCallback(
    PVCurrentCallback _pvCurrentCallback)
{
    pvCurrentCallback = _pvCurrentCallback;
};
void AsyncTracer::setPVPowerCallback(
    PVPowerCallback _pvPowerCallback)
{
    pvPowerCallback = _pvPowerCallback;
};
void AsyncTracer::setBatteryChargingVoltageCallback(
    BatteryChargingVoltageCallback _batteryChargingVoltageCallback)
{
    batteryChargingVoltageCallback = _batteryChargingVoltageCallback;
};
void AsyncTracer::setBatteryChargingCurrentCallback(
    BatteryChargingCurrentCallback _batteryChargingCurrentCallback)
{
    batteryChargingCurrentCallback = _batteryChargingCurrentCallback;
};
void AsyncTracer::setBatteryChargingPowerCallback(
    BatteryChargingPowerCallback _batteryChargingPowerCallback)
{
    batteryChargingPowerCallback = _batteryChargingPowerCallback;
};
void AsyncTracer::setLoadVoltageCallback(
    LoadVoltageCallback _loadVoltageCallback)
{
    loadVoltageCallback = _loadVoltageCallback;
};
void AsyncTracer::setLoadCurrentCallback(
    LoadCurrentCallback _loadCurrentCallback)
{
    loadCurrentCallback = _loadCurrentCallback;
};
void AsyncTracer::setLoadPowerCallback(
    LoadPowerCallback _loadPowerCallback)
{
    loadPowerCallback = _loadPowerCallback;
};
void AsyncTracer::setBatteryTemperatureCallback(
    BatteryTemperatureCallback _batteryTemperatureCallback)
{
    batteryTemperatureCallback = _batteryTemperatureCallback;
};
void AsyncTracer::setCaseTemperatureCallback(
    CaseTemperatureCallback _caseTemperatureCallback)
{
    caseTemperatureCallback = _caseTemperatureCallback;
};
void AsyncTracer::setHeatSinkTemperatureCallback(
    HeatSinkTemperatureCallback _heatSinkTemperatureCallback)
{
    heatSinkTemperatureCallback = _heatSinkTemperatureCallback;
};
void AsyncTracer::setBatteryPercentageCallback(
    BatteryPercentageCallback _batteryPercentageCallback)
{
    batteryPercentageCallback = _batteryPercentageCallback;
};
void AsyncTracer::setRemoteBatteryTemperatureCallback(
    RemoteBatteryTemperatureCallback _remoteBatteryTemperatureCallback)
{
    remoteBatteryTemperatureCallback = _remoteBatteryTemperatureCallback;
};
void AsyncTracer::setSystemVoltageCallback(
    SystemVoltageCallback _systemVoltageCallback)
{
    systemVoltageCallback = _systemVoltageCallback;
};

void AsyncTracer::setup(void (*preTransmission)(), void (*postTransmission)())
{
    client = new ModbusMaster();
    client->begin(address, *serial);
    Sam::Log.infof(
        "Tracer initialised at address 0x%02X\n",
        address);
};

void AsyncTracer::loop()
{
    if (!client)
        setup();
    if (TimeUtils.millisSince(lastReadMillis) > (long unsigned int)readPeriod && !jobQueue.size())
    {
        if (pvVoltageCallback != nullptr)
            jobQueue.push_back("checkPVVoltage");
        if (pvCurrentCallback != nullptr)
            jobQueue.push_back("checkPVCurrent");
        if (pvPowerCallback != nullptr)
            jobQueue.push_back("checkPVPower");
        if (batteryChargingVoltageCallback != nullptr)
            jobQueue.push_back("checkBatteryChargingVoltage");
        if (batteryChargingCurrentCallback != nullptr)
            jobQueue.push_back("checkBatteryChargingCurrent");
        if (batteryChargingPowerCallback != nullptr)
            jobQueue.push_back("checkBatteryChargingPower");
        if (loadVoltageCallback != nullptr)
            jobQueue.push_back("checkLoadVoltage");
        if (loadCurrentCallback != nullptr)
            jobQueue.push_back("checkLoadCurrent");
        if (loadPowerCallback != nullptr)
            jobQueue.push_back("checkLoadPower");
        if (batteryTemperatureCallback != nullptr)
            jobQueue.push_back("checkBatteryTemperature");
        if (caseTemperatureCallback != nullptr)
            jobQueue.push_back("checkCaseTemperature");
        if (heatSinkTemperatureCallback != nullptr)
            jobQueue.push_back("checkHeatSinkTemperature");
        if (batteryPercentageCallback != nullptr)
            jobQueue.push_back("checkBatteryPercentage");
        if (remoteBatteryTemperatureCallback != nullptr)
            jobQueue.push_back("checkRemoteBatteryTemperature");
        if (systemVoltageCallback != nullptr)
            jobQueue.push_back("checkSystemVoltage");
        lastReadMillis = millis();
        readCount++;
    }
    checkJobQueue();
};

boolean AsyncTracer::getCoil(uint16_t address, boolean defaultValue)
{
    uint8_t response = client->readCoils(address, 1);
    if (response == client->ku8MBSuccess)
        return client->getResponseBuffer(0x00);

    Sam::Log.warnf("Failed to get coil at 0x%02X: 0x%02X\n", address, response);
    return defaultValue;
};

float AsyncTracer::get8BitRegister(uint16_t address, float multiplier, float defaultValue)
{
    uint8_t response = client->readInputRegisters(address, 1);
    if (response == client->ku8MBSuccess)
        return client->getResponseBuffer(0x00) / multiplier;

    Sam::Log.warnf("Failed to get 8 bit register at 0x%02X: 0x%02X\n", address, response);
    return defaultValue;
};

float AsyncTracer::get16BitRegister(uint16_t address, float multiplier, float defaultValue)
{
    uint8_t response = client->readInputRegisters(address, 2);
    if (response == client->ku8MBSuccess)
        return (
                   client->getResponseBuffer(0x00) |
                   client->getResponseBuffer(0x01) << 16) /
               multiplier;

    Sam::Log.warnf("Failed to get 16 bit register at 0x%02X: 0x%02X\n", address, response);
    return defaultValue;
};

void AsyncTracer::checkPVVoltage()
{
    float newPVVoltage = get8BitRegister(PV_VOLTAGE, 100.0f);

    if (!isnan(newPVVoltage) && abs(newPVVoltage - pvVoltage) > 0.05)
    {
        pvVoltage = newPVVoltage;
        if (pvVoltageCallback != nullptr)
            pvVoltageCallback(pvVoltage);
    }
};

void AsyncTracer::checkPVCurrent()
{
    float newPVCurrent = get8BitRegister(PV_CURRENT, 100.0f);

    if (!isnan(newPVCurrent) && abs(newPVCurrent - pvCurrent) > 0.05)
    {
        pvCurrent = newPVCurrent;
        if (pvCurrentCallback != nullptr)
            pvCurrentCallback(pvCurrent);
    }
};

void AsyncTracer::checkPVPower()
{
    float newPVPower = get16BitRegister(PV_POWER, 100.0f);

    if (!isnan(newPVPower) && abs(newPVPower - pvPower) > 0.05)
    {
        pvPower = newPVPower;
        if (pvPowerCallback != nullptr)
            pvPowerCallback(pvPower);
    }
};

void AsyncTracer::checkBatteryChargingVoltage()
{
    float newBatteryChargingVoltage = get8BitRegister(BATTERY_CHARGING_VOLTAGE, 100.0f);

    if (!isnan(newBatteryChargingVoltage) && abs(newBatteryChargingVoltage - batteryChargingVoltage) > 0.05)
    {
        batteryChargingVoltage = newBatteryChargingVoltage;
        if (batteryChargingVoltageCallback != nullptr)
            batteryChargingVoltageCallback(batteryChargingVoltage);
    }
};

void AsyncTracer::checkBatteryChargingCurrent()
{
    float newBatteryChargingCurrent = get8BitRegister(BATTERY_CHARGING_CURRENT, 100.0f);

    if (!isnan(newBatteryChargingCurrent) && abs(newBatteryChargingCurrent - batteryChargingCurrent) > 0.05)
    {
        batteryChargingCurrent = newBatteryChargingCurrent;
        if (batteryChargingCurrentCallback != nullptr)
            batteryChargingCurrentCallback(batteryChargingCurrent);
    }
};

void AsyncTracer::checkBatteryChargingPower()
{
    float newBatteryChargingPower = get16BitRegister(BATTERY_CHARGING_POWER, 100.0f);

    if (!isnan(newBatteryChargingPower) && abs(newBatteryChargingPower - batteryChargingPower) > 0.05)
    {
        batteryChargingPower = newBatteryChargingPower;
        if (batteryChargingPowerCallback != nullptr)
            batteryChargingPowerCallback(batteryChargingPower);
    }
};

void AsyncTracer::checkLoadVoltage()
{
    float newLoadVoltage = get8BitRegister(LOAD_VOLTAGE, 100.0f);

    if (!isnan(newLoadVoltage) && abs(newLoadVoltage - loadVoltage) > 0.05)
    {
        loadVoltage = newLoadVoltage;
        if (loadVoltageCallback != nullptr)
            loadVoltageCallback(loadVoltage);
    }
};

void AsyncTracer::checkLoadCurrent()
{
    float newLoadCurrent = get8BitRegister(LOAD_CURRENT, 100.0f);

    if (!isnan(newLoadCurrent) && abs(newLoadCurrent - loadCurrent) > 0.05)
    {
        loadCurrent = newLoadCurrent;
        if (loadCurrentCallback != nullptr)
            loadCurrentCallback(loadCurrent);
    }
};

void AsyncTracer::checkLoadPower()
{
    float newLoadPower = get16BitRegister(LOAD_POWER, 100.0f);

    if (!isnan(newLoadPower) && abs(newLoadPower - loadPower) > 0.05)
    {
        loadPower = newLoadPower;
        if (loadPowerCallback != nullptr)
            loadPowerCallback(loadPower);
    }
};

void AsyncTracer::checkBatteryTemperature()
{
    float newBatteryTemperature = get8BitRegister(BATTERY_TEMPERATURE, 100.0f);

    if (!isnan(newBatteryTemperature) && abs(newBatteryTemperature - batteryTemperature) > 0.05)
    {
        batteryTemperature = newBatteryTemperature;
        if (batteryTemperatureCallback != nullptr)
            batteryTemperatureCallback(batteryTemperature);
    }
};

void AsyncTracer::checkCaseTemperature()
{
    float newCaseTemperature = get8BitRegister(CASE_TEMPERATURE, 100.0f);

    if (!isnan(newCaseTemperature) && abs(newCaseTemperature - caseTemperature) > 0.05)
    {
        caseTemperature = newCaseTemperature;
        if (caseTemperatureCallback != nullptr)
            caseTemperatureCallback(caseTemperature);
    }
};

void AsyncTracer::checkHeatSinkTemperature()
{
    float newHeatSinkTemperature = get8BitRegister(HEAT_SINK_TEMPERATURE, 100.0f);

    if (!isnan(newHeatSinkTemperature) && abs(newHeatSinkTemperature - heatSinkTemperature) > 0.05)
    {
        heatSinkTemperature = newHeatSinkTemperature;
        if (heatSinkTemperatureCallback != nullptr)
            heatSinkTemperatureCallback(heatSinkTemperature);
    }
};

void AsyncTracer::checkBatteryPercentage()
{
    float newBatteryPercentage = get8BitRegister(BATTERY_PERCENTAGE, 100.0f) * 100;

    if (!isnan(newBatteryPercentage) && abs(newBatteryPercentage - batteryPercentage) > 0.05)
    {
        batteryPercentage = newBatteryPercentage;
        if (batteryPercentageCallback != nullptr)
            batteryPercentageCallback(batteryPercentage);
    }
};

void AsyncTracer::checkRemoteBatteryTemperature()
{
    float newRemoteBatteryTemperature = get8BitRegister(REMOTE_BATTERY_TEMPERATURE, 100.0f);

    if (!isnan(newRemoteBatteryTemperature) && abs(newRemoteBatteryTemperature - remoteBatteryTemperature) > 0.05)
    {
        remoteBatteryTemperature = newRemoteBatteryTemperature;
        if (remoteBatteryTemperatureCallback != nullptr)
            remoteBatteryTemperatureCallback(remoteBatteryTemperature);
    }
};

void AsyncTracer::checkSystemVoltage()
{
    float newSystemVoltage = get8BitRegister(SYSTEM_VOLTAGE, 100.0f);

    if (!isnan(newSystemVoltage) && abs(newSystemVoltage - systemVoltage) > 0.05)
    {
        systemVoltage = newSystemVoltage;
        if (systemVoltageCallback != nullptr)
            systemVoltageCallback(systemVoltage);
    }
};

void AsyncTracer::checkJobQueue()
{
    if (!jobQueue.size())
        return;

    std::string job = jobQueue.front();

    if (job == "checkPVVoltage")
        checkPVVoltage();
    if (job == "checkPVCurrent")
        checkPVCurrent();
    if (job == "checkPVPower")
        checkPVPower();
    if (job == "checkBatteryChargingVoltage")
        checkBatteryChargingVoltage();
    if (job == "checkBatteryChargingCurrent")
        checkBatteryChargingCurrent();
    if (job == "checkBatteryChargingPower")
        checkBatteryChargingPower();
    if (job == "checkLoadVoltage")
        checkLoadVoltage();
    if (job == "checkLoadCurrent")
        checkLoadCurrent();
    if (job == "checkLoadPower")
        checkLoadPower();
    if (job == "checkBatteryTemperature")
        checkBatteryTemperature();
    if (job == "checkCaseTemperature")
        checkCaseTemperature();
    if (job == "checkHeatSinkTemperature")
        checkHeatSinkTemperature();
    if (job == "checkBatteryPercentage")
        checkBatteryPercentage();
    if (job == "checkRemoteBatteryTemperature")
        checkRemoteBatteryTemperature();
    if (job == "checkSystemVoltage")
        checkSystemVoltage();

    jobQueue.pop_front();
}