#ifndef SILICA_LOGGING_SYSTEM_H
#define SILICA_LOGGING_SYSTEM_H

#include <silica/LogEntry.h>

#define LOG(formatString, ...) \
{ \
        Silica::LogEntry le(__LINE__, __FILE__);          \
        le.format(formatString, ##__VA_ARGS__);           \
        le.setType(Silica::LogEntry::Type::Log); \
        Silica::LoggingSystem::instance()->sinkEntry(le); \
}


#define WARN(formatString, ...) \
{ \
        Silica::LogEntry le(__LINE__, __FILE__);          \
        le.format(formatString, ##__VA_ARGS__);           \
        le.setType(Silica::LogEntry::Type::Warning); \
        Silica::LoggingSystem::instance()->sinkEntry(le); \
}

#define FATAL(formatString, ...) \
{ \
        Silica::LogEntry le(__LINE__, __FILE__);          \
        le.format(formatString, ##__VA_ARGS__);           \
        le.setType(Silica::LogEntry::Type::Fatal); \
        Silica::LoggingSystem::instance()->sinkEntry(le); \
}

namespace Silica
{

class LogSink
{
public:
    virtual void sinkEntry(const LogEntry &entry) = 0;
};


class LoggingSystem
{

public:
    static LoggingSystem* instance();
    void setSink(LogSink *sink);
    void sinkEntry(const LogEntry &entry);

private:
    LoggingSystem();
    ~LoggingSystem();
    struct
    {
        LogSink *sink = nullptr;
    } d;


};


}


#endif // SILICA_LOGGING_SYSTEM_H
