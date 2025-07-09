#ifndef SIGNAL_SLOTS_H
#define SIGNAL_SLOTS_H

#include <kjut/Array.h>

#ifndef MAX_NUMBER_OF_CONNECTIONS_PER_SIGNAL_OR_SLOT
#define MAX_NUMBER_OF_CONNECTIONS_PER_SIGNAL_OR_SLOT 10
#endif

#include <functional>

#define KJUT_ENABLE_LAMBDAS_IN_SIGNAL_SLOTS
namespace Kjut
{



template <typename ...Ts> class Signal;
template <typename ...Ts> class Slot;
template <typename ...Ts> class Connection;

template <typename ...Ts> class Connection
{
public:
    Connection();
private:
    friend class Signal<Ts...>;
    friend class Slot<Ts...>;

    Connection(Signal<Ts...> *source, Slot<Ts...> *slotDestination);
    Connection(Signal<Ts...> *source, Signal<Ts...> *signalDestination);
#ifdef KJUT_ENABLE_LAMBDAS_IN_SIGNAL_SLOTS
    //Connection(Signal<Ts...> *source, std::function<void(Ts...)> *functionObjectDestination);
#endif
    void distributeInvocation(Ts... parameters);

    union
    {
#ifdef KJUT_ENABLE_LAMBDAS_IN_SIGNAL_SLOTS
        //std::function<void(Ts...)> *functionObject;
#endif
        Slot<Ts...> *slot;
        Signal<Ts...> *signal;
    } destinations;

    enum class DestinationType
    {
        None = 0,
        Signal,
        Slot,
#ifdef KJUT_ENABLE_LAMBDAS_IN_SIGNAL_SLOTS
        FunctionObject
#endif
    };

    DestinationType destinationType;

    Signal<Ts...> *source;

};


template <typename ...Ts> class Slot
{

public:
    Slot();
    Slot(std::function<void(Ts...)>);
    Slot(void (*freeFloatingFunction)(Ts...));
    void invoke(Ts... parameters);

private:
    void (*freeFloatingFunction)(Ts...);
    std::function<void(Ts...)> functionObject;

    friend class Signal<Ts...>;
    Array<Signal<Ts...>*, MAX_NUMBER_OF_CONNECTIONS_PER_SIGNAL_OR_SLOT> sources;
};









template <typename ...Ts> class Signal
{

public:
    void operator()(Ts... parameters);

    bool connectTo(Slot<Ts...> *target);
    bool connectTo(Signal<Ts...> *target);
private:
    friend class Slot<Ts...>;
    Array<Connection<Ts...>, MAX_NUMBER_OF_CONNECTIONS_PER_SIGNAL_OR_SLOT> connections;
};

}








/// ------------------------------------------------------------------------------------------
/// Signal method definitions
/// ------------------------------------------------------------------------------------------

template <typename ...Ts>  void Kjut::Signal<Ts...>::operator()(Ts... parameters)
{
    for(auto &connection : connections)
    {
        connection.distributeInvocation(parameters...);
    }
}

template <typename ...Ts> bool Kjut::Signal<Ts...>::connectTo(Slot<Ts...> *target)
{
    Connection<Ts...> connection(this, target);
    this->connections.append(connection);
    return false;
}

template <typename ...Ts> bool Kjut::Signal<Ts...>::connectTo(Signal<Ts...> *target)
{
    Connection<Ts...> connection(this, target);
    this->connections.append(connection);
    return false;
}




/// ------------------------------------------------------------------------------------------
/// Slot method definitions
/// ------------------------------------------------------------------------------------------

template <typename ...Ts> Kjut::Slot<Ts...>::Slot()
{
    freeFloatingFunction = nullptr;
    functionObject  = nullptr;
}

template <typename ...Ts> Kjut::Slot<Ts...>::Slot(void (*freeFloatingFunction)(Ts...))
{
    this->freeFloatingFunction = freeFloatingFunction;
    this->functionObject  = nullptr;
}


template <typename ...Ts> Kjut::Slot<Ts...>::Slot(std::function<void(Ts...)> functor)
{
    this->freeFloatingFunction = nullptr;
    this->functionObject  = functor;

}

template <typename ...Ts> void Kjut::Slot<Ts...>::invoke(Ts... parameters)
{
    if(freeFloatingFunction)
    {
        freeFloatingFunction(parameters...);
    }
    else if(functionObject)
    {
        functionObject(parameters...);
    }
}

/// ------------------------------------------------------------------------------------------
/// Connection method definitions
/// ------------------------------------------------------------------------------------------

template <typename... Ts>
Kjut::Connection<Ts...>::Connection()
{
    this->source = nullptr;
    memset(&this->destinations, 0, sizeof(this->destinations));
    this->destinationType = DestinationType::None;
}

template <typename... Ts>
Kjut::Connection<Ts...>::Connection(Signal<Ts...>* source, Slot<Ts...>* slotDestination)
{
    this->source = source;
    this->destinations.slot = slotDestination;
    this->destinationType = DestinationType::Slot;
}

template <typename... Ts>
Kjut::Connection<Ts...>::Connection(Signal<Ts...>* source, Signal<Ts...>* signalDestination)
{
    this->source = source;
    this->destinations.signal = signalDestination;
    this->destinationType = DestinationType::Signal;
}

template <typename... Ts>
void Kjut::Connection<Ts...>::distributeInvocation(Ts... parameters)
{
    switch(this->destinationType)
    {
    case DestinationType::Signal:
        (*this->destinations.signal)(parameters...);
        break;
    case DestinationType::Slot:
        this->destinations.slot->invoke(parameters...);
        break;

    case DestinationType::None:
        break;

    }
}


#endif // SIGNAL_SLOTS_H
