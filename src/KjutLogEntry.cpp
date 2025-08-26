#include "include/kjut/LogEntry.h"
#include <stdio.h>
#include <string.h>


#ifdef _MSC_VER
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#define MIN(a,b) ( (a) < (b) ? (a) : (b) )
#define MAX(a,b) ( (a) < (b) ? (b) : (a) )

static const int elision_size = 3;

Kjut::LogEntry::LogEntry(size_t line, const char *originatingFile, const char *message)
    : Kjut::LogEntry(line, originatingFile)
{
    setMessage(message);
}

Kjut::LogEntry::~LogEntry()
{}

Kjut::LogEntry::LogEntry()
{}

Kjut::LogEntry::LogEntry(size_t line, const char *originatingFile)
    : LogEntry()
{
    d.originatingLine = line;
    setOriginatingFile(originatingFile);

}

void Kjut::LogEntry::setOriginatingFile(const char *originatingFile)
{
    ssize_t lenght_of_filename = strlen(originatingFile);
    ssize_t index_of_last_path_separator = lenght_of_filename-1;
    while(index_of_last_path_separator > 0)
    {
        if((originatingFile[index_of_last_path_separator] == '/' )
            ||
            (originatingFile[index_of_last_path_separator] == '\\' ))
        {
            index_of_last_path_separator;
            break;
        }
        index_of_last_path_separator--;
    }

    size_t bytesToCopy = lenght_of_filename - index_of_last_path_separator -1 ;

    memcpy(
        d.originatingFile,
        originatingFile+index_of_last_path_separator + 1 ,
        bytesToCopy);
    d.originatingFile[bytesToCopy] = 0;


}

const char *Kjut::LogEntry::originatingFile() const
{
    return d.originatingFile;
}

size_t Kjut::LogEntry::format(const char *str, ...)
{
    va_list args;
    va_start(args, str);
    const int bytesPrinted = vsnprintf(d.message, d.messageMaxLength+1, str, args);
    va_end(args);

    if(bytesPrinted > d.messageMaxLength)
    {
        for(int i = d.messageMaxLength - elision_size; i <d.messageMaxLength; i++)
        {
            d.message[i] = '.';
        }
    }
    return 0;
}

void Kjut::LogEntry::setMessage(const char *message, size_t length)
{
    int bytesToCopy = length;
    if(bytesToCopy == 0)
    {
        bytesToCopy = strlen(message);
    }
    bool shouldElisionBeDone = false;
    if( bytesToCopy > d.messageMaxLength)
    {
        shouldElisionBeDone = true;
        bytesToCopy = d.messageMaxLength - elision_size;
    }
    memcpy(d.message, message, bytesToCopy);
    if(shouldElisionBeDone)
    {
        for(int i = d.messageMaxLength - elision_size; i <d.messageMaxLength; i++)
        {
            d.message[i] = '.';
        }
    }
}

const char * const Kjut::LogEntry::message()
{
    return d.message;
}
