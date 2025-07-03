#include <stdio.h>

namespace Kjut
{

/** E.g. :

```
#define MyWarning(message, ...) createLogEntry(__LINE__, __FILE__, message, __VA_ARGS__)
MyWarning("Foo");
MyWarning("Foo and %d bars", 24);

```
*/
    ///@cond

    void createLogEntry(size_t lineno, const char *filename, const char *fmt, ...);
    void processLogEntry(int, const char*, const char*);
    ///@endcond

    /**
    LogHandler is a type dhat. ...*/
    typedef void(*LogHandler)(int, const char*, const char*);

    /**
     *  \anchor registerLoghandler_method
     *  Register log handler is a methog dhat .... */
    void registerLoghandler(LogHandler newHandler);

}
