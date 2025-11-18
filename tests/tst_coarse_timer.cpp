#include <gtest/gtest.h>
#include <silica/Application.h>
#include <silica/CoarseTimer.h>

#include <chrono>

using namespace std::chrono;


#define suiteName tst_coarse_timer


TEST(suiteName, test_simple_timer)
{


    Silica::Application app;
    Silica::CoarseTimer everySecond;
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
    Silica::Application app;

    int counter = 0;
    Silica::CoarseTimer singleShotTimer;
    singleShotTimer.triggered.connectTo([&](){
        counter++;
    });
    singleShotTimer.setTimeout(100'000_us);
    singleShotTimer.setType(Silica::CoarseTimer::Type::SingleShot);
    singleShotTimer.start();



    Silica::CoarseTimer exitTimer;
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
    Silica::Application app;

    int counter = 0;
    Silica::CoarseTimer singleShotTimer;
    singleShotTimer.triggered.connectTo([&](){
        counter++;
    });
    singleShotTimer.setTimeout(100'000_us);
    singleShotTimer.setType(Silica::CoarseTimer::Type::Repeated);
    singleShotTimer.start();



    Silica::CoarseTimer exitTimer;
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


