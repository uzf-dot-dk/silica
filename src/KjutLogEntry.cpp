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
    ssize_t index_first_character_after_last_path_separator = lenght_of_filename-1;
    while(index_first_character_after_last_path_separator > 0)
    {
        if((originatingFile[index_first_character_after_last_path_separator] == '/' )
            ||
            (originatingFile[index_first_character_after_last_path_separator] == '\\' ))
        {
            index_first_character_after_last_path_separator++;
            break;
        }
        index_first_character_after_last_path_separator--;
    }

    size_t bytesToCopy = lenght_of_filename - index_first_character_after_last_path_separator  ;
    bool should_elide = false;
    if(bytesToCopy > KJUT_LOGENTRY_MESSAGE_MAX_LENGTH)
    {
        should_elide = true;
        bytesToCopy = KJUT_LOGENTRY_MESSAGE_MAX_LENGTH;
    }

    if ( ! should_elide )
    {
        memcpy(
            d.originatingFile,
            originatingFile+index_first_character_after_last_path_separator ,
            bytesToCopy);
        d.originatingFile[bytesToCopy] = 0;
    }
    else
    {
        const size_t left_part_size = (KJUT_LOGENTRY_FILENAME_MAX_LENGTH-elision_size)/2;
        const size_t right_part_size = KJUT_LOGENTRY_FILENAME_MAX_LENGTH-elision_size-left_part_size;

        const char * const source_left_part_start = originatingFile+index_first_character_after_last_path_separator;
        const char * const source_left_part_end = source_left_part_start + left_part_size;
        const char * const source_right_part_start = originatingFile+(strlen(originatingFile)-right_part_size);

        char * destination_left_part_start = d.originatingFile;
        char * destination_right_part_start = d.originatingFile + left_part_size+elision_size;

        memcpy(destination_left_part_start, source_left_part_start, left_part_size);
        memcpy(destination_right_part_start, source_right_part_start, right_part_size);

        d.originatingFile[d.orignatingFileMaxLength] = 0;

        for(size_t i = 0; i < elision_size; i++)
        {
            d.originatingFile[left_part_size+i] = '.';
        }





    }


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
    d.message[bytesToCopy] = 0;
    if(shouldElisionBeDone)
    {
        for(int i = d.messageMaxLength - elision_size; i <d.messageMaxLength; i++)
        {
            d.message[i] = '.';
        }
        d.message[d.messageMaxLength] = 0;
    }
}

const char * const Kjut::LogEntry::message() const
{
    return d.message;
}
