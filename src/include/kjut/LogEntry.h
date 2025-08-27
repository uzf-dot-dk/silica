#ifndef KJUT_LOG_ENTRY_H
#define KJUT_LOG_ENTRY_H

#include <stdio.h>
#include <stdarg.h>


#ifndef KJUT_LOGENTRY_MESSAGE_MAX_LENGTH
#define KJUT_LOGENTRY_MESSAGE_MAX_LENGTH 50
#endif

#ifndef KJUT_LOGENTRY_FILENAME_MAX_LENGTH
#define KJUT_LOGENTRY_FILENAME_MAX_LENGTH 50
#endif



namespace Kjut
{

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
        const size_t messageMaxLength = KJUT_LOGENTRY_MESSAGE_MAX_LENGTH;
        const size_t orignatingFileMaxLength = KJUT_LOGENTRY_FILENAME_MAX_LENGTH;
        char originatingFile[KJUT_LOGENTRY_FILENAME_MAX_LENGTH + 1];
        char message[KJUT_LOGENTRY_MESSAGE_MAX_LENGTH
                     + 1 // For null
                     + 1 // To help figuring out elision
        ];
        Type type;
    } d;

};


}

#endif // KJUT_LOG_ENTRY_H
