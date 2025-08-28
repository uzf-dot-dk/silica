#include "include/kjut/LoggingSystem.h"
#include <new>
#include <cstddef>
#include <stdio.h>

void registerDefaultLogSink(Kjut::LoggingSystem *);

alignas(Kjut::LoggingSystem) char theInstanceData[sizeof(Kjut::LoggingSystem)] = {};
Kjut::LoggingSystem * theInstance = nullptr;

namespace Kjut
{

LoggingSystem::LoggingSystem()
{}

LoggingSystem::~LoggingSystem()
{}

LoggingSystem* LoggingSystem::instance()
{
    if ( ! theInstance )
    {
        theInstance = new (theInstanceData)LoggingSystem();
        registerDefaultLogSink(theInstance);
    }
    return theInstance;
}

void LoggingSystem::setSink(LogSink *sink)
{
    d.sink = sink;
}

void LoggingSystem::sinkEntry(const LogEntry &entry)
{
    if(d.sink)
    {
        d.sink->sinkEntry(entry);
    }
}


}
