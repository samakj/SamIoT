#include <AsyncTracer.h>

AsyncTracer::AsyncTracer(
    HardwareSerial* _serial,
    uint8_t _ro,
    uint8_t _re,
    uint8_t _de,
    uint8_t _di,
    uint8_t _address
) : serial(_serial),
    ro(_ro),
    re(_re),
    de(_de),
    di(_di),
    address(_address)
{}

void AsyncTracer::setPVVoltageCallback(
    PVVoltageCallback _pvVoltageCallback
)
{
    pvVoltageCallback = _pvVoltageCallback;
};
void AsyncTracer::setPVCurrentCallback(
    PVCurrentCallback _pvCurrentCallback
)
{
    pvCurrentCallback = _pvCurrentCallback;
};
void AsyncTracer::setPVPowerCallback(
    PVPowerCallback _pvPowerCallback
)
{
    pvPowerCallback = _pvPowerCallback;
};
void AsyncTracer::setBatteryChargingVoltageCallback(
    BatteryChargingVoltageCallback _batteryChargingVoltageCallback
)
{
    batteryChargingVoltageCallback = _batteryChargingVoltageCallback;
};
void AsyncTracer::setBatteryChargingCurrentCallback(
    BatteryChargingCurrentCallback _batteryChargingCurrentCallback
)
{
    batteryChargingCurrentCallback = _batteryChargingCurrentCallback;
};
void AsyncTracer::setBatteryChargingPowerCallback(
    BatteryChargingPowerCallback _batteryChargingPowerCallback
)
{
    batteryChargingPowerCallback = _batteryChargingPowerCallback;
};
void AsyncTracer::setLoadVoltageCallback(
    LoadVoltageCallback _loadVoltageCallback
)
{
    loadVoltageCallback = _loadVoltageCallback;
};
void AsyncTracer::setLoadCurrentCallback(
    LoadCurrentCallback _loadCurrentCallback
)
{
    loadCurrentCallback = _loadCurrentCallback;
};
void AsyncTracer::setLoadPowerCallback(
    LoadPowerCallback _loadPowerCallback
)
{
    loadPowerCallback = _loadPowerCallback;
};
void AsyncTracer::setBatteryTemperatureCallback(
    BatteryTemperatureCallback _batteryTemperatureCallback
)
{
    batteryTemperatureCallback = _batteryTemperatureCallback;
};
void AsyncTracer::setCaseTemperatureCallback(
    CaseTemperatureCallback _caseTemperatureCallback
)
{
    caseTemperatureCallback = _caseTemperatureCallback;
};
void AsyncTracer::setHeatSinkTemperatureCallback(
    HeatSinkTemperatureCallback _heatSinkTemperatureCallback
)
{
    heatSinkTemperatureCallback = _heatSinkTemperatureCallback;
};
void AsyncTracer::setBatteryPercentageCallback(
    BatteryPercentageCallback _batteryPercentageCallback
)
{
    batteryPercentageCallback = _batteryPercentageCallback;
};
void AsyncTracer::setRemoteBatteryTemperatureCallback(
    RemoteBatteryTemperatureCallback _remoteBatteryTemperatureCallback
)
{
    remoteBatteryTemperatureCallback = _remoteBatteryTemperatureCallback;
};
void AsyncTracer::setSystemVoltageCallback(
    SystemVoltageCallback _systemVoltageCallback
)
{
    systemVoltageCallback = _systemVoltageCallback;
};

void AsyncTracer::setup()
{
    client = new ModbusMaster();
    client->begin(address, *serial);
    Log.infof(
        "Tracer initialised at address 0x%02X\n",
        address
    );
};

