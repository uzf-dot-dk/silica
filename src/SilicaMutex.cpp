#include <silica/Mutex.h>

namespace Silica
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
