template <typename... Args>
void Sam::LogClass::logf(int _level, const char *format, Args... args)
{
    size_t size = snprintf(nullptr, 0, format, args...);
    char buffer[size + 8];
    sprintf(buffer, format, args...);
    log(_level, buffer, "", "");
};

template <typename... Args>
void Sam::LogClass::debugf(const char *format, Args... args)
{
    logf(DEBUG, format, args...);
};

template <typename... Args>
void Sam::LogClass::infof(const char *format, Args... args)
{
    logf(INFO, format, args...);
};

template <typename... Args>
void Sam::LogClass::warnf(const char *format, Args... args)
{
    logf(WARN, format, args...);
};

template <typename... Args>
void Sam::LogClass::errorf(const char *format, Args... args)
{
    logf(ERROR, format, args...);
};