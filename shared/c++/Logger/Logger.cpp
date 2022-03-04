#include "Logger.h"

std::string SamIoT::Logger::levelName(SamIoT::Logger::LogLevel _level)
{
    switch (_level)
    {
    case SamIoT::Logger::LogLevel::DEBUG:
        return "debug";
    case SamIoT::Logger::LogLevel::INFO:
        return "info";
    case SamIoT::Logger::LogLevel::WARN:
        return "warn";
    case SamIoT::Logger::LogLevel::ERROR:
        return "error";
    default:
        return "unknown";
    }
};

const char *SamIoT::Logger::levelColour(SamIoT::Logger::LogLevel _level)
{
    switch (_level)
    {
    case SamIoT::Logger::LogLevel::DEBUG:
        return "\033[36m";
    case SamIoT::Logger::LogLevel::INFO:
        return "";
    case SamIoT::Logger::LogLevel::WARN:
        return "\033[33m";
    case SamIoT::Logger::LogLevel::ERROR:
        return "\033[31m";
    default:
        return "";
    }
};

std::string SamIoT::Logger::levelLogPrefix(SamIoT::Logger::LogLevel _level)
{
    std::string _levelName = SamIoT::Logger::levelName(_level);
    std::transform(_levelName.begin(), _levelName.end(), _levelName.begin(), ::toupper);
    std::string prefix = "[";
    prefix += _levelName;
    prefix += "]";
    while (prefix.size() < 8)
        prefix += " ";
    return prefix;
};

void SamIoT::Logger::log(SamIoT::Logger::LogLevel _level, const char *message, const char *start, const char *end)
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

void SamIoT::Logger::log(SamIoT::Logger::LogLevel _level, std::string message, const char *start, const char *end)
{
    SamIoT::Logger::log(_level, message.c_str(), start, end);
};

void SamIoT::Logger::debug(const char *message, const char *start, const char *end)
{
    SamIoT::Logger::log(DEBUG, message, start, end);
};

void SamIoT::Logger::debug(std::string message, const char *start, const char *end)
{
    SamIoT::Logger::log(DEBUG, message, start, end);
};

void SamIoT::Logger::info(const char *message, const char *start, const char *end)
{
    SamIoT::Logger::log(INFO, message, start, end);
};

void SamIoT::Logger::info(std::string message, const char *start, const char *end)
{
    SamIoT::Logger::log(INFO, message, start, end);
};

void SamIoT::Logger::warn(const char *message, const char *start, const char *end)
{
    SamIoT::Logger::log(WARN, message, start, end);
};

void SamIoT::Logger::warn(std::string message, const char *start, const char *end)
{
    SamIoT::Logger::log(WARN, message, start, end);
};

void SamIoT::Logger::error(const char *message, const char *start, const char *end)
{
    SamIoT::Logger::log(ERROR, message, start, end);
};

void SamIoT::Logger::error(std::string message, const char *start, const char *end)
{
    SamIoT::Logger::log(ERROR, message, start, end);
};

std::string SamIoT::Logger::getTimestamp()
{
    time_t tm = time(nullptr);

    if (tm > 1616000000)
    {
        return SamIoT::Time::getIsoTimestamp();
    }

    return std::string("                    ");
};