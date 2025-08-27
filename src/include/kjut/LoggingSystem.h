#ifndef KJUT_LOGGING_SYSTEM_H
#define KJUT_LOGGING_SYSTEM_H

#include <kjut/LogEntry.h>

#define LOG(format, ...) \
{ \
        Kjut::LogEntry le(__LINE__, __FILE__);          \
        le.setMessage(format, ##__VA_ARGS__);           \
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
