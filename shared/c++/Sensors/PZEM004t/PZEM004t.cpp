#include "PZEM004t.h"

SamIoT::Sensors::PZEM004t::PZEM004t(
    HardwareSerial *_serial,
    uint8_t _rx,
    uint8_t _tx,
    uint8_t _address) : serial(_serial),
                        rx(_rx),
                        tx(_tx),
                        address(_address)
{
}

void SamIoT::Sensors::PZEM004t::setVoltageCallback(
    VoltageCallback _voltageCallback)
{
    this->voltageCallback = _voltageCallback;
};

void SamIoT::Sensors::PZEM004t::setCurrentCallback(
    CurrentCallback _currentCallback)
{
    this->currentCallback = _currentCallback;
};

void SamIoT::Sensors::PZEM004t::setPowerCallback(
    PowerCallback _powerCallback)
{
    this->powerCallback = _powerCallback;
};

void SamIoT::Sensors::PZEM004t::setEnergyCallback(
    EnergyCallback _energyCallback)
{
    this->energyCallback = _energyCallback;
};

void SamIoT::Sensors::PZEM004t::setFrequencyCallback(
    FrequencyCallback _frequencyCallback)
{
    this->frequencyCallback = _frequencyCallback;
};

void SamIoT::Sensors::PZEM004t::setPowerFactorCallback(
    PowerFactorCallback _powerFactorCallback)
{
    this->powerFactorCallback = _powerFactorCallback;
};

void SamIoT::Sensors::PZEM004t::setup()
{
#ifdef ESP8266
    this->client = new PZEM004Tv30(this->rx, this->tx, this->address);
#else
    this->client = new PZEM004Tv30(this->serial, this->rx, this->tx, this->address);
#endif
    SamIoT::Logger::infof(
        "PZEM sensor initialised on pins %d and %d at address 0x%02X\n",
        this->tx,
        this->rx,
        this->address);
};

void SamIoT::Sensors::PZEM004t::loop()
{
    if (!this->client)
        this->setup();
    if (
        SamIoT::Time::millisSince(this->lastReadMillis) >
            (unsigned long)this->readPeriod &&
        !this->callstack.size())
    {
        this->callstack.push_back(
            [this]()
            { this->checkVoltage(); });
        this->callstack.push_back(
            [this]()
            { this->checkCurrent(); });
        this->callstack.push_back(
            [this]()
            { this->checkPower(); });
        this->callstack.push_back(
            [this]()
            { this->checkEnergy(); });
        this->callstack.push_back(
            [this]()
            { this->checkFrequency(); });
        this->callstack.push_back(
            [this]()
            { this->checkPowerFactor(); });
        this->lastReadMillis = millis();
        this->readCount++;
    }

    if (this->callstack.size())
    {
        this->callstack.back()();
        this->callstack.pop_back();
    }
};

void SamIoT::Sensors::PZEM004t::checkVoltage()
{
    float newVoltage = this->client->voltage();

    if (!isnan(newVoltage) && abs(newVoltage - this->voltage) > 0.1)
    {
        if (this->voltageCallback != nullptr)
            this->voltageCallback(newVoltage);
        this->voltage = newVoltage;
    }
};

void SamIoT::Sensors::PZEM004t::checkCurrent()
{
    float newCurrent = this->client->current();

    if (!isnan(newCurrent) && abs(newCurrent - this->current) > 0.01)
    {
        if (this->currentCallback != nullptr)
            this->currentCallback(newCurrent);
        this->current = newCurrent;
    }
};

void SamIoT::Sensors::PZEM004t::checkPower()
{
    float newPower = this->client->power();

    if (!isnan(newPower) && abs(newPower - this->power) > 0.1)
    {
        if (this->powerCallback != nullptr)
            this->powerCallback(newPower);
        this->power = newPower;
    }
};

void SamIoT::Sensors::PZEM004t::checkEnergy()
{
    float newEnergy = this->client->energy() * 1000;

    if (!isnan(newEnergy) && abs(newEnergy - this->energy) > 1)
    {
        if (this->energyCallback != nullptr)
            this->energyCallback(newEnergy);
        this->energy = newEnergy;
    }
};

void SamIoT::Sensors::PZEM004t::checkFrequency()
{
    float newFrequency = this->client->frequency();

    if (!isnan(newFrequency) && abs(newFrequency - this->frequency) > 0.1)
    {
        if (this->frequencyCallback != nullptr)
            this->frequencyCallback(newFrequency);
        this->frequency = newFrequency;
    }
};

void SamIoT::Sensors::PZEM004t::checkPowerFactor()
{
    float newPowerFactor = this->client->pf();

    if (!isnan(newPowerFactor) && abs(newPowerFactor - this->powerFactor) > 0.02)
    {
        if (this->powerFactorCallback != nullptr)
            this->powerFactorCallback(newPowerFactor);
        this->powerFactor = newPowerFactor;
    }
};
