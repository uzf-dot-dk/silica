#include <silica/Mutex.h>

namespace Silica
{


Mutex::Mutex()
{}

Mutex::~Mutex()
{}

void Mutex::lock()
{
    d.mutex.lock();
}

void Mutex::unlock()
{
    d.mutex.unlock();

}

bool Mutex::tryLock()
{
    return d.mutex.try_lock();
}


};


