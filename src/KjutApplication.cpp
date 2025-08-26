#include "include/kjut/Application.h"

class EmptyApplication : public Kjut::Application
{};


namespace Kjut
{

static const EmptyApplication * emptyInstance = new EmptyApplication;
Kjut::Application * theInstance = nullptr;



Application::Application()
{
    theInstance = this;
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



}
