#ifndef KJUT_EVENT_GENERATOR_H
#define KJUT_EVENT_GENERATOR_H

namespace Kjut
{

class EventGenerator
{
public:
    EventGenerator();
    virtual ~EventGenerator();

private:
    friend class Application;
    virtual void visit() = 0;
};

}









#endif // KJUT_EVENT_GENERATOR_H

