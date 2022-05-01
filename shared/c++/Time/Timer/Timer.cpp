#include "Timer.h"

namespace SamIoT::Time::Timer
{
    uint16_t reportPeriod = 60000;

    uint32_t loopCount = 0;
    unsigned long lastLoopReportMillis = -1;

    std::unordered_map<const char *, SamIoT::Time::Timer::TimerData> data = {};

    void TimerData::reset()
    {
        this->min = 65535;
        this->max = 0;
        this->total = 0;
        this->count = 0;
    };

    void TimerData::start_loop()
    {
        this->start = millis();
    };

    void TimerData::end_loop()
    {
        if (this->start)
        {
            this->add(SamIoT::Time::millisSince(this->start));
            this->start = 0;
        }
    };

    void TimerData::add(uint16_t value)
    {
        if (value < this->min)
            this->min = value;
        if (value > this->max)
            this->max = value;
        this->total += value;
        this->count += 1;
    };

    void start(const char *id)
    {
        if (!SamIoT::Time::Timer::data.count(id))
            SamIoT::Time::Timer::data.insert(
                std::make_pair(id, SamIoT::Time::Timer::TimerData()));
        SamIoT::Time::Timer::data[id].start_loop();
    };

    void end(const char *id)
    {
        if (!SamIoT::Time::Timer::data.count(id))
            SamIoT::Time::Timer::data.insert(
                std::make_pair(id, SamIoT::Time::Timer::TimerData()));
        SamIoT::Time::Timer::data[id].end_loop();
    };

    void loop()
    {
        if (SamIoT::Time::Timer::lastLoopReportMillis == -1)
            SamIoT::Time::Timer::lastLoopReportMillis = millis();
        if (
            SamIoT::Time::millisSince(
                SamIoT::Time::Timer::lastLoopReportMillis) >
            SamIoT::Time::Timer::reportPeriod)
        {
            float dt = SamIoT::Time::millisSince(SamIoT::Time::Timer::lastLoopReportMillis) / 1000.0f;
            SamIoT::Logger::debugf(
                "%d loops in last %.1fs => %.1f loops/s => %.3fms/loop\n",
                SamIoT::Time::Timer::loopCount,
                dt,
                SamIoT::Time::Timer::loopCount / dt,
                1000 * dt / SamIoT::Time::Timer::loopCount);

            SamIoT::Logger::debug("  | ID       | min (ms) | max (ms) | ave (ms) | % of total |");
            for (
                auto iterator = SamIoT::Time::Timer::data.begin();
                iterator != SamIoT::Time::Timer::data.end(); iterator++)
            {
                SamIoT::Logger::debugf(
                    "  | %8s | %8d | %8d | %8.1f | %9.2f%% |\n",
                    iterator->first,
                    iterator->second.min,
                    iterator->second.max,
                    (float)iterator->second.total / iterator->second.count,
                    .1 * (float)iterator->second.total / dt);
                iterator->second.reset();
            }

            SamIoT::Time::Timer::lastLoopReportMillis = millis();
            SamIoT::Time::Timer::loopCount = 0;
        }
        else
        {
            SamIoT::Time::Timer::loopCount += 1;
        }
    };
}