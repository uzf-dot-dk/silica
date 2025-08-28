#ifndef SIGNAL_SLOTS_H
#define SIGNAL_SLOTS_H

#include <kjut/Array.h>
#include <kjut/Set.h>

#ifndef MAX_NUMBER_OF_CONNECTIONS_PER_SIGNAL_OR_SLOT
#define MAX_NUMBER_OF_CONNECTIONS_PER_SIGNAL_OR_SLOT 10
#endif

#include <functional>

#define KJUT_ENABLE_LAMBDAS_IN_SIGNAL_SLOTS
#define emit

namespace Kjut
{

template <typename ...Ts> class Signal;
template <typename ...Ts> class Slot;

template <typename ...Ts> class Connection
{
public:
    enum class Type
    {
        Direct,
        Queued,
        Auto,
    };

    Connection(Connection::Type type = Connection::Type::Auto);
    bool isTarget(const Slot<Ts...> *slotDestination) const;


private:
    friend class Signal<Ts...>;
    friend class Slot<Ts...>;

    Connection(Signal<Ts...> *source, Slot<Ts...> *slotDestination, Connection::Type type = Connection::Type::Auto);
    Connection(Signal<Ts...> *source, Signal<Ts...> *signalDestination, Connection::Type type = Connection::Type::Auto);
    Connection(Signal<Ts...> *source, void (*functionPointerDestination)(Ts...), Connection::Type type = Connection::Type::Auto);
#ifdef KJUT_ENABLE_LAMBDAS_IN_SIGNAL_SLOTS
    //Connection(Signal<Ts...> *source, std::function<void(Ts...)> *functionObjectDestination, Connection::Type type = Connection::Type::Auto);
#endif
    void distributeInvocation(Ts... parameters);


    enum class DestinationType
    {
        None = 0,
        Signal,
        Slot,
        FunctionPointer,
#ifdef KJUT_ENABLE_LAMBDAS_IN_SIGNAL_SLOTS
        FunctionObject
#endif
    };

    struct
    {
        union
        {
    #ifdef KJUT_ENABLE_LAMBDAS_IN_SIGNAL_SLOTS
            //std::function<void(Ts...)> *functionObject;
    #endif
            void (*functionPointer)(Ts...);
            Slot<Ts...> *slot;
            Signal<Ts...> *signal;
        } destinations;


        DestinationType destinationType;
        Connection::Type connectionType;
        Signal<Ts...> *source;
        std::tuple<Ts...> queuedParameters;
    } d;


};


/** \brief Class Slot implements the observer in Kjut's implementation of the Observer pattern.
 *
 *  \see [Signal and Slots](\ref signal_and_slots)
 *
 *  \ingroup Core
 */
template <typename ...Ts> class Slot
{

public:
    /** \brief Creates a new slot connected to nothing and invoking nothing.*/
    Slot();

/** \brief Creates a new Slot<Ts...> connected to nothing and invoking the \p functionObject when invoked.

Example:

```cpp
class ClassWithSlot
{

public:

    ClassWithSlot()
        : printNewInt(std::bind(&MySlotOwner::doPrintNextImpl, this, std::placeholders::_1))
    {
    }

    void doPrintNextImpl(int i)
    {
        std::cout << name << " got invoked with " << i << std::endl;
    }

    Slot<int> printNewInt;
    std::string name;
};

...
ClassWithSlot cws;
cws.name = "SomeName";

Slot<int> newInt;
newInt.connectTo(cws.printNewInt);
emit newInt(117);   // Prints "SomeName got invoked with 117"
```

\param functionObject The function object to call, when invoked.
*/
    Slot(std::function<void(Ts...)> functionObject);

    /**
\brief Creates a new Slot<Ts...> connected to nothing and invoking the \p freeFloatingFunction when invoked.

Example:

```cpp
void doStuff(int, double)
{
   std::cout << int << " " << double << std::endl;
}
...
Slot<int, double> doStuffWithIntAnddoubles(doStuff);
Signal<int, double> newValues;
newValues.connectTo(&doStuffWithIntAnddoubles);
emit newValues(42, 3.1415);
// prints "42 3.1415"
```

\param functionObject The function object to call, when invoked.
    */
    Slot(void (*freeFloatingFunction)(Ts...));

/** \brief Deletes this slot and breaks connections it may have to any [Signals](\ref Signal).*/
    ~Slot();
    void operator()(Ts... parameters);
    void invoke(Ts... parameters);

/// @cond DEVELOPER_DOCUMENTATION
private:
    friend class Signal<Ts...>;
    struct
    {
        void (*freeFloatingFunction)(Ts...);
        std::function<void(Ts...)> functionObject;

        Set<Signal<Ts...>*, MAX_NUMBER_OF_CONNECTIONS_PER_SIGNAL_OR_SLOT> sources;
    } d;
/// @endcond
};








/** \brief Class Signal implements the subject in Kjut's implementation of the Observer pattern.
 *
 *  \see \ref SignalAndSlots
 *
 *  \ingroup Core
 */
template <typename ...Ts> class Signal
{

public:
    void operator()(Ts... parameters);

    bool connectTo(Slot<Ts...> *target);
    bool connectTo(Signal<Ts...> *target);
    bool connectTo(void(*target)(Ts...));
private:
    friend class Slot<Ts...>;

    void removeTarget(Slot<Ts...> *target);
    struct
    {
        Array<Connection<Ts...>, MAX_NUMBER_OF_CONNECTIONS_PER_SIGNAL_OR_SLOT> connections;
    } d;
};

}








/// ------------------------------------------------------------------------------------------
/// Signal method definitions
/// ------------------------------------------------------------------------------------------

