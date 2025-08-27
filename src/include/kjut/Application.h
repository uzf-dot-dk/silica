#ifndef KJUT_APPLICATION_H
#define KJUT_APPLICATION_H

#include <stdio.h>




namespace Kjut
{

class LogEntrySink
{
public:
    virtual void sink(const class LogEntry &) = 0;
};

class Application
{

public:
    Application();
    virtual ~Application() = default;

    static Application * instance();
    void assertInstanceExists(const char *messageIfFailed );
    void registerLogSink(LogEntrySink *sink);
    void sink(const class LogEntry &logEntry);


private:
    struct
    {
        LogEntrySink *logEntrySink ;
    } d;
};


}

#endif // KJUT_APPLICATION_H
