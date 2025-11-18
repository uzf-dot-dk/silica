
#include <tuple>
#include <vector>
#include <silica/SignalSlot.h>

template <typename ...Ts>
class SignalSpy
{
public:
    void spyOn(Silica::Signal<Ts...> *signal)
    {
        signal->connectTo([this](Ts... args){
            d.invocations.push_back(std::tuple<Ts...>(args...));
        });
    }

    size_t invocationCount() const
    {
        return d.invocations.size();
    }

    std::vector<std::tuple<Ts...>> & invocations()
    {
        return d.invocations;
    }


private:
    struct
    {
        std::vector<std::tuple<Ts...>> invocations;
    } d;
};

