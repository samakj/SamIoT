#ifndef AsyncPZEM_h
#define AsyncPZEM_h

#include <PZEM004Tv30.h>
#include <functional>

#include <Logger.h>
#include <Time.h>

namespace SamIoT::Sensors
{
    typedef std::function<void(float voltage)> VoltageCallback;
    typedef std::function<void(float current)> CurrentCallback;
    typedef std::function<void(float power)> PowerCallback;
    typedef std::function<void(float energy)> EnergyCallback;
    typedef std::function<void(float frequency)> FrequencyCallback;
    typedef std::function<void(float powerFactor)> PowerFactorCallback;

    extern float VOLTAGE_NULL_VALUE;
    extern float CURRENT_NULL_VALUE;
    extern float POWER_NULL_VALUE;
    extern float ENERGY_NULL_VALUE;
    extern float FREQUENCY_NULL_VALUE;
    extern float POWER_FACTOR_NULL_VALUE;

    class PZEM004t
    {
    public:
        float voltage = SamIoT::Sensors::VOLTAGE_NULL_VALUE;
        float current = SamIoT::Sensors::CURRENT_NULL_VALUE;
        float power = SamIoT::Sensors::POWER_NULL_VALUE;
        float energy = SamIoT::Sensors::ENERGY_NULL_VALUE;
        float frequency = SamIoT::Sensors::FREQUENCY_NULL_VALUE;
        float powerFactor = SamIoT::Sensors::POWER_FACTOR_NULL_VALUE;

        PZEM004Tv30 *client;
        HardwareSerial *serial;
        uint8_t rx;
        uint8_t tx;
        uint8_t address;

        VoltageCallback voltageCallback = nullptr;
        CurrentCallback currentCallback = nullptr;
        PowerCallback powerCallback = nullptr;
        EnergyCallback energyCallback = nullptr;
        FrequencyCallback frequencyCallback = nullptr;
        PowerFactorCallback powerFactorCallback = nullptr;

        uint16_t readPeriod = 2000;
        unsigned long lastReadMillis = 0;
        uint32_t readCount = 0;

    private:
        std::vector<std::function<void()>> callstack = {};

    public:
        PZEM004t(
            HardwareSerial *_serial,
            uint8_t _rx,
            uint8_t _tx,
            uint8_t _address);

        void setVoltageCallback(
            VoltageCallback _voltageCallback = nullptr);
        void setCurrentCallback(
            CurrentCallback _currentCallback = nullptr);
        void setPowerCallback(
            PowerCallback _powerCallback = nullptr);
        void setEnergyCallback(
            EnergyCallback _energyCallback = nullptr);
        void setFrequencyCallback(
            FrequencyCallback _frequencyCallback = nullptr);
        void setPowerFactorCallback(
            PowerFactorCallback _powerFactorCallback = nullptr);

        void setup();
        void loop();

    private:
        void checkVoltage();
        void checkCurrent();
        void checkPower();
        void checkEnergy();
        void checkFrequency();
        void checkPowerFactor();
    };
}

#endif