void AsyncTracer::loop()
{
    if (!client) setup();
    if (TimeUtils.millisSince(lastReadMillis) > (long unsigned int)readPeriod)
    {
        checkPVVoltage();
        checkPVCurrent();
        checkPVPower();
        checkBatteryChargingVoltage();
        checkBatteryChargingCurrent();
        checkBatteryChargingPower();
        checkLoadVoltage();
        checkLoadCurrent();
        checkLoadPower();
        checkBatteryTemperature();
        checkCaseTemperature();
        checkHeatSinkTemperature();
        checkBatteryPercentage();
        checkRemoteBatteryTemperature();
        checkSystemVoltage();
        lastReadMillis = millis();
        readCount++;
    }
};

boolean AsyncTracer::getCoil(uint16_t address, boolean defaultValue)
{
    uint8_t response = client->readCoils(address, 1);

    if (response == client->ku8MBSuccess)
    {
        return client->getResponseBuffer(0x00);
    }

    return defaultValue;
};

float AsyncTracer::get8BitRegister(uint16_t address, float multiplier, float defaultValue)
{
    uint8_t response = client->readInputRegisters(address, 1);

    if (response == client->ku8MBSuccess)
    {
        return client->getResponseBuffer(0x00) / multiplier;
    }

    return defaultValue;
};

float AsyncTracer::get16BitRegister(uint16_t address, float multiplier, float defaultValue)
{
    uint8_t response = client->readInputRegisters(address, 2);

    if (response == client->ku8MBSuccess)
    {
        return (
            client->getResponseBuffer(0x00) |
            client->getResponseBuffer(0x01) << 16
        ) / multiplier;
    }

    return defaultValue;
};

void AsyncTracer::checkPVVoltage()
{
    float newPVVoltage = get8BitRegister(PV_VOLTAGE, 100.0f);

    if (!isnan(newPVVoltage) && newPVVoltage != pvVoltage)
    {
        pvVoltage = newPVVoltage;
        if (pvVoltageCallback != nullptr) pvVoltageCallback(pvVoltage);
    }
};

void AsyncTracer::checkPVCurrent()
{
    float newPVCurrent = get8BitRegister(PV_CURRENT, 100.0f);

    if (!isnan(newPVCurrent) && newPVCurrent != pvCurrent)
    {
        pvCurrent = newPVCurrent;
        if (pvCurrentCallback != nullptr) pvCurrentCallback(pvCurrent);
    }
};

void AsyncTracer::checkPVPower()
{
    float newPVPower = get16BitRegister(PV_POWER, 100.0f);

    if (!isnan(newPVPower) && newPVPower != pvPower)
    {
        pvPower = newPVPower;
        if (pvPowerCallback != nullptr) pvPowerCallback(pvPower);
    }
};

void AsyncTracer::checkBatteryChargingVoltage()
{
    float newBatteryChargingVoltage = get8BitRegister(BATTERY_CHARGING_VOLTAGE, 100.0f);

    if (!isnan(newBatteryChargingVoltage) && newBatteryChargingVoltage != batteryChargingVoltage)
    {
        batteryChargingVoltage = newBatteryChargingVoltage;
        if (batteryChargingVoltageCallback != nullptr) batteryChargingVoltageCallback(batteryChargingVoltage);
    }
};

void AsyncTracer::checkBatteryChargingCurrent()
{
    float newBatteryChargingCurrent = get8BitRegister(BATTERY_CHARGING_CURRENT, 100.0f);

    if (!isnan(newBatteryChargingCurrent) && newBatteryChargingCurrent != batteryChargingCurrent)
    {
        batteryChargingCurrent = newBatteryChargingCurrent;
        if (batteryChargingCurrentCallback != nullptr) batteryChargingCurrentCallback(batteryChargingCurrent);
    }
};

void AsyncTracer::checkBatteryChargingPower()
{
    float newBatteryChargingPower = get16BitRegister(BATTERY_CHARGING_POWER, 100.0f);

    if (!isnan(newBatteryChargingPower) && newBatteryChargingPower != batteryChargingPower)
    {
        batteryChargingPower = newBatteryChargingPower;
        if (batteryChargingPowerCallback != nullptr) batteryChargingPowerCallback(batteryChargingPower);
    }
};

