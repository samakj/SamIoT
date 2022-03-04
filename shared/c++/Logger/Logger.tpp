template <typename... Args>
void SamIoT::Logger::logf(SamIoT::Logger::LogLevel _level, const char *format, Args... args)
{
    size_t size = snprintf(nullptr, 0, format, args...);
    char buffer[size + 8];
    sprintf(buffer, format, args...);
    log(_level, buffer, "", "");
};

template <typename... Args>
void SamIoT::Logger::debugf(const char *format, Args... args)
{
    logf(SamIoT::Logger::LogLevel::DEBUG, format, args...);
};

template <typename... Args>
void SamIoT::Logger::infof(const char *format, Args... args)
{
    logf(SamIoT::Logger::LogLevel::INFO, format, args...);
};

template <typename... Args>
void SamIoT::Logger::warnf(const char *format, Args... args)
{
    logf(SamIoT::Logger::LogLevel::WARN, format, args...);
};

template <typename... Args>
void SamIoT::Logger::errorf(const char *format, Args... args)
{
    logf(SamIoT::Logger::LogLevel::ERROR, format, args...);
};