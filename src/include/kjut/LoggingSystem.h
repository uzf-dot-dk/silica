#ifndef KJUT_LOGGING_SYSTEM_H
#define KJUT_LOGGING_SYSTEM_H

#include <kjut/LogEntry.h>

#define LOG(formatString, ...) \
{ \
        Kjut::LogEntry le(__LINE__, __FILE__);          \
        le.format(formatString, ##__VA_ARGS__);           \
        le.setType(Kjut::LogEntry::Type::Log); \
        Kjut::LoggingSystem::instance()->sinkEntry(le); \
}


#define WARN(formatString, ...) \
{ \
        Kjut::LogEntry le(__LINE__, __FILE__);          \
        le.format(formatString, ##__VA_ARGS__);           \
        le.setType(Kjut::LogEntry::Type::Warning); \
        Kjut::LoggingSystem::instance()->sinkEntry(le); \
}

#define FATAL(formatString, ...) \
{ \
        Kjut::LogEntry le(__LINE__, __FILE__);          \
        le.format(formatString, ##__VA_ARGS__);           \
        le.setType(Kjut::LogEntry::Type::Fatal); \
        Kjut::LoggingSystem::instance()->sinkEntry(le); \
}

namespace Kjut
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


#endif // KJUT_LOGGING_SYSTEM_H
