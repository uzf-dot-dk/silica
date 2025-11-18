#include <silica/LoggingSystem.h>
class CountingLogSink : public Silica::LogSink
{
public:

    CountingLogSink()
    {
        reset();
    }

    void sinkEntry(const Silica::LogEntry &entry)
    {
        d.invocations++;
    }

    int invocationCount() const
    {
        return d.invocations;
    }

    void reset()
    {
        d.invocations = 0;
    }

private:
    struct
    {
        int invocations;
    } d;
};
