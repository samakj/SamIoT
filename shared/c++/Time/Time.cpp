#include "Time.h"

unsigned long SamIoT::Time::millisDiff(unsigned long start, unsigned long end)
{
    if (end >= start)
        return end - start;
    unsigned long startOffset = std::numeric_limits<unsigned long>::max() - start;
    return end + startOffset;
};

unsigned long SamIoT::Time::millisSince(unsigned long start)
{
    return SamIoT::Time::millisDiff(start, millis());
};

std::string SamIoT::Time::getIsoTimestamp()
{
    char buffer[23];
    time_t tm = time(nullptr);
    strftime(buffer, 23, "%FT%TZ", gmtime(&tm));
    return std::string(buffer);
};

std::string SamIoT::Time::formatTime(const char *format)
{
    char buffer[64];
    time_t tm = ::time(nullptr);
    strftime(buffer, sizeof(buffer), format, gmtime(&tm));
    return std::string(buffer);
};