#include <gtest/gtest.h>
#include <silica/Array.h>
#include "array_helpers.h"

#include <sstream>

#define suiteName tst_array_fixed_size_dynamic_size_interchangability

size_t mockCount = 0;

void mockLogEntryHandler(int, const char*, const char*)
{
    mockCount++;
}

size_t mockLogEntryHandlerInvocationCount()
{
    return mockCount;
}

void registerMockLogEntryHandler()
{
    //Kjut::registerLoghandler(mockLogEntryHandler);
    mockCount = 0;
}


TEST(suiteName, test_static_dynamic_interchangability_function_calls)
{
    auto assertCapacity  = []( Kjut::Array<int> &array)
    {

        static int invocationCount = 0;
        if(invocationCount == 0)
        {
            ASSERT_EQ(array.capacity(), SILICA_ARRAY_INITIAL_CAPACITY);
        }
        else if(invocationCount == 1)
        {
            ASSERT_EQ(array.capacity(), 55);
        }
        else
        {
            //Nop
        }
        invocationCount ++;
    };

    Kjut::Array<int> dynamicSized;
    Kjut::Array<int, 55> staticSized;

    assertCapacity(dynamicSized);
    assertCapacity(staticSized);
}

TEST(suiteName, test_static_dynamic_interchangability_ostream)
{

    {
        Kjut::Array<int> dynamicSized;
        std::ostringstream stringBulder;

        dynamicSized.append(1);
        dynamicSized.append(2);
        dynamicSized.append(3);
        dynamicSized.append(4);
        dynamicSized.append(5);
        dynamicSized.append(6);
        dynamicSized.append(7);
        dynamicSized.append(8);
        dynamicSized.append(9);
        dynamicSized.append(10);

        stringBulder << dynamicSized;

        ASSERT_EQ(stringBulder.str(), std::string("[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]"));
    }

    {
        Kjut::Array<int, 10> staticSized;
        std::ostringstream stringBulder;

        staticSized.append(1);
        staticSized.append(2);
        staticSized.append(3);
        staticSized.append(4);
        staticSized.append(5);
        staticSized.append(6);
        staticSized.append(7);
        staticSized.append(8);
        staticSized.append(9);
        staticSized.append(10);
        staticSized.append(11);
        staticSized.append(12);

        stringBulder << staticSized;

        ASSERT_EQ(stringBulder.str(), std::string("[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]"));
    }
}

TEST(suiteName, test_static_dynamic_interchangability_equals_operator)
{
    Kjut::Array<int, 10> staticSized;
    Kjut::Array<int> dynamicSized;

    {
        [[maybe_unused]] const bool doesThisCompile = staticSized == dynamicSized;
    }
    {
        [[maybe_unused]] const bool doesThisCompile = dynamicSized == staticSized;
    }
    {
        [[maybe_unused]] const bool doesThisCompile = dynamicSized == dynamicSized;
    }
    {
        [[maybe_unused]] const bool doesThisCompile = staticSized == staticSized;
    }
}
