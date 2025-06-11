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
void createLogEntry(size_t lineno, const char *filename, const char *fmt, ...);

    typedef void(*LogHandler)(int, const char*, const char*);

    void registerLoghandler(LogHandler newHandler);
    void processLogEntry(int, const char*, const char*);
}
