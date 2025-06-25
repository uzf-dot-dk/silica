#include <kjut/Mutex.h>

namespace Kjut
{
MutexLocker::MutexLocker(Mutex &target)
    : target(target)
{
    this->target.lock();
}

MutexLocker::~MutexLocker()
{
    this->target.unlock();
}

}
