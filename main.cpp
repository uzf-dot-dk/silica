#include <kjut/LoggingSystem.h>
#include <stdio.h>
int main(int argc, char *argv[])
{

    Kjut::LogEntry le(__LINE__, __FILE__);
    le.format("Jeg er %d %s %s", 1, "flot", "ged");
    Kjut::LoggingSystem::instance()->sinkEntry(le);

    return 0;
}

