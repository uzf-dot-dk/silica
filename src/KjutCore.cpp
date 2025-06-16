#include <kjut/Debug.h>

#include <stdarg.h>
#include <stdio.h>

namespace Kjut
{

    void defaultLogHandler(int lineNumber , const char* fileName, const char* message)
    {
        fprintf(stderr, "%s:%d : %s\n", fileName, lineNumber, message);
        fflush(stderr);
    }

    static LogHandler logHandler = defaultLogHandler;

    void registerLoghandler(LogHandler newHandler)
    {
        logHandler = newHandler;
    }

    void processLogEntry(int lineNumber , const char* fileName, const char* message)
    {
        logHandler(lineNumber , fileName, message);
    }

    void createLogEntry(size_t lineno, const char *filename, const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        char buffer[200];
        vsnprintf(buffer, 200, fmt, args);
        va_end(args);


        const char * baseFileName = filename;
        while(*baseFileName != '\0')
        {
            baseFileName++;
        }
        while( ! (*baseFileName == '\\' || *baseFileName == '/') )
        {
            baseFileName--;
        }
        baseFileName++;

        Kjut::processLogEntry(__LINE__, baseFileName, buffer);
    }


}

#include <kjut/Array.hpp>
