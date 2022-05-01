#include "Logger.h"

namespace SamIoT::Logger
{
    SamIoT::Logger::LogLevel level = SamIoT::Logger::LogLevel::DEBUG;
    bool timestamp = true;
    bool colour = true;

    std::string levelName(SamIoT::Logger::LogLevel _level)
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

    const char *levelColour(SamIoT::Logger::LogLevel _level)
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

    std::string levelLogPrefix(SamIoT::Logger::LogLevel _level)
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

    void log(SamIoT::Logger::LogLevel _level, const char *message, const char *start, const char *end)
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

    void log(SamIoT::Logger::LogLevel _level, std::string message, const char *start, const char *end)
    {
        SamIoT::Logger::log(_level, message.c_str(), start, end);
    };

    void debug(const char *message, const char *start, const char *end)
    {
        SamIoT::Logger::log(DEBUG, message, start, end);
    };

    void debug(std::string message, const char *start, const char *end)
    {
        SamIoT::Logger::log(DEBUG, message, start, end);
    };

    void info(const char *message, const char *start, const char *end)
    {
        SamIoT::Logger::log(INFO, message, start, end);
    };

    void info(std::string message, const char *start, const char *end)
    {
        SamIoT::Logger::log(INFO, message, start, end);
    };

    void warn(const char *message, const char *start, const char *end)
    {
        SamIoT::Logger::log(WARN, message, start, end);
    };

    void warn(std::string message, const char *start, const char *end)
    {
        SamIoT::Logger::log(WARN, message, start, end);
    };

    void error(const char *message, const char *start, const char *end)
    {
        SamIoT::Logger::log(ERROR, message, start, end);
    };

    void error(std::string message, const char *start, const char *end)
    {
        SamIoT::Logger::log(ERROR, message, start, end);
    };

    std::string getTimestamp()
    {
        time_t tm = time(nullptr);

        if (tm > 1616000000)
        {
            return SamIoT::Time::getIsoTimestamp();
        }

        return std::string("                    ");
    };
}