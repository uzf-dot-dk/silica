#ifndef SILICA_EVENT_GENERATOR_H
#define SILICA_EVENT_GENERATOR_H

namespace Silica
{

/**

\brief The EventGenerator is one of the basic building blocks in the event driven implementation of the Silica framework.

An EventGenerator is meant to be subclassed to classes that respond to external events such as clock, serial ports, input devices, e.t.c.

\ingroup Core

*/
class EventGenerator
{
public:
    /** \brief Constructs and registers a new EventGenerator.

    The constructor registers this EventGenerator in the event system, so when subclassing an EventGenerator, it is important to call this constructor from the extending class.
    */
    EventGenerator();

    virtual ~EventGenerator();

private:
    /// \cond DEVELOPER_DOC
    friend class Application;
    /// \endcond

    /** \brief Is called periodically from the main event loop to allow the EventGenerator to process data and emit signals if appropriate.

    visit() is where you should implement your logic to work on data originating from, e.g. interrupts or file descriptors.
    */
    virtual void visit() = 0;
};

}









#endif // SILICA_EVENT_GENERATOR_H

