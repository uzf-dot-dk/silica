#include <gtest/gtest.h>
#include <kjut/Application.h>
#include <kjut/CoarseTimer.h>

#include <chrono>

using namespace std::chrono;


#define suiteName tst_coarse_timer


TEST(suiteName, test_simple_timer)
{


    Kjut::Application app;
    Kjut::CoarseTimer everySecond;
    everySecond.triggered.connectTo([&](){
        app.exit(27);
    });
    everySecond.setTimeout(1'000'000_us);
    everySecond.start();

    auto start = high_resolution_clock::now();
    int exitCode;

    exitCode = app.exec();

    auto end = high_resolution_clock::now();
    auto elapsed = duration_cast<milliseconds>(end - start);
    EXPECT_GT(elapsed.count() , 900);
    EXPECT_LT(elapsed.count(), 1100);

    ASSERT_EQ(exitCode, 27);
}



TEST(suiteName, test_single_shot_timer)
{
    Kjut::Application app;

    int counter = 0;
    Kjut::CoarseTimer singleShotTimer;
    singleShotTimer.triggered.connectTo([&](){
        counter++;
    });
    singleShotTimer.setTimeout(100'000_us);
    singleShotTimer.setType(Kjut::CoarseTimer::Type::SingleShot);
    singleShotTimer.start();



    Kjut::CoarseTimer exitTimer;
    exitTimer.triggered.connectTo([&]()
    {
        app.exit(27);
    });
    exitTimer.setTimeout(1'050'000_us);
    exitTimer.start();


    auto start = high_resolution_clock::now();
    int exitCode = app.exec();
    auto end = high_resolution_clock::now();
    auto elapsed = duration_cast<milliseconds>(end - start);
    EXPECT_GT(elapsed.count() , 1000);
    EXPECT_LT(elapsed.count(),  1200);

    ASSERT_EQ(counter, 10);
    ASSERT_EQ(exitCode, 27);
}


TEST(suiteName, test_repeated_timer)
{
    Kjut::Application app;

    int counter = 0;
    Kjut::CoarseTimer singleShotTimer;
    singleShotTimer.triggered.connectTo([&](){
        counter++;
    });
    singleShotTimer.setTimeout(100'000_us);
    singleShotTimer.setType(Kjut::CoarseTimer::Type::Repeated);
    singleShotTimer.start();



    Kjut::CoarseTimer exitTimer;
    exitTimer.triggered.connectTo([&]()
                                  {
                                      app.exit(27);
                                  });
    exitTimer.setTimeout(1'050'000_us);
    exitTimer.start();


    auto start = high_resolution_clock::now();
    int exitCode = app.exec();
    auto end = high_resolution_clock::now();
    auto elapsed = duration_cast<milliseconds>(end - start);
    EXPECT_GT(elapsed.count() , 1000);
    EXPECT_LT(elapsed.count(),  1200);

    ASSERT_EQ(counter, 10);
    ASSERT_EQ(exitCode, 27);
}


