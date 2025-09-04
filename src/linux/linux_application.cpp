#include <sys/time.h>
#include <cstdint>
#include <kjut/Application.h>





namespace Kjut
{

    MicroSeconds Application::microsecondsSinceStart() const
    {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return MicroSeconds(static_cast<int64_t>(tv.tv_sec) * 1000000 + tv.tv_usec);
    }
}
