#ifndef _SamIoT_Time_h
#define _SamIoT_Time_h

#include <Arduino.h>
#include <string>
#include <time.h>

#include "NTP/NTP.h"
#include "Timer/Timer.h"

namespace SamIoT::Time
{
    unsigned long millisDiff(unsigned long start, unsigned long end);
    unsigned long millisSince(unsigned long start);
    std::string getIsoTimestamp();
    std::string formatTime(const char *format);
};

#endif