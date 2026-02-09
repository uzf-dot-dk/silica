#include <silica/CoarseTimer.h>
#include <silica/Application.h>
#include <silica/LoggingSystem.h>
namespace Silica
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
        if(d.type == Silica::CoarseTimer::Type::Repeated)
        {
            restart();
        }
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

bool CoarseTimer::isRunning() const
{
    return d.isRunning;
}

}
