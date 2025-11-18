#include "include/silica/LoggingSystem.h"
#include <new>
#include <cstddef>
#include <stdio.h>

void registerDefaultLogSink(Silica::LoggingSystem *);

alignas(Silica::LoggingSystem) char theInstanceData[sizeof(Silica::LoggingSystem)] = {};
Silica::LoggingSystem * theInstance = nullptr;

namespace Silica
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
