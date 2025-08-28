#ifndef KJUT_APPLICATION_H
#define KJUT_APPLICATION_H

#include <stddef.h>
#include <kjut/Array.h>
#include <stdint.h>
#include <kjut/UnitsOfTime.h>
#ifndef KJUT_EVENT_GENERATORS_HELD_BY_APPLICATION
#define KJUT_EVENT_GENERATORS_HELD_BY_APPLICATION 50
#endif





namespace Kjut
{


class Application
{

public:
    Application();
    virtual ~Application() = default;

    int exec();

    static Application * instance();
    MicroSeconds microsecondsSinceStart() const;

private:

    friend class EventGenerator;

    struct
    {
        bool exitRequested = false;
        Kjut::Array<class EventGenerator *, KJUT_EVENT_GENERATORS_HELD_BY_APPLICATION> eventGenerators;

    } d;

};


}

#endif // KJUT_APPLICATION_H
