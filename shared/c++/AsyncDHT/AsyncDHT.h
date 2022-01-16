#ifndef Async_DHT_h
#define Async_DHT_h

#include <Arduino.h>
#include <array>
#include <functional>

#include <DHT.h>

#include <Log.h>
#include <TimeUtils.h>
#include <ValuePurgatory.h>


class AsyncDHT
{
    public:
        typedef std::function<void(float temperature)> TemperatureCallback;
        typedef std::function<void(float humidity)> HumidityCallback;

        float TEMPERATURE_NULL_VALUE = -1000.0f;
        float HUMIDITY_NULL_VALUE    = -1000.0f;

        DHT* client = nullptr;
        int pinNo = -1;
        int type = DHT22;

        float temperature = TEMPERATURE_NULL_VALUE;
        float humidity = HUMIDITY_NULL_VALUE;
    
        TemperatureCallback temperatureCallback = nullptr;
        HumidityCallback humidityCallback = nullptr;
        
        int readPeriod = 2000;
        int lastReadMillis = 0;
        int readCount = 0;
        
    private:
        ValuePurgatory<float>* _temperaturePurgatory = nullptr;
        ValuePurgatory<float>* _humidityPurgatory = nullptr;

        int _runInLogNo = 0;
        
    public:
        AsyncDHT(
            int _pinNo,
            int _type = DHT22,
            TemperatureCallback _temperatureCallback = nullptr,
            HumidityCallback _humidityCallback = nullptr
        );

        void setTemperatureCallback(
            TemperatureCallback _temperatureCallback = nullptr
        );
        void setHumidityCallback(
            HumidityCallback _humidityCallback = nullptr
        );
        void setup();
        void loop();

    private:
        void checkHumidity();
        void checkTemperature();
};

#endif
