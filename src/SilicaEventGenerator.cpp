#include <silica/EventGenerator.h>
#include <silica/Application.h>
namespace Silica
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

