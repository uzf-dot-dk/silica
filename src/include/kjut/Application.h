#ifndef KJUT_APPLICATION_H
#define KJUT_APPLICATION_H

#include <stddef.h>
#include <kjut/Array.h>
#include <kjut/Macros.h>
#include <kjut/UnitsOfTime.h>
#include <kjut/SignalSlot.h>

#ifndef KJUT_EVENT_GENERATORS_HELD_BY_APPLICATION
#define KJUT_EVENT_GENERATORS_HELD_BY_APPLICATION 50
#endif

namespace Kjut
{

/** \brief The Application class is the runtime engine that makes a Kjut application run and ensures that events are handled and propagated.

\ingroup Core
\note There must exist a single Application instance, and the instance is retuired to exist for several Kjut classes to allow instantanion.

*/
class Application
{
    DISABLE_COPY(Application);
    DISABLE_MOVE(Application);

public:
    Application();

    virtual ~Application();

    /** \brief Runs the eventloop that drives the application.
     *
     *  Calling exec performs various setup tasks and then enters an infinite eventloop. To exit an eventloop do one of the following:
     *  - Call Application::quit(exitcode). This will exit the loop with the provided exit code, and continut execution after the exec() call.
     *  - Call the FATAL() macro. That will cause application execution to halt, and no statements after the exec() is executed. Exactely how, FATAL ensures this, is platform dependent.
     *  - Call Application::abort(exitcode). This causes a behaviour similar to calling the FATAL macro.
     *
     *  \returns The exitcode provided by Application::quit or Application::abort.
    */
    int exec();

    /** Returns the instance of Application or null if none has been instantiated yet.
     *  \returns Returns the instance of Application or null if none has been instantiated yet.
     */
    static Application * instance();

    /** Returns the numbers of microseconds that has passed since application start.
     *  \returns The numbers of microseconds that has passed since application start.
    \addtogroup PlatformRequiresImplementation.*/
    MicroSeconds microsecondsSinceStart() const;

    Slot<int> exit;

    /// \cond DEVELOPER_DOC
private:

    friend class EventGenerator;
    static Application* theApplicationInstance;
    void exitImplementation(int exitCode);

    struct
    {
        bool exitRequested = false;
        int providedExitCode = 0;
        Kjut::Array<class EventGenerator *, KJUT_EVENT_GENERATORS_HELD_BY_APPLICATION> eventGenerators;

    } d;

    /// \endcond
};

/// \cond DEVELOPER_DOC

/// \endcond

}

#endif // KJUT_APPLICATION_H
