#ifndef Async_Switch_h
#define Async_Switch_h

#include <Arduino.h>
#include <functional>

#include <Logger.h>
#include <TimeUtils.h>

namespace SamIoT::Peripherals
{
    typedef std::function<void(bool state)> SwitchCallback;

    class Switch
    {
    public:
        uint8_t pinNo = -1;
        uint8_t outPin = -1;

        bool state = false;

        SwitchCallback switchCallback = nullptr;

    private:
    public:
        Switch(
            uint8_t pinNo,
            uint8_t outPin = -1,
            bool defaultState = false,
            SwitchCallback switchCallback = nullptr);

        void setSwitchCallback(
            SwitchCallback switchCallback = nullptr);
        void setup();
        void loop();

    private:
    };
}

#endif
