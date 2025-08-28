#include <kjut/CoarseTimer.h>
#include <kjut/Application.h>
#include <kjut/LoggingSystem.h>
namespace Kjut
{

CoarseTimer::CoarseTimer()
    : EventGenerator()
{

}

void CoarseTimer::visit()
{
    if( ! d.isRunning)
    {
        return;
    }
    MicroSeconds now = Application::instance()->microsecondsSinceStart();
    if( now >= d.nextTimeOut )
    {
        emit this->triggered();
        restart();
    }

}


void CoarseTimer::setTimeout(MilliSeconds timeout)
{
    d.periodTime = timeout;
}

void CoarseTimer::setType(Type type)
{
    d.type = type;
}

void CoarseTimer::restart()
{
    d.nextTimeOut = Application::instance()->microsecondsSinceStart() + d.periodTime;
}

void CoarseTimer::start()
{
    d.isRunning = true;
    restart();
}

void CoarseTimer::stop()
{
    d.isRunning = false;
}

}
