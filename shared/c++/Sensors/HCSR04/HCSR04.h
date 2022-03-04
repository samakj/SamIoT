#ifndef _SamIoT_Sensors_HCSR04_h
#define _SamIoT_Sensors_HCSR04_h

#include <Arduino.h>
#include <vector>

#include <Logger.h>
#include <Time.h>

namespace SamIoT::Sensors
{
    typedef std::function<void(float distance)> DistanceCallback;
    static float DISTANCE_NULL_VALUE = -1000.0f;

    class HCSR04
    {
    public:
        uint8_t triggerPin = -1;
        uint8_t echoPin = -1;

        float temperature = 20;
        float distance = DISTANCE_NULL_VALUE;

        DistanceCallback distanceCallback = nullptr;

        uint16_t readPeriod = 1000;
        unsigned long lastReadMillis = 0;
        uint32_t readCount = 0;

    public:
        HCSR04(
            uint8_t triggerPin,
            uint8_t echoPin,
            DistanceCallback distanceCallback = nullptr);

        void setDistanceCallback(
            DistanceCallback distanceCallback = nullptr);
        void setTemperature(float temperature);

        float getSpeedOfSound();
        float getEchoTime();
        float getDistance();
        float getMedianDistance(uint8_t noTests = 5);

        void check();
        void trigger();
        void setup();
        void loop();
    };
}

#endif
