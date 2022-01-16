#ifndef Async_Switch_h
#define Async_Switch_h

#include <Arduino.h>
#include <functional>

#include <Log.h>
#include <TimeUtils.h>


class AsyncSwitch
{
    public:
        typedef std::function<void(bool state)> SwitchCallback;

        int pinNo = -1;
        int outPin = -1;

        bool state = false;
    
        SwitchCallback switchCallback = nullptr;

    private:
        
    public:
        AsyncSwitch(
            int pinNo,
            int outPin = -1,
            bool defaultState = false,
            SwitchCallback switchCallback = nullptr
        );

        void setSwitchCallback(
            SwitchCallback switchCallback = nullptr
        );
        void setup();
        void loop();

    private:
};

#endif
