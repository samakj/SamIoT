#include <AsyncPZEM.h>

AsyncPZEM::AsyncPZEM(
    HardwareSerial *_serial,
    uint8_t _rx,
    uint8_t _tx,
    uint8_t _address) : serial(_serial),
                        rx(_rx),
                        tx(_tx),
                        address(_address)
{
}

void AsyncPZEM::setVoltageCallback(
    VoltageCallback _voltageCallback)
{
    voltageCallback = _voltageCallback;
};

void AsyncPZEM::setCurrentCallback(
    CurrentCallback _currentCallback)
{
    currentCallback = _currentCallback;
};

void AsyncPZEM::setPowerCallback(
    PowerCallback _powerCallback)
{
    powerCallback = _powerCallback;
};

void AsyncPZEM::setEnergyCallback(
    EnergyCallback _energyCallback)
{
    energyCallback = _energyCallback;
};

void AsyncPZEM::setFrequencyCallback(
    FrequencyCallback _frequencyCallback)
{
    frequencyCallback = _frequencyCallback;
};

void AsyncPZEM::setPowerFactorCallback(
    PowerFactorCallback _powerFactorCallback)
{
    powerFactorCallback = _powerFactorCallback;
};

void AsyncPZEM::setup()
{
    client = new PZEM004Tv30(serial, rx, tx, address);
    Sam::Log.infof(
        "PZEM sensor initialised on pins %d and %d at address 0x%02X\n",
        tx,
        rx,
        address);
};

void AsyncPZEM::loop()
{
    if (!client)
        setup();
    if (TimeUtils.millisSince(lastReadMillis) > (long unsigned int)readPeriod)
    {
        checkVoltage();
        checkCurrent();
        checkPower();
        checkEnergy();
        checkFrequency();
        checkPowerFactor();
        lastReadMillis = millis();
        readCount++;
    }
};

void AsyncPZEM::checkVoltage()
{
    float newVoltage = client->voltage();

    if (!isnan(newVoltage) && abs(newVoltage - voltage) > 0.1)
    {
        voltage = newVoltage;
        if (voltageCallback != nullptr)
            voltageCallback(voltage);
    }
};

void AsyncPZEM::checkCurrent()
{
    float newCurrent = client->current();

    if (!isnan(newCurrent) && abs(newCurrent - current) > 0.01)
    {
        current = newCurrent;
        if (currentCallback != nullptr)
            currentCallback(current);
    }
};

void AsyncPZEM::checkPower()
{
    float newPower = client->power();

    if (!isnan(newPower) && abs(newPower - power) > 0.1)
    {
        power = newPower;
        if (powerCallback != nullptr)
            powerCallback(power);
    }
};

void AsyncPZEM::checkEnergy()
{
    float newEnergy = client->energy() * 1000;

    if (!isnan(newEnergy) && abs(newEnergy - energy) > 1)
    {
        energy = newEnergy;
        if (energyCallback != nullptr)
            energyCallback(energy);
    }
};

void AsyncPZEM::checkFrequency()
{
    float newFrequency = client->frequency();

    if (!isnan(newFrequency) && abs(newFrequency - frequency) > 0.1)
    {
        frequency = newFrequency;
        if (frequencyCallback != nullptr)
            frequencyCallback(frequency);
    }
};

void AsyncPZEM::checkPowerFactor()
{
    float newPowerFactor = client->pf();

    if (!isnan(newPowerFactor) && abs(newPowerFactor - powerFactor) > 0.02)
    {
        powerFactor = newPowerFactor;
        if (powerFactorCallback != nullptr)
            powerFactorCallback(powerFactor);
    }
};
