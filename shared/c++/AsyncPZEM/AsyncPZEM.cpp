#include <AsyncPZEM.h>

AsyncPZEM::AsyncPZEM(
    HardwareSerial* _serial,
    uint8_t _rx,
    uint8_t _tx,
    uint8_t _address
) : serial(_serial),
    rx(_rx),
    tx(_tx),
    address(_address)
{}

void AsyncPZEM::setVoltageCallback(
    VoltageCallback _voltageCallback
)
{
    voltageCallback = _voltageCallback;
};

void AsyncPZEM::setCurrentCallback(
    CurrentCallback _currentCallback
)
{
    currentCallback = _currentCallback;
};

void AsyncPZEM::setPowerCallback(
    PowerCallback _powerCallback
)
{
    powerCallback = _powerCallback;
};

void AsyncPZEM::setEnergyCallback(
    EnergyCallback _energyCallback
)
{
    energyCallback = _energyCallback;
};

void AsyncPZEM::setFrequencyCallback(
    FrequencyCallback _frequencyCallback
)
{
    frequencyCallback = _frequencyCallback;
};

void AsyncPZEM::setPowerFactorCallback(
    PowerFactorCallback _powerFactorCallback
)
{
    powerFactorCallback = _powerFactorCallback;
};

void AsyncPZEM::setup()
{
    client = new PZEM004Tv30(serial, rx, tx, address);
    Log.infof(
        "PZEM sensor initialised on pins %d and %d at address 0x%02X\n",
        tx,
        rx,
        address
    );
};

void AsyncPZEM::loop()
{
    if (!client) setup();
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

    if (!isnan(newVoltage) && newVoltage != voltage)
    {
        voltage = newVoltage;
        if (voltageCallback != nullptr) voltageCallback(voltage);
    }
};

void AsyncPZEM::checkCurrent()
{
    float newCurrent = client->current();

    if (!isnan(newCurrent) && newCurrent != current)
    {
        current = newCurrent;
        if (currentCallback != nullptr) currentCallback(current);
    }
};

void AsyncPZEM::checkPower()
{
    float newPower = client->power();

    if (!isnan(newPower) && newPower != power)
    {
        power = newPower;
        if (powerCallback != nullptr) powerCallback(power);
    }
};

void AsyncPZEM::checkEnergy()
{
    float newEnergy = client->energy();

    if (!isnan(newEnergy) && newEnergy != energy)
    {
        energy = newEnergy;
        if (energyCallback != nullptr) energyCallback(energy);
    }
};

void AsyncPZEM::checkFrequency()
{
    float newFrequency = client->frequency();

    if (!isnan(newFrequency) && newFrequency != frequency)
    {
        frequency = newFrequency;
        if (frequencyCallback != nullptr) frequencyCallback(frequency);
    }
};

void AsyncPZEM::checkPowerFactor()
{
    float newPowerFactor = client->pf();

    if (!isnan(newPowerFactor) && newPowerFactor != powerFactor)
    {
        powerFactor = newPowerFactor;
        if (powerFactorCallback != nullptr) powerFactorCallback(powerFactor);
    }
};
