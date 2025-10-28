#include "include/kjut/Application.h"
#include <kjut/EventGenerator.h>
#include <kjut/LoggingSystem.h>


static_assert(KJUT_EVENT_GENERATORS_HELD_BY_APPLICATION >= 0, "KJUT_EVENT_GENERATORS_HELD_BY_APPLICATION must be a non negative integer. Use 0 for infinite and growing capacity.");

namespace Kjut {

Application* Application::theApplicationInstance = nullptr;

Application::Application()
    : exit(std::bind(&Application::exitImplementation, this, std::placeholders::_1))
{
    printf("Application()::theApplicationInstance  = %p\n", Application::theApplicationInstance ); fflush(stdout);
    if ( Application::theApplicationInstance )
    {
        FATAL("Only a single Application instance may exist.");
    }
    platformSpecificInitialization();
    Application::theApplicationInstance = this;

}


Application::~Application()
{
    printf("~Application()::theApplicationInstance  = %p\n", Application::theApplicationInstance ); fflush(stdout);
    Application::theApplicationInstance = nullptr;
    printf("~Application()::theApplicationInstance  = %p\n", Application::theApplicationInstance ); fflush(stdout);
}

int Application::exec()
{
    while( ! d.exitRequested )
    {
        for(auto eventGenerator : d.eventGenerators)
        {
            eventGenerator->visit();
        }
    }
    return d.providedExitCode;
}


void Application::exitImplementation(int exitCode)
{
    d.exitRequested  = true;
    d.providedExitCode = exitCode;
}

Application * Application::instance()
{
    if( ! theApplicationInstance )
    {
        FATAL("No Application has been instantated. Ensure you instantiate a such, becore asking for the intance.");
    }
    return theApplicationInstance;
}



}
