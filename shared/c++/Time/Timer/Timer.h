#ifndef _SamIoT_Time_Timer_h
#define _SamIoT_Time_Timer_h

#include <Arduino.h>
#include <limits>
#include <string>
#include <unordered_map>

#include <Logger.h>

namespace SamIoT::Time
{
    unsigned long millisDiff(unsigned long start, unsigned long end);
    unsigned long millisSince(unsigned long start);
    std::string getIsoTimestamp();
    std::string formatTime(const char *format);
}

namespace SamIoT::Time::Timer
{
    class TimerData
    {
    public:
        uint16_t min = 65535;
        uint16_t max = 0;
        uint16_t total = 0;
        uint16_t count = 0;

    private:
        unsigned long start = 0;

    public:
        void reset();
        void start_loop();
        void end_loop();
        void add(uint16_t value);
    };

    static uint16_t reportPeriod = 60000;

    static uint32_t loopCount = 0;
    static unsigned long lastLoopReportMillis = -1;

    static std::unordered_map<const char *, SamIoT::Time::Timer::TimerData> data = {};

    void start(const char *id);
    void end(const char *id);
    void loop();
};

#endif