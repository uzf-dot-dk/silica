#include <kjut/LoggingSystem.h>
#include <stdlib.h>

#define TEXT_NORMAL  "\x1B[0m"
#define TEXT_RED     "\x1B[31m"
#define TEXT_GREEN   "\x1B[32m"
#define TEXT_YELLOW  "\x1B[33m"
#define TEXT_BLUE    "\x1B[34m"
#define TEXT_MAGENTA "\x1B[35m"
#define TEXT_CYAN    "\x1B[36m"
#define TEXT_WHITE   "\x1B[37m"

class OperatingSystemWithPrintfLogSink : public Kjut::LogSink
{
public:
    void sinkEntry(const Kjut::LogEntry &entry) override
    {
        const char *color;
        FILE* descriptor;
        char indicator;
        switch(entry.type())
        {
        case Kjut::LogEntry::Type::Log:
            color = TEXT_NORMAL;
            descriptor = stdout;
            indicator = 'L';
            break;
        case Kjut::LogEntry::Type::Warning:
            color = TEXT_YELLOW;
            descriptor = stdout;
            indicator = 'W';
            break;
        case Kjut::LogEntry::Type::Fatal:
            color = TEXT_RED;
            descriptor = stderr;
            indicator = 'F';
            break;
        }

        fprintf(descriptor, "%s# %c : %4zu : %-*s : %s\n" TEXT_NORMAL, color, indicator, entry.originatingLine(), KJUT_LOGENTRY_FILENAME_MAX_LENGTH, entry.originatingFile(), entry.message());
        fflush(descriptor);

        if(entry.type() == Kjut::LogEntry::Type::Fatal)
        {
            ::exit(1);
        }

    };
};

OperatingSystemWithPrintfLogSink sink;

void registerDefaultLogSink(Kjut::LoggingSystem *loggingSystem)
{
    loggingSystem->setSink(&sink);
}
