#include <silica/EventGenerator.h>
#include <silica/Application.h>
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

