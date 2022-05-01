#ifndef _SamIoT_Logger_h
#define _SamIoT_Logger_h

#include <Arduino.h>
#include <TelnetStream.h>
#include <algorithm>
#include <string>

#include <Time.h>

namespace SamIoT::Logger
{
    enum LogLevel
    {
        DEBUG,
        INFO,
        WARN,
        ERROR
    };
    extern SamIoT::Logger::LogLevel level;
    extern bool timestamp;
    extern bool colour;

    std::string levelName(SamIoT::Logger::LogLevel _level);
    const char *levelColour(SamIoT::Logger::LogLevel _level);
    std::string levelLogPrefix(SamIoT::Logger::LogLevel _level);
    void log(SamIoT::Logger::LogLevel _level, const char *message, const char *start = "", const char *end = "\n");
    void log(SamIoT::Logger::LogLevel _level, std::string message, const char *start = "", const char *end = "\n");
    template <typename... Args>
    void logf(SamIoT::Logger::LogLevel _level, const char *format, Args... args);
    void debug(const char *message, const char *start = "", const char *end = "\n");
    void debug(std::string message, const char *start = "", const char *end = "\n");
    template <typename... Args>
    void debugf(const char *format, Args... args);
    void info(const char *message, const char *start = "", const char *end = "\n");
    void info(std::string message, const char *start = "", const char *end = "\n");
    template <typename... Args>
    void infof(const char *format, Args... args);
    void warn(const char *message, const char *start = "", const char *end = "\n");
    void warn(std::string message, const char *start = "", const char *end = "\n");
    template <typename... Args>
    void warnf(const char *format, Args... args);
    void error(const char *message, const char *start = "", const char *end = "\n");
    void error(std::string message, const char *start = "", const char *end = "\n");
    template <typename... Args>
    void errorf(const char *format, Args... args);

    std::string getTimestamp();
};

#include "Logger.tpp"

#endif