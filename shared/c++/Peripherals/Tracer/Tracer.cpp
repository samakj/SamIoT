#include <AsyncTracer.h>

namespace SamIoT::Peripherals
{
    float NULL_RESPONSE_VALUE = -1000.0f;

    Tracer::Tracer(
        HardwareSerial *_serial,
        uint8_t _tx,
        uint8_t _rx,
        uint8_t _address) : serial(_serial),
                            tx(_tx),
                            rx(_rx),
                            address(_address)
    {
    }

    void Tracer::setPVVoltageCallback(
        PVVoltageCallback _pvVoltageCallback)
    {
        this->pvVoltageCallback = _pvVoltageCallback;
    };
    void Tracer::setPVCurrentCallback(
        PVCurrentCallback _pvCurrentCallback)
    {
        this->pvCurrentCallback = _pvCurrentCallback;
    };
    void Tracer::setPVPowerCallback(
        PVPowerCallback _pvPowerCallback)
    {
        this->pvPowerCallback = _pvPowerCallback;
    };
    void Tracer::setBatteryChargingVoltageCallback(
        BatteryChargingVoltageCallback _batteryChargingVoltageCallback)
    {
        this->batteryChargingVoltageCallback = _batteryChargingVoltageCallback;
    };
    void Tracer::setBatteryChargingCurrentCallback(
        BatteryChargingCurrentCallback _batteryChargingCurrentCallback)
    {
        this->batteryChargingCurrentCallback = _batteryChargingCurrentCallback;
    };
    void Tracer::setBatteryChargingPowerCallback(
        BatteryChargingPowerCallback _batteryChargingPowerCallback)
    {
        this->batteryChargingPowerCallback = _batteryChargingPowerCallback;
    };
    void Tracer::setLoadVoltageCallback(
        LoadVoltageCallback _loadVoltageCallback)
    {
        this->loadVoltageCallback = _loadVoltageCallback;
    };
    void Tracer::setLoadCurrentCallback(
        LoadCurrentCallback _loadCurrentCallback)
    {
        this->loadCurrentCallback = _loadCurrentCallback;
    };
    void Tracer::setLoadPowerCallback(
        LoadPowerCallback _loadPowerCallback)
    {
        this->loadPowerCallback = _loadPowerCallback;
    };
    void Tracer::setBatteryTemperatureCallback(
        BatteryTemperatureCallback _batteryTemperatureCallback)
    {
        this->batteryTemperatureCallback = _batteryTemperatureCallback;
    };
    void Tracer::setCaseTemperatureCallback(
        CaseTemperatureCallback _caseTemperatureCallback)
    {
        this->caseTemperatureCallback = _caseTemperatureCallback;
    };
    void Tracer::setHeatSinkTemperatureCallback(
        HeatSinkTemperatureCallback _heatSinkTemperatureCallback)
    {
        this->heatSinkTemperatureCallback = _heatSinkTemperatureCallback;
    };
    void Tracer::setBatteryPercentageCallback(
        BatteryPercentageCallback _batteryPercentageCallback)
    {
        this->batteryPercentageCallback = _batteryPercentageCallback;
    };
    void Tracer::setRemoteBatteryTemperatureCallback(
        RemoteBatteryTemperatureCallback _remoteBatteryTemperatureCallback)
    {
        this->remoteBatteryTemperatureCallback = _remoteBatteryTemperatureCallback;
    };
    void Tracer::setSystemVoltageCallback(
        SystemVoltageCallback _systemVoltageCallback)
    {
        this->systemVoltageCallback = _systemVoltageCallback;
    };

    void Tracer::setup(void (*preTransmission)(), void (*postTransmission)())
    {
        this->client = new ModbusMaster();
        this->client->begin(this->address, *this->serial);
        SamIoT::Logger::infof(
            "Tracer initialised at address 0x%02X\n",
            this->address);
    };

