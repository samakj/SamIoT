#ifndef Log_h
#define Log_h

#include <Arduino.h>
#include <algorithm>
#include <string>
#include <TelnetStream.h>

#include <TimeUtils.h>

class LogClass 
{
    public:
        enum LOG_LEVEL { DEBUG, INFO, WARN, ERROR };
        int level = 0;
        bool timestamp = true;
        bool colour = true;
    
    private:
    
    public:
        LogClass (const LogClass&) = delete;
        static LogClass& getInstance();

        static std::string levelName(int _level);
        static const char* levelColour(int _level);
        static std::string levelLogPrefix(int _level);
        void log(int _level, const char* message, const char* start = "", const char* end = "\n");
        void log(int _level, std::string message, const char* start = "", const char* end = "\n");
        template<typename... Args>
        void logf(int _level, const char* format, Args... args);
        void debug(const char* message, const char* start = "", const char* end = "\n");
        void debug(std::string message, const char* start = "", const char* end = "\n");
        template<typename... Args>
        void debugf(const char* format, Args... args);
        void info(const char* message, const char* start = "", const char* end = "\n");
        void info(std::string message, const char* start = "", const char* end = "\n");
        template<typename... Args>
        void infof(const char* format, Args... args);
        void warn(const char* message, const char* start = "", const char* end = "\n");
        void warn(std::string message, const char* start = "", const char* end = "\n");
        template<typename... Args>
        void warnf(const char* format, Args... args);
        void error(const char* message, const char* start = "", const char* end = "\n");
        void error(std::string message, const char* start = "", const char* end = "\n");
        template<typename... Args>
        void errorf(const char* format, Args... args);
    
    private:
        LogClass() {};
        static LogClass _instance;

        static std::string getTimestamp();
};

#include <Log.tpp>

static LogClass& Log = LogClass::getInstance();

#endif