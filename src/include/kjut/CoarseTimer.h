#ifndef KJUT_COARSE_TIMER_H
#define KJUT_COARSE_TIMER_H

#include <kjut/EventGenerator.h>
#include <kjut/UnitsOfTime.h>
#include <kjut/SignalSlot.h>

namespace Kjut
{

class CoarseTimer : public EventGenerator
{
public:

    enum class Type
    {
        SingleShot,
        Repeated
    };

    CoarseTimer();
    void visit() override;

    void setTimeout(MilliSeconds timeout);
    void setType(Type type);
    void restart();
    void start();
    void stop();

    Signal<> triggered;

private:

    struct {
        bool isRunning = false;
        Type type = Type::Repeated;
        MicroSeconds nextTimeOut{0};
        MicroSeconds periodTime{0};

    } d;
};


}

#endif // KJUT_COARSE_TIMER_H
