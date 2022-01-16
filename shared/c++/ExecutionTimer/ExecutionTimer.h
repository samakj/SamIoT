#ifndef Execution_Timer_h
#define Execution_Timer_h

#include <Arduino.h>
#include <limits>
#include <string>
#include <unordered_map>

#include <Log.h>
#include <TimeUtils.h>

class ExecutionTimeData 
{
    public:
        int min = -1;
        int max = -1;
        int total = 0;
        int count = 0;
    
    private:
        unsigned long _start = 0;

    public:
        void reset();
        void start();
        void end();
        void add(int value);
};

class ExecutionTimerClass {
    public:
        int reportPeriod = 60;

    private:
        int _loopCount = 0;
        int _lastLoopReportMillis = -1;
        std::unordered_map<const char*, ExecutionTimeData> _data = {};

    public:
        ExecutionTimerClass (const ExecutionTimerClass&) = delete;
        static ExecutionTimerClass& getInstance();

        void start(const char* id);
        void end(const char* id);
        void loop();
    
    private:
        ExecutionTimerClass() {};
        static ExecutionTimerClass _instance;
};

static ExecutionTimerClass& ExecutionTimer = ExecutionTimerClass::getInstance();

#endif