    void Tracer::loop()
    {
        if (!this->client)
            this->setup();
        if (
            SamIoT::Time::millisSince(this->lastReadMillis) >
                (unsigned long)this->readPeriod &&
            !callstack.size())
        {
            if (this->pvVoltageCallback != nullptr)
                this->callstack.push_back(
                    [this]()
                    { this->checkPVVoltage(); });
            if (this->pvCurrentCallback != nullptr)
                this->callstack.push_back(
                    [this]()
                    { this->checkPVCurrent(); });
            if (this->pvPowerCallback != nullptr)
                this->callstack.push_back(
                    [this]()
                    { this->checkPVPower(); });
            if (this->batteryChargingVoltageCallback != nullptr)
                this->callstack.push_back(
                    [this]()
                    { this->checkBatteryChargingVoltage(); });
            if (this->batteryChargingCurrentCallback != nullptr)
                this->callstack.push_back(
                    [this]()
                    { this->checkBatteryChargingCurrent(); });
            if (this->batteryChargingPowerCallback != nullptr)
                this->callstack.push_back(
                    [this]()
                    { this->checkBatteryChargingPower(); });
            if (this->loadVoltageCallback != nullptr)
                this->callstack.push_back(
                    [this]()
                    { this->checkLoadVoltage(); });
            if (this->loadCurrentCallback != nullptr)
                this->callstack.push_back(
                    [this]()
                    { this->checkLoadCurrent(); });
            if (this->loadPowerCallback != nullptr)
                this->callstack.push_back(
                    [this]()
                    { this->checkLoadPower(); });
            if (this->batteryTemperatureCallback != nullptr)
                this->callstack.push_back(
                    [this]()
                    { this->checkBatteryTemperature(); });
            if (this->caseTemperatureCallback != nullptr)
                this->callstack.push_back(
                    [this]()
                    { this->checkCaseTemperature(); });
            if (this->heatSinkTemperatureCallback != nullptr)
                this->callstack.push_back(
                    [this]()
                    { this->checkHeatSinkTemperature(); });
            if (this->batteryPercentageCallback != nullptr)
                this->callstack.push_back(
                    [this]()
                    { this->checkBatteryPercentage(); });
            if (this->remoteBatteryTemperatureCallback != nullptr)
                this->callstack.push_back(
                    [this]()
                    { this->checkRemoteBatteryTemperature(); });
            if (this->systemVoltageCallback != nullptr)
                this->callstack.push_back(
                    [this]()
                    { this->checkSystemVoltage(); });
            this->lastReadMillis = millis();
            this->readCount++;
        }

        if (this->callstack.size())
        {
            this->callstack.back()();
            this->callstack.pop_back();
        }
    };

    boolean Tracer::getCoil(uint16_t address, boolean defaultValue)
    {
        uint8_t response = this->client->readCoils(address, 1);
        if (response == this->client->ku8MBSuccess)
            return this->client->getResponseBuffer(0x00);

        SamIoT::Logger::warnf("Failed to get coil at 0x%02X: 0x%02X\n", address, response);
        return this->defaultValue;
    };

    float Tracer::get8BitRegister(uint16_t address, float multiplier, float defaultValue)
    {
        uint8_t response = this->client->readInputRegisters(address, 1);
        if (response == this->client->ku8MBSuccess)
            return this->client->getResponseBuffer(0x00) / multiplier;

        SamIoT::Logger::warnf("Failed to get 8 bit register at 0x%02X: 0x%02X\n", address, response);
        return defaultValue;
    };

    float Tracer::get16BitRegister(uint16_t address, float multiplier, float defaultValue)
    {
        uint8_t response = this->client->readInputRegisters(address, 2);
        if (response == this->client->ku8MBSuccess)
            return (
                       this->client->getResponseBuffer(0x00) |
                       this->client->getResponseBuffer(0x01) << 16) /
                   multiplier;

        SamIoT::Logger::warnf("Failed to get 16 bit register at 0x%02X: 0x%02X\n", address, response);
        return defaultValue;
    };

    void Tracer::checkPVVoltage()
    {
        float newPVVoltage = get8BitRegister(
            SamIoT::Peripherals::Tracer::RegisterAddresses::PV_VOLTAGE,
            100.0f);

        if (!isnan(newPVVoltage) && abs(newPVVoltage - this->pvVoltage) > 0.05)
        {
            if (newPVVoltage != SamIoT::Peripherals::NULL_RESPONSE_VALUE)
                this->pvVoltageCallback(newPVVoltage);
            this->pvVoltage = newPVVoltage;
        }
    };

