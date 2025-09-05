#include <kjut/Application.h>
#include <kjut/CoarseTimer.h>
#include <kjut/SignalSlot.h>

int main(int argc, char *argv[])
{
    Kjut::Application app;

    Kjut::CoarseTimer everySecond;
    everySecond.triggered.connectTo([&]()
    {
        LOG("LAMBDA does stuff...");
        app.exit(1);
    });
    everySecond.setTimeout(1000_ms);
    everySecond.start();

    return app.exec();

}

