#include <kjut/communication/TextAPI.h>

class MyApi : public Kjut::TextBasedAPI<MyApi>
{

public:

    MyApi() : Kjut::TextBasedAPI<MyApi>()
    {

// START -- Remove this block, when reflection becomes available
#ifndef REFLECTION_FINALLY_GOT_HERE
#define REGISTER_METHOD( name ) registerCommand(#name, &MyApi :: name)

        REGISTER_METHOD(chant);
        REGISTER_METHOD(storeTruth);
        REGISTER_METHOD(doStuff);
        REGISTER_METHOD(addStuff);
        REGISTER_METHOD(doNuffin);

#undef REGISTER_METHOD
#endif
// end -- Remove this block, when reflection becomes available
    }

    void doNuffin()
    {
        printf("doNuffin called\n"); fflush(stdout);
    }

    int doStuff()
    {
        printf("doStuff called\n"); fflush(stdout);
        return 3;
    }

    int addStuff(int a, double b)
    {
        printf("Add stuff called with %d and %e\n", a, b); fflush(stdout);
        return a + b;
    }

    int storeTruth(bool theValue)
    {
        printf("storeTruth called with %s \n", theValue ? "TRUE" : "FALSE"); fflush(stdout);
        return 1;
    }

    double chant(std::string_view message, int count)
    {
        printf("Chant called with %d x \"%.*s\" \n",
               count ,
               static_cast<int>(message.length()),
               message.data());
        return 3.1415 * count;
    }

};




int main(int argc, char *argv[])
{
    MyApi tba;
    { auto r = tba.execute("doNuffin"); printf("%s\n", r.output); }
    { auto r = tba.execute("doStuff"); printf("%s\n", r.output); }
    { auto r = tba.execute("addStuff 0b100 53.7"); printf("%s\n", r.output); }
    { auto r = tba.execute("storeTruth 0"); printf("%s\n", r.output); }
    { auto r = tba.execute("chant hey 5"); printf("%s\n", r.output); }

    fflush(stdout);
    /*
    Kjut::Application app;
    Kjut::CoarseTimer everySecond;
    everySecond.triggered.connectTo([&](){
        printf("Ping...\n");
        fflush(stdout);
    });
    everySecond.setTimeout(1'000'000_us);
    everySecond.start();

    return app.exec();
*/
    return 0;
}

