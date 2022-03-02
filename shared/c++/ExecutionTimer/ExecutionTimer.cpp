#include <ExecutionTimer.h>

void ExecutionTimeData::reset()
{
    min = -1;
    max = -1;
    total = 0;
    count = 0;
};

void ExecutionTimeData::start()
{
    _start = millis();
};

void ExecutionTimeData::end()
{
    if (_start)
    {
        add(TimeUtils.millisSince(_start));
        _start = 0;
    }
};

void ExecutionTimeData::add(int value)
{
    if (min == -1 || value < min)
        min = value;
    if (max == -1 || value > max)
        max = value;
    total += value;
    count += 1;
};

ExecutionTimerClass ExecutionTimerClass::_instance;

ExecutionTimerClass &ExecutionTimerClass::getInstance()
{
    return _instance;
}

void ExecutionTimerClass::start(const char *id)
{
    if (!_data.count(id))
        _data.insert(std::make_pair(id, ExecutionTimeData()));
    _data[id].start();
};

void ExecutionTimerClass::end(const char *id)
{
    if (!_data.count(id))
        _data.insert(std::make_pair(id, ExecutionTimeData()));
    _data[id].end();
};

void ExecutionTimerClass::loop()
{
    if (_lastLoopReportMillis == -1)
        _lastLoopReportMillis = millis();
    if (TimeUtils.millisSince(_lastLoopReportMillis) / 1000.0f > reportPeriod)
    {
        float dt = TimeUtils.millisSince(_lastLoopReportMillis) / 1000.0f;
        Sam::Log.debugf(
            "%d loops in last %.1fs => %.1f loops/s => %.3fms/loop\n",
            _loopCount,
            dt,
            _loopCount / dt,
            1000 * dt / _loopCount);

        Sam::Log.debug("  | ID       | min (ms) | max (ms) | ave (ms) | % of total |");
        for (auto iterator = _data.begin(); iterator != _data.end(); iterator++)
        {
            Sam::Log.debugf(
                "  | %8s | %8d | %8d | %8.1f | %9.2f%% |\n",
                iterator->first,
                iterator->second.min,
                iterator->second.max,
                (float)iterator->second.total / iterator->second.count,
                .1 * (float)iterator->second.total / dt);
            iterator->second.reset();
        }

        _lastLoopReportMillis = millis();
        _loopCount = 0;
    }
    else
    {
        _loopCount += 1;
    }
};