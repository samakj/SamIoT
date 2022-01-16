template <typename... Args>
void LogClass::logf(int _level, const char* format, Args... args)
{
    size_t size = snprintf(nullptr, 0, format, args...);
    char buffer[size + 8];
    sprintf(buffer, format, args...);
    log(_level, buffer, "", "");
};


template <typename... Args>
void LogClass::debugf(const char* format, Args... args)
{
    logf(DEBUG, format, args...);
};


template <typename... Args>
void LogClass::infof(const char* format, Args... args)
{
    logf(INFO, format, args...);
};


template<typename... Args>
void LogClass::warnf(const char* format, Args... args)
{
    logf(WARN, format, args...);
};


template <typename... Args>
void LogClass::errorf(const char* format, Args... args)
{
    logf(ERROR, format, args...);
};