    void Tracer::checkPVCurrent()
    {
        float newPVCurrent = get8BitRegister(
            SamIoT::Peripherals::Tracer::RegisterAddresses::PV_CURRENT,
            100.0f);

        if (!isnan(newPVCurrent) && abs(newPVCurrent - this->pvCurrent) > 0.05)
        {
            if (newPVCurrent != SamIoT::Peripherals::NULL_RESPONSE_VALUE)
                this->pvCurrentCallback(newPVCurrent);
            this->pvCurrent = newPVCurrent;
        }
    };

    void Tracer::checkPVPower()
    {
        float newPVPower = get16BitRegister(
            SamIoT::Peripherals::Tracer::RegisterAddresses::PV_POWER,
            100.0f);

        if (!isnan(newPVPower) && abs(newPVPower - this->pvPower) > 0.05)
        {
            if (newPVPower != SamIoT::Peripherals::NULL_RESPONSE_VALUE)
                this->pvPowerCallback(newPVPower);
            this->pvPower = newPVPower;
        }
    };

    void Tracer::checkBatteryChargingVoltage()
    {
        float newBatteryChargingVoltage = get8BitRegister(
            SamIoT::Peripherals::Tracer::RegisterAddresses::BATTERY_CHARGING_VOLTAGE,
            100.0f);

        if (!isnan(newBatteryChargingVoltage) && abs(newBatteryChargingVoltage - this->batteryChargingVoltage) > 0.05)
        {
            if (newBatteryChargingVoltage != SamIoT::Peripherals::NULL_RESPONSE_VALUE)
                this->batteryChargingVoltageCallback(newBatteryChargingVoltage);
            this->batteryChargingVoltage = newBatteryChargingVoltage;
        }
    };

    void Tracer::checkBatteryChargingCurrent()
    {
        float newBatteryChargingCurrent = get8BitRegister(
            SamIoT::Peripherals::Tracer::RegisterAddresses::BATTERY_CHARGING_CURRENT,
            100.0f);

        if (!isnan(newBatteryChargingCurrent) && abs(newBatteryChargingCurrent - this->batteryChargingCurrent) > 0.05)
        {
            if (newBatteryChargingCurrent != SamIoT::Peripherals::NULL_RESPONSE_VALUE)
                this->batteryChargingCurrentCallback(newBatteryChargingCurrent);
            this->batteryChargingCurrent = newBatteryChargingCurrent;
        }
    };

    void Tracer::checkBatteryChargingPower()
    {
        float newBatteryChargingPower = get16BitRegister(
            SamIoT::Peripherals::Tracer::RegisterAddresses::BATTERY_CHARGING_POWER,
            100.0f);

        if (!isnan(newBatteryChargingPower) && abs(newBatteryChargingPower - this->batteryChargingPower) > 0.05)
        {
            if (newBatteryChargingPower != SamIoT::Peripherals::NULL_RESPONSE_VALUE)
                this->batteryChargingPowerCallback(newBatteryChargingPower);
            this->batteryChargingPower = newBatteryChargingPower;
        }
    };

    void Tracer::checkLoadVoltage()
    {
        float newLoadVoltage = get8BitRegister(
            SamIoT::Peripherals::Tracer::RegisterAddresses::LOAD_VOLTAGE,
            100.0f);

        if (!isnan(newLoadVoltage) && abs(newLoadVoltage - this->loadVoltage) > 0.05)
        {
            if (newLoadVoltage != SamIoT::Peripherals::NULL_RESPONSE_VALUE)
                this->loadVoltageCallback(newLoadVoltage);
            this->loadVoltage = newLoadVoltage;
        }
    };

    void Tracer::checkLoadCurrent()
    {
        float newLoadCurrent = get8BitRegister(
            SamIoT::Peripherals::Tracer::RegisterAddresses::LOAD_CURRENT,
            100.0f);

        if (!isnan(newLoadCurrent) && abs(newLoadCurrent - this->loadCurrent) > 0.05)
        {
            if (newLoadCurrent != SamIoT::Peripherals::NULL_RESPONSE_VALUE)
                this->loadCurrentCallback(newLoadCurrent);
            this->loadCurrent = newLoadCurrent;
        }
    };

