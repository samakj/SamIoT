#ifndef Time_Utils_h
#define Time_Utils_h

#include <Arduino.h>
#include <string>
#include <time.h>

class TimeUtilsClass
{
    public:
        TimeUtilsClass (const TimeUtilsClass&) = delete;
        static TimeUtilsClass& getInstance();

        static unsigned long millisDiff(unsigned long start, unsigned long end);
        static unsigned long millisSince(unsigned long start);
        static std::string getIsoTimestamp();
        static std::string formatTime(const char* format);
    
    private:
        TimeUtilsClass() {};
        static TimeUtilsClass _instance;
};

static TimeUtilsClass& TimeUtils = TimeUtilsClass::getInstance();

#endif