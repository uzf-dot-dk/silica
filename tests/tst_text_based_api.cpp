#include <gtest/gtest.h>

#include <silica/communication/TextAPI.h>

// START -- Remove this block, when reflection becomes available
#ifndef REFLECTION_FINALLY_GOT_HERE
#define REGISTER_METHOD( name ) registerCommand(#name, &ClassUnderTest:: name)
#endif
// END -- Remove this block, when reflection becomes available


#define suiteName test_text_based_api

class ClassUnderTest: public Kjut::TextBasedAPI<ClassUnderTest>
{

public:

    ClassUnderTest() : Kjut::TextBasedAPI<ClassUnderTest>()
    {
        REGISTER_METHOD(voidNoParams);
        REGISTER_METHOD(intWithIntInt);
        REGISTER_METHOD(doubleWithIntDouble);
        REGISTER_METHOD(throwingABall);
    }

    void voidNoParams()
    {
        called = true;
    }

    int intWithIntInt(int a, int b)
    {
        called = true;
        return a + b;
    }

    double doubleWithIntDouble(int a, double b)
    {
        called = true;
        return a * b;
    }

    void throwingABall()
    {
        called = true;
        throw "Ball";
    }

    bool called = false;

};




TEST(suiteName, voidNoParams)
{
    ClassUnderTest cut;
    ASSERT_FALSE(cut.called);

    auto R = cut.execute("voidNoParams");

    ASSERT_EQ(R.returnCode, 0);
    ASSERT_EQ(std::string(R.output), "");
    ASSERT_TRUE(cut.called);
}


TEST(suiteName, intWithIntInt)
{
    ClassUnderTest cut;
    ASSERT_FALSE(cut.called);

    auto R = cut.execute("intWithIntInt 11 22");

    ASSERT_EQ(R.returnCode, 0);
    ASSERT_EQ(std::string(R.output), "33");
    ASSERT_TRUE(cut.called);
}

TEST(suiteName, doubleWithIntDouble)
{
    ClassUnderTest cut;
    ASSERT_FALSE(cut.called);

    auto R = cut.execute("doubleWithIntDouble 4 3.1415");

    ASSERT_EQ(R.returnCode, 0);
    ASSERT_EQ(std::string(R.output).substr(0, 5), "1.256"/*600000E+001*/);
    ASSERT_TRUE(cut.called);
}


TEST(suiteName, noSuchFunction)
{
    ClassUnderTest cut;
    ASSERT_FALSE(cut.called);

    auto R = cut.execute("noSuchFunction 4 3.1415");

    ASSERT_EQ(R.returnCode, static_cast<int>(ClassUnderTest::Error::NoSuchMethod));
    ASSERT_FALSE(cut.called);
}

TEST(suiteName, conversionError)
{
    ClassUnderTest cut;
    ASSERT_FALSE(cut.called);

    auto R = cut.execute("intWithIntInt fubar 123");

    ASSERT_EQ(R.returnCode, static_cast<int>(ClassUnderTest::Error::ConversionError));
    ASSERT_FALSE(cut.called);
}


TEST(suiteName, parameterCountMisMatch)
{
    ClassUnderTest cut;
    ASSERT_FALSE(cut.called);

    auto R = cut.execute("intWithIntInt 123");

    ASSERT_EQ(R.returnCode, static_cast<int>(ClassUnderTest::Error::ParameterCountMismatch));
    ASSERT_FALSE(cut.called);
}


TEST(suiteName, methodThrowsException)
{
    ClassUnderTest cut;
    ASSERT_FALSE(cut.called);

    auto R = cut.execute("throwingABall");

    ASSERT_EQ(R.returnCode, static_cast<int>(ClassUnderTest::Error::MethodThrewException));
    ASSERT_TRUE(cut.called);
}
