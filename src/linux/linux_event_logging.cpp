#include <kjut/LoggingSystem.h>

class OperatingSystemWithPrintfLogSink : public Kjut::LogSink
{
public:
    void sinkEntry(const Kjut::LogEntry &entry) override
    {
        printf("#%4zu : %-*s : %s\n", entry.originatingLine(), KJUT_LOGENTRY_FILENAME_MAX_LENGTH, entry.originatingFile(), entry.message());
        fflush(stdout);
    };
};

OperatingSystemWithPrintfLogSink sink;

void registerDefaultLogSink(Kjut::LoggingSystem *loggingSystem)
{
    loggingSystem->setSink(&sink);
}
