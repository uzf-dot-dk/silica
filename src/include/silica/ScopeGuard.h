#ifndef SILICA_SCOPE_GUARD_H
#define SILICA_SCOPE_GUARD_H

#include <functional>
namespace Silica
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

#endif // SILICA_SCOPE_GUARD
