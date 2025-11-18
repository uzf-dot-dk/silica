#ifndef SILICA_COARSE_TIMER_H
#define SILICA_COARSE_TIMER_H

#include <silica/EventGenerator.h>
#include <silica/UnitsOfTime.h>
#include <silica/SignalSlot.h>

namespace Silica
{

/** \brief Implements a coarse timer that is normally good enough for humans, but not for machine control.

The CoarseTimer implements a timer that has its resolution in milliseconds and whigh may be significanly off and have triggers skipped if the main eventloop is held busy for longer periods of time.
The CoarseTimer should not be used for timing critical issues, but is under normal circumstances good for logging, indicator lamps and watchdogs.

An example could be:
```cpp
void doStuff()
{
    LOG("Does stuff");
}

int main(int argc, char *argv[])
{
    Silica::Application app;

    Silica::CoarseTimer everySecond;
    everySecond.triggered.connectTo(doStuff);
    everySecond.setTimeout(1000);
    everySecond.start();

    return app.exec();
}
```

\ingroup Core

*/
class CoarseTimer : public EventGenerator
{
public:

    /** Used to specify the type of timer invocation*/
    enum class Type
    {
        /** Causes a single timout triggering, and then the Coarsetimer is stopped.*/
        SingleShot,
        /** Causes a repeated timout triggering infinetly. This is the default state of a CoarseTimer.*/
        Repeated
    };

    /** \brief Creates a new CoarseTimer with a timeout of one second and repeated triggering. */
    CoarseTimer();

    /** \brief Sets the timeout period of this CoarseTimer.
    \param timeout The new period of this CoarseTimer.    */
    void setTimeout(MilliSeconds timeout);

    /** \brief Sets the repetition type of this CoarseTimer.
    \param type The new repetition type of this CoarseTimer. */
    void setType(Type type);

    /** \brief Restarts this timer, so that the next trigger happens after a full period. */
    void restart();

    /** \brief Starts this timer, so that the next timeout will be after a full period.*/
    void start();

    /** \brief Stops this timer, preventing any further timeouts. */
    void stop();

    /** \brief Signal emitted, when a timeout occurs. */
    Signal<> triggered;

    /// \cond DEVELOPER_DOC
    void visit() override;
private:

    struct {
        bool isRunning = false;
        Type type = Type::Repeated;
        MicroSeconds nextTimeOut{0};
        MicroSeconds periodTime{1000};

    } d;
    /// \endcond
};


}

#endif // SILICA_COARSE_TIMER_H