void AsyncTracer::checkLoadVoltage()
{
    float newLoadVoltage = get8BitRegister(LOAD_VOLTAGE, 100.0f);

    if (!isnan(newLoadVoltage) && newLoadVoltage != loadVoltage)
    {
        loadVoltage = newLoadVoltage;
        if (loadVoltageCallback != nullptr) loadVoltageCallback(loadVoltage);
    }
};

void AsyncTracer::checkLoadCurrent()
{
    float newLoadCurrent = get8BitRegister(LOAD_CURRENT, 100.0f);

    if (!isnan(newLoadCurrent) && newLoadCurrent != loadCurrent)
    {
        loadCurrent = newLoadCurrent;
        if (loadCurrentCallback != nullptr) loadCurrentCallback(loadCurrent);
    }
};

void AsyncTracer::checkLoadPower()
{
    float newLoadPower = get16BitRegister(LOAD_POWER, 100.0f);

    if (!isnan(newLoadPower) && newLoadPower != loadPower)
    {
        loadPower = newLoadPower;
        if (loadPowerCallback != nullptr) loadPowerCallback(loadPower);
    }
};

void AsyncTracer::checkBatteryTemperature()
{
    float newBatteryTemperature = get8BitRegister(BATTERY_TEMPERATURE, 100.0f);

    if (!isnan(newBatteryTemperature) && newBatteryTemperature != batteryTemperature)
    {
        batteryTemperature = newBatteryTemperature;
        if (batteryTemperatureCallback != nullptr) batteryTemperatureCallback(batteryTemperature);
    }
};

void AsyncTracer::checkCaseTemperature()
{
    float newCaseTemperature = get8BitRegister(CASE_TEMPERATURE, 100.0f);

    if (!isnan(newCaseTemperature) && newCaseTemperature != caseTemperature)
    {
        caseTemperature = newCaseTemperature;
        if (caseTemperatureCallback != nullptr) caseTemperatureCallback(caseTemperature);
    }
};

void AsyncTracer::checkHeatSinkTemperature()
{
    float newHeatSinkTemperature = get8BitRegister(HEAT_SINK_TEMPERATURE, 100.0f);

    if (!isnan(newHeatSinkTemperature) && newHeatSinkTemperature != heatSinkTemperature)
    {
        heatSinkTemperature = newHeatSinkTemperature;
        if (heatSinkTemperatureCallback != nullptr) heatSinkTemperatureCallback(heatSinkTemperature);
    }
};

void AsyncTracer::checkBatteryPercentage()
{
    float newBatteryPercentage = get8BitRegister(BATTERY_PERCENTAGE, 100.0f);

    if (!isnan(newBatteryPercentage) && newBatteryPercentage != batteryPercentage)
    {
        batteryPercentage = newBatteryPercentage;
        if (batteryPercentageCallback != nullptr) batteryPercentageCallback(batteryPercentage);
    }
};

void AsyncTracer::checkRemoteBatteryTemperature()
{
    float newRemoteBatteryTemperature = get8BitRegister(REMOTE_BATTERY_TEMPERATURE, 100.0f);

    if (!isnan(newRemoteBatteryTemperature) && newRemoteBatteryTemperature != remoteBatteryTemperature)
    {
        remoteBatteryTemperature = newRemoteBatteryTemperature;
        if (remoteBatteryTemperatureCallback != nullptr) remoteBatteryTemperatureCallback(remoteBatteryTemperature);
    }
};

void AsyncTracer::checkSystemVoltage()
{
    float newSystemVoltage = get8BitRegister(SYSTEM_VOLTAGE, 100.0f);

    if (!isnan(newSystemVoltage) && newSystemVoltage != systemVoltage)
    {
        systemVoltage = newSystemVoltage;
        if (systemVoltageCallback != nullptr) systemVoltageCallback(systemVoltage);
    }
};