template <typename ...Ts>  void Kjut::Signal<Ts...>::operator()(Ts... parameters)
{
    for(auto &connection : d.connections)
    {
        connection.distributeInvocation(parameters...);
    }
}

template <typename ...Ts> bool Kjut::Signal<Ts...>::connectTo(Slot<Ts...> *target)
{
    Connection<Ts...> connection(this, target);
    this->d.connections.append(connection);
    target->d.sources.insert(this);
    return false;
}

template <typename ...Ts> bool Kjut::Signal<Ts...>::connectTo(Signal<Ts...> *target)
{
    Connection<Ts...> connection(this, target);
    this->d.connections.append(connection);
    return false;
}

template <typename ...Ts> bool Kjut::Signal<Ts...>::connectTo(void(*target)(Ts...))
{
    Connection<Ts...> connection(this, target);
    this->d.connections.append(connection);
    return false;
}


template <typename ...Ts> void Kjut::Signal<Ts...>::removeTarget(Slot<Ts...> *target)
{
    for(size_t i = 0 ; i < d.connections.size(); i++)
    {
        if(d.connections[i].isTarget(target))
        {
            d.connections.remove(i);
        }
    }
}




/// ------------------------------------------------------------------------------------------
/// Slot method definitions
/// ------------------------------------------------------------------------------------------

template <typename ...Ts> Kjut::Slot<Ts...>::Slot()
{
    d.freeFloatingFunction = nullptr;
    d.functionObject  = nullptr;
}

template <typename ...Ts> Kjut::Slot<Ts...>::~Slot()
{
    for(auto source : d.sources.values())
    {
        source->removeTarget(this);
    }

}

template <typename ...Ts> Kjut::Slot<Ts...>::Slot(void (*freeFloatingFunction)(Ts...))
{
    this->d.freeFloatingFunction = freeFloatingFunction;
    this->d.functionObject  = nullptr;
}


template <typename ...Ts> Kjut::Slot<Ts...>::Slot(std::function<void(Ts...)> functor)
{
    this->d.freeFloatingFunction = nullptr;
    this->d.functionObject  = functor;

}

template <typename ...Ts> void Kjut::Slot<Ts...>::operator()(Ts... parameters)
{
    this->invoke(parameters...);
}

template <typename ...Ts> void Kjut::Slot<Ts...>::invoke(Ts... parameters)
{
    if(d.freeFloatingFunction)
    {
        d.freeFloatingFunction(parameters...);
    }
    else if(d.functionObject)
    {
        d.functionObject(parameters...);
    }
}

/// ------------------------------------------------------------------------------------------
/// Connection method definitions
/// ------------------------------------------------------------------------------------------

template <typename... Ts>
Kjut::Connection<Ts...>::Connection(Connection::Type connectionType)
{
    if(connectionType == Connection::Type::Auto)
    {
        d.connectionType = Connection::Type::Direct;
    }
    else
    {
        d.connectionType = connectionType;
    }
    this->d.source = nullptr;
    memset(&this->d.destinations, 0, sizeof(this->d.destinations));
    this->d.destinationType = DestinationType::None;
}

template <typename... Ts>
Kjut::Connection<Ts...>::Connection(Signal<Ts...>* source, Slot<Ts...>* slotDestination, Connection::Type type)
    : Connection(type)
{
    this->d.source = source;
    this->d.destinations.slot = slotDestination;
    this->d.destinationType = DestinationType::Slot;
}

template <typename... Ts>
Kjut::Connection<Ts...>::Connection(Signal<Ts...>* source, Signal<Ts...>* signalDestination, Connection::Type type)
    : Connection(type)
{
    this->d.source = source;
    this->d.destinations.signal = signalDestination;
    this->d.destinationType = DestinationType::Signal;
}


template <typename... Ts>
Kjut::Connection<Ts...>::Connection(Signal<Ts...>* source, void (*functionPointerDestination)(Ts...), Connection::Type type)
    : Connection(type)
{
    this->d.source = source;
    this->d.destinations.functionPointer = functionPointerDestination;
    this->d.destinationType = DestinationType::FunctionPointer;
}

template <typename... Ts>
void Kjut::Connection<Ts...>::distributeInvocation(Ts... parameters)
{
    if(d.connectionType == Connection::Type::Direct)
    {

        switch(this->d.destinationType)
        {
        case DestinationType::Signal:
            (*this->d.destinations.signal)(parameters...);
            break;
        case DestinationType::Slot:
            this->d.destinations.slot->invoke(parameters...);
            break;
        case DestinationType::FunctionPointer:
            this->d.destinations.functionPointer(parameters...);
            break;
#ifdef KJUT_ENABLE_LAMBDAS_IN_SIGNAL_SLOTS
        case DestinationType::FunctionObject:
            break;
#endif


        case DestinationType::None:
            break;
        }
    }
    // else if(d.connectionType == Connection::Type::Queued)
    // {
        // d.queuedParameters = std::make_tuple(parameters...);
        //
        // and then later do
        //
        // switch(this->d.destinationType)
        // {
        // case DestinationType::Signal:
        //     std::apply(*this->d.destinations.signal, d.queuedParameters);
        //     break;
        // case DestinationType::Slot:
        //     std::apply(*this->d.destinations.slot, d.queuedParameters);
        //     this->d.destinations.slot->invoke(parameters...);
        //     break;

        // case DestinationType::None:
        //     break;
        // }
    // }
}

template <typename ...Ts>
bool Kjut::Connection<Ts...>::isTarget(const Slot<Ts...> *slotDestination) const
{
    if(d.destinationType != DestinationType::Slot)
    {
        return false;
    }
    if(d.destinations.slot == slotDestination)
    {
        return true;
    }
    return false;
}


#endif // SIGNAL_SLOTS_H