    void Tracer::checkLoadPower()
    {
        float newLoadPower = get16BitRegister(
            SamIoT::Peripherals::Tracer::RegisterAddresses::LOAD_POWER,
            100.0f);

        if (!isnan(newLoadPower) && abs(newLoadPower - this->loadPower) > 0.05)
        {
            if (newLoadPower != SamIoT::Peripherals::NULL_RESPONSE_VALUE)
                this->loadPowerCallback(newLoadPower);
            this->loadPower = newLoadPower;
        }
    };

    void Tracer::checkBatteryTemperature()
    {
        float newBatteryTemperature = get8BitRegister(
            SamIoT::Peripherals::Tracer::RegisterAddresses::BATTERY_TEMPERATURE,
            100.0f);

        if (!isnan(newBatteryTemperature) && abs(newBatteryTemperature - this->batteryTemperature) > 0.05)
        {
            if (newBatteryTemperature != SamIoT::Peripherals::NULL_RESPONSE_VALUE)
                this->batteryTemperatureCallback(newBatteryTemperature);
            this->batteryTemperature = newBatteryTemperature;
        }
    };

    void Tracer::checkCaseTemperature()
    {
        float newCaseTemperature = get8BitRegister(
            SamIoT::Peripherals::Tracer::RegisterAddresses::CASE_TEMPERATURE,
            100.0f);

        if (!isnan(newCaseTemperature) && abs(newCaseTemperature - this->caseTemperature) > 0.05)
        {
            if (newCaseTemperature != SamIoT::Peripherals::NULL_RESPONSE_VALUE)
                this->caseTemperatureCallback(newCaseTemperature);
            this->caseTemperature = newCaseTemperature;
        }
    };

    void Tracer::checkHeatSinkTemperature()
    {
        float newHeatSinkTemperature = get8BitRegister(
            SamIoT::Peripherals::Tracer::RegisterAddresses::HEAT_SINK_TEMPERATURE,
            100.0f);

        if (!isnan(newHeatSinkTemperature) && abs(newHeatSinkTemperature - this->heatSinkTemperature) > 0.05)
        {
            if (newHeatSinkTemperature != SamIoT::Peripherals::NULL_RESPONSE_VALUE)
                this->heatSinkTemperatureCallback(newHeatSinkTemperature);
            this->heatSinkTemperature = newHeatSinkTemperature;
        }
    };

    void Tracer::checkBatteryPercentage()
    {
        float newBatteryPercentage = get8BitRegister(
                                         SamIoT::Peripherals::Tracer::RegisterAddresses::BATTERY_PERCENTAGE,
                                         100.0f) *
                                     100;

        if (!isnan(newBatteryPercentage) && abs(newBatteryPercentage - this->batteryPercentage) > 0.05)
        {
            if (newBatteryPercentage != SamIoT::Peripherals::NULL_RESPONSE_VALUE)
                this->batteryPercentageCallback(newBatteryPercentage);
            this->batteryPercentage = newBatteryPercentage;
        }
    };

    void Tracer::checkRemoteBatteryTemperature()
    {
        float newRemoteBatteryTemperature = get8BitRegister(
            SamIoT::Peripherals::Tracer::RegisterAddresses::REMOTE_BATTERY_TEMPERATURE,
            100.0f);

        if (!isnan(newRemoteBatteryTemperature) && abs(newRemoteBatteryTemperature - this->remoteBatteryTemperature) > 0.05)
        {
            if (newRemoteBatteryTemperature != SamIoT::Peripherals::NULL_RESPONSE_VALUE)
                this->remoteBatteryTemperatureCallback(newRemoteBatteryTemperature);
            this->remoteBatteryTemperature = newRemoteBatteryTemperature;
        }
    };

    void Tracer::checkSystemVoltage()
    {
        float newSystemVoltage = get8BitRegister(
            SamIoT::Peripherals::Tracer::RegisterAddresses::SYSTEM_VOLTAGE,
            100.0f);

        if (!isnan(newSystemVoltage) && abs(newSystemVoltage - this->systemVoltage) > 0.05)
        {
            if (newSystemVoltage != SamIoT::Peripherals::NULL_RESPONSE_VALUE)
                this->systemVoltageCallback(newSystemVoltage);
            this->systemVoltage = newSystemVoltage;
        }
    };
}