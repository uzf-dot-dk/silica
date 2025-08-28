#include <kjut/LoggingSystem.h>
class CountingLogSink : public Kjut::LogSink
{
public:

    CountingLogSink()
    {
        reset();
    }

    void sinkEntry(const Kjut::LogEntry &entry)
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
