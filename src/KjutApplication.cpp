#include "include/kjut/Application.h"

#include <kjut/LogEntry.h>

#if defined(KJUT_OS_WINDOWS) || defined(KJUT_OS_LINUX)

class PrintfLogSink : public Kjut::LogEntrySink
{
public:
    void sink(const Kjut::LogEntry &entry) override
    {
        printf("#%4zu : %-*s : %s\n", entry.originatingLine(), KJUT_LOGENTRY_FILENAME_MAX_LENGTH, entry.originatingFile(), entry.message());
    };
};

#endif

class EmptyApplication : public Kjut::Application
{
public:

#if defined(KJUT_OS_WINDOWS) || defined(KJUT_OS_LINUX)
    EmptyApplication()
    {
        registerLogSink(&sink);
    }
    PrintfLogSink sink;
#else
    EmptyApplication() { }
#endif
};


static const EmptyApplication * emptyInstance = new EmptyApplication;
Kjut::Application * theInstance = nullptr;

namespace Kjut
{








Application::Application()
{
    theInstance = this;
    this->d.logEntrySink = nullptr;
}

Application * Application::instance()
{
    if(theInstance)
    {
        return theInstance;
    }
    return const_cast<EmptyApplication*>(emptyInstance);
}

void Application::assertInstanceExists(const char *messageIfFailed)
{
    if ( ! theInstance )
    {
        while(1) { }
    }

}

void Application::registerLogSink(LogEntrySink *sink)
{
    d.logEntrySink = sink;
}

void Application::sink(const LogEntry &logEntry)
{
    if( this->d.logEntrySink )
    {
        this->d.logEntrySink->sink(logEntry);
    }

}



}
