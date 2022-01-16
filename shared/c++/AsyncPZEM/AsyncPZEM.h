#ifndef AsyncPZEM_h
#define AsyncPZEM_h

#include <functional>
#include <PZEM004Tv30.h>

#include <Log.h>
#include <TimeUtils.h>

class AsyncPZEM
{
    public:
        typedef std::function<void(float voltage)> VoltageCallback;
        typedef std::function<void(float current)> CurrentCallback;
        typedef std::function<void(float power)> PowerCallback;
        typedef std::function<void(float energy)> EnergyCallback;
        typedef std::function<void(float frequency)> FrequencyCallback;
        typedef std::function<void(float powerFactor)> PowerFactorCallback;

        float VOLTAGE_NULL_VALUE = -1000.0f;
        float CURRENT_NULL_VALUE = -1000.0f;
        float POWER_NULL_VALUE = -1000.0f;
        float ENERGY_NULL_VALUE = -1000.0f;
        float FREQUENCY_NULL_VALUE = -1000.0f;
        float POWER_FACTOR_NULL_VALUE = -1000.0f;

        float voltage = VOLTAGE_NULL_VALUE;
        float current = CURRENT_NULL_VALUE;
        float power = POWER_NULL_VALUE;
        float energy = ENERGY_NULL_VALUE;
        float frequency = FREQUENCY_NULL_VALUE;
        float powerFactor = POWER_FACTOR_NULL_VALUE;

        PZEM004Tv30* client;
        HardwareSerial* serial;
        uint8_t rx;
        uint8_t tx;
        uint8_t address;
        
        VoltageCallback voltageCallback = nullptr;
        CurrentCallback currentCallback = nullptr;
        PowerCallback powerCallback = nullptr;
        EnergyCallback energyCallback = nullptr;
        FrequencyCallback frequencyCallback = nullptr;
        PowerFactorCallback powerFactorCallback = nullptr;

        int readPeriod = 2000;
        int lastReadMillis = 0;
        int readCount = 0;

    private:

    public:
        AsyncPZEM(
            HardwareSerial* _serial,
            uint8_t _rx,
            uint8_t _tx,
            uint8_t _address
        );

        void setVoltageCallback(
            VoltageCallback _voltageCallback = nullptr
        );
        void setCurrentCallback(
            CurrentCallback _currentCallback = nullptr
        );
        void setPowerCallback(
            PowerCallback _powerCallback = nullptr
        );
        void setEnergyCallback(
            EnergyCallback _energyCallback = nullptr
        );
        void setFrequencyCallback(
            FrequencyCallback _frequencyCallback = nullptr
        );
        void setPowerFactorCallback(
            PowerFactorCallback _powerFactorCallback = nullptr
        );

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

#endif