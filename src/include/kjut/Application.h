#ifndef KJUT_APPLICATION_H
#define KJUT_APPLICATION_H

#include <stdio.h>


namespace Kjut
{



class Application
{

public:
    Application();
    virtual ~Application() = default;

    static Application * instance();
    void assertInstanceExists(const char *messageIfFailed );

};


}

#endif // KJUT_APPLICATION_H
