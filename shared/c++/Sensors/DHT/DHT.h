#ifndef _SamIoT_Sensors_DHT_h
#define _SamIoT_Sensors_DHT_h

#include <Arduino.h>
#include <array>
#include <functional>

#include <DHT.h>

#include <Logger.h>
#include <Time.h>

class _DHT : public DHT
{
public:
    _DHT(uint8_t pin, uint8_t type, uint8_t count = 6) : DHT(pin, type, count) {}
};

namespace SamIoT::Sensors
{
    typedef std::function<void(float temperature)> TemperatureCallback;
    typedef std::function<void(float humidity)> HumidityCallback;

    extern float TEMPERATURE_NULL_VALUE;
    extern float HUMIDITY_NULL_VALUE;

    class DHT
    {
    public:
        _DHT *client = nullptr;
        uint8_t pinNo = -1;
        uint8_t type = DHT22;

        float temperature = SamIoT::Sensors::TEMPERATURE_NULL_VALUE;
        float humidity = SamIoT::Sensors::HUMIDITY_NULL_VALUE;

        TemperatureCallback temperatureCallback = nullptr;
        HumidityCallback humidityCallback = nullptr;

        uint16_t readPeriod = 2000;
        unsigned long lastReadMillis = 0;
        uint32_t readCount = 0;

    private:
        bool nanTemperatureReported = false;
        bool nanHumidityReported = false;

    public:
        DHT(
            uint8_t _pinNo,
            uint8_t _type = DHT22,
            TemperatureCallback _temperatureCallback = nullptr,
            HumidityCallback _humidityCallback = nullptr);

        void setTemperatureCallback(
            TemperatureCallback _temperatureCallback = nullptr);
        void setHumidityCallback(
            HumidityCallback _humidityCallback = nullptr);
        void setup();
        void loop();

    private:
        void checkHumidity();
        void checkTemperature();
    };
};
#endif
