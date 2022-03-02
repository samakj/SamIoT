#ifndef Async_HCSR04_h
#define Async_HCSR04_h

#include <Arduino.h>
#include <vector>

#include <Log.h>
#include <TimeUtils.h>

class AsyncHCSR04
{
public:
    typedef std::function<void(float distance)> DistanceCallback;

    int DISTANCE_NULL_VALUE = -1000;

    int triggerPin = -1;
    int echoPin = -1;

    float temperature = 20;
    int distance = DISTANCE_NULL_VALUE;

    DistanceCallback distanceCallback = nullptr;

    int readPeriod = 1000;
    int lastReadMillis = 0;
    int readCount = 0;

public:
    AsyncHCSR04(
        int triggerPin,
        int echoPin,
        DistanceCallback distanceCallback = nullptr);

    void setDistanceCallback(
        DistanceCallback distanceCallback = nullptr);
    void setTemperature(float temperature);
    float getSpeedOfSound();
    void setup();
    void loop();

private:
    void check();
    void trigger();
    float getEchoTime();
    float getDistance();
    float getMedianDistance(int noTests = 5);
};

#endif
