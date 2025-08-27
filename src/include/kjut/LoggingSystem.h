#ifndef KJUT_LOGGING_SYSTEM_H
#define KJUT_LOGGING_SYSTEM_H

#include <kjut/LogEntry.h>

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
