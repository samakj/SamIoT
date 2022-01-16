#include <TimeUtils.h>

TimeUtilsClass TimeUtilsClass::_instance;

TimeUtilsClass& TimeUtilsClass::getInstance()
{
    return _instance;
}

unsigned long TimeUtilsClass::millisDiff(unsigned long start, unsigned long end)
{
    if (end >= start) return end - start;
    unsigned long startOffset = std::numeric_limits<unsigned long>::max() - start;
    return end + startOffset;
};

unsigned long TimeUtilsClass::millisSince(unsigned long start)
{
    return millisDiff(start, millis());
};

std::string TimeUtilsClass::getIsoTimestamp()
{
    char buffer[23];
    time_t tm = time(nullptr);
    strftime(buffer, 23, "%FT%TZ", gmtime(&tm));
    return std::string(buffer);
};

std::string TimeUtilsClass::formatTime(const char* format)
{
    char buffer[64];
    time_t tm = time(nullptr);
    strftime(buffer, sizeof(buffer), format, gmtime(&tm));
    return std::string(buffer);
};