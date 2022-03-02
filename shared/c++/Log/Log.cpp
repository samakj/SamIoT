#include <Log.h>

Sam::LogClass Sam::LogClass::_instance;

Sam::LogClass &Sam::LogClass::getInstance()
{
    return _instance;
}

std::string Sam::LogClass::levelName(int _level)
{
    switch (_level)
    {
    case DEBUG:
        return "debug";
    case INFO:
        return "info";
    case WARN:
        return "warn";
    case ERROR:
        return "error";
    default:
        return "unknown";
    }
};

const char *Sam::LogClass::levelColour(int _level)
{
    switch (_level)
    {
    case DEBUG:
        return "\033[36m";
    case INFO:
        return "";
    case WARN:
        return "\033[33m";
    case ERROR:
        return "\033[31m";
    default:
        return "";
    }
};

std::string Sam::LogClass::levelLogPrefix(int _level)
{
    std::string _levelName = levelName(_level);
    std::transform(_levelName.begin(), _levelName.end(), _levelName.begin(), ::toupper);
    std::string prefix = "[";
    prefix += _levelName;
    prefix += "]";
    while (prefix.size() < 8)
        prefix += " ";
    return prefix;
};

void Sam::LogClass::log(int _level, const char *message, const char *start, const char *end)
{
    if (_level >= level)
    {
        Serial.printf(
            "%s%s%s %s %s%s%s",
            colour ? levelColour(_level) : "",
            start,
            timestamp ? getTimestamp().c_str() : "",
            levelLogPrefix(_level).c_str(),
            message,
            end,
            "\033[00m");
        if (TelnetStream.available())
        {
            TelnetStream.printf(
                "%s%s%s %s %s%s%s",
                colour ? levelColour(_level) : "",
                start,
                timestamp ? getTimestamp().c_str() : "",
                levelLogPrefix(_level).c_str(),
                message,
                end,
                "\033[00m");
        }
    }
};

void Sam::LogClass::log(int _level, std::string message, const char *start, const char *end)
{
    log(_level, message.c_str(), start, end);
};

void Sam::LogClass::debug(const char *message, const char *start, const char *end)
{
    log(DEBUG, message, start, end);
};

void Sam::LogClass::debug(std::string message, const char *start, const char *end)
{
    log(DEBUG, message, start, end);
};

void Sam::LogClass::info(const char *message, const char *start, const char *end)
{
    log(INFO, message, start, end);
};

void Sam::LogClass::info(std::string message, const char *start, const char *end)
{
    log(INFO, message, start, end);
};

void Sam::LogClass::warn(const char *message, const char *start, const char *end)
{
    log(WARN, message, start, end);
};

void Sam::LogClass::warn(std::string message, const char *start, const char *end)
{
    log(WARN, message, start, end);
};

void Sam::LogClass::error(const char *message, const char *start, const char *end)
{
    log(ERROR, message, start, end);
};

void Sam::LogClass::error(std::string message, const char *start, const char *end)
{
    log(ERROR, message, start, end);
};

std::string Sam::LogClass::getTimestamp()
{
    time_t tm = time(nullptr);

    if (tm > 1616000000)
    {
        return TimeUtils.getIsoTimestamp();
    }

    return std::string("                   ");
};