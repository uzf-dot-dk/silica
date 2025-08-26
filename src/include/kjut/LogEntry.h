#ifndef KJUT_APPLICATION_H
#define KJUT_APPLICATION_H

#include <stdio.h>
#include <stdarg.h>


#ifndef KJUT_LOGENTRY_MESSAGE_MAX_LENGTH
    #define KJUT_LOGENTRY_MESSAGE_MAX_LENGTH 50
#endif
namespace Kjut
{

class LogEntry
{

public:
    LogEntry();
    LogEntry(size_t line, const char *originatingFile);
    LogEntry(size_t line, const char *originatingFile, const char *message);
    ~LogEntry();

    size_t format(const char *, ...);
    void setMessage(const char * message, size_t length = 0);
    char const * const message();

    void setOriginatingFile(const char *fileName);
    const char * originatingFile() const;

private:
    struct
    {
        size_t originatingLine;
        const size_t messageMaxLength = KJUT_LOGENTRY_MESSAGE_MAX_LENGTH;
        char originatingFile[KJUT_LOGENTRY_MESSAGE_MAX_LENGTH + 1];
        char message[KJUT_LOGENTRY_MESSAGE_MAX_LENGTH
                     + 1 // For null
                     + 1 // To help figuring out elision
        ];
    } d;

};


}

#endif // KJUT_APPLICATION_H
