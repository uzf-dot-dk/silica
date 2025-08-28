#include <kjut/EventGenerator.h>
#include <kjut/Application.h>
namespace Kjut
{

EventGenerator::EventGenerator()
{
    Application::instance()->d.eventGenerators.append(this);
}

EventGenerator::~EventGenerator()
{
//    Application::instance()->d.eventGenerators.remove(this);
}


}

