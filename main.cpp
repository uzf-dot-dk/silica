#include <kjut/Application.h>
#include <kjut/CoarseTimer.h>
#include <kjut/SignalSlot.h>

void doStuff()
{
    LOG("Does stuff");
}

int main(int argc, char *argv[])
{
    Kjut::Application app;
    Kjut::CoarseTimer everySecond;
    everySecond.triggered.connectTo(doStuff);
    everySecond.setTimeout(1000);
    everySecond.start();


    return app.exec();

}

