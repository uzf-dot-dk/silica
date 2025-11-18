#ifndef SILICA_LOG_ENTRY_H
#define SILICA_LOG_ENTRY_H

#include <stdio.h>
#include <stdarg.h>


#ifndef SILICA_LOGENTRY_MESSAGE_MAX_LENGTH
#define SILICA_LOGENTRY_MESSAGE_MAX_LENGTH 50
#endif

#ifndef SILICA_LOGENTRY_FILENAME_MAX_LENGTH
#define SILICA_LOGENTRY_FILENAME_MAX_LENGTH 50
#endif



namespace Kjut
{

/**
\brief LogEntry models a single entry in to the logging system.

LogEntry models a single entry in to the logging system. This is the lowest level of granularty there exists in the logging system.
\ingroup Core
\ingroup Logging

*/
class LogEntry
{

public:

    enum class Type
    {
        Log,
        Warning,
        Fatal
    };


    LogEntry();
    LogEntry(size_t line, const char *originatingFile);
    LogEntry(size_t line, const char *originatingFile, const char *message);
    ~LogEntry();

    size_t format(const char *, ...);
    void setMessage(const char * message, size_t length = 0);
    char const * const message() const;

    void setOriginatingFile(const char *fileName);
    const char * originatingFile() const;

    void setType(Type type);
    Type type() const;

    size_t originatingLine() const { return d.originatingLine; }

private:
    struct
    {
        size_t originatingLine;
        const size_t messageMaxLength = SILICA_LOGENTRY_MESSAGE_MAX_LENGTH;
        const size_t orignatingFileMaxLength = SILICA_LOGENTRY_FILENAME_MAX_LENGTH;
        char originatingFile[SILICA_LOGENTRY_FILENAME_MAX_LENGTH + 1];
        char message[SILICA_LOGENTRY_MESSAGE_MAX_LENGTH
                     + 1 // For null
                     + 1 // To help figuring out elision
        ];
        Type type;
    } d;

};


}

#endif // SILICA_LOG_ENTRY_H
