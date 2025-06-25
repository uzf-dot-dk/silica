#include <kjut/Mutex.h>




int main() {

    Kjut::Mutex m;
    Kjut::MutexLocker ml(m);

    return 0;
}

