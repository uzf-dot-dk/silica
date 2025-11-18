#ifndef KJUT_SCOPE_GUARD_H
#define KJUT_SCOPE_GUARD_H

#include <functional>
namespace Kjut
{

class ScopeGuard
{
public:

    ScopeGuard(std::function<void()> callback)
    {
        d.callback = callback;
    }

    ~ScopeGuard()
    {
        d.callback();
    }

private:
    struct
    {
        std::function<void()> callback;
    } d;
};



}

#endif // KJUT_SCOPE_GUARD
