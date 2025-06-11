#include <gtest/gtest.h>

#include <kjut/Array.h>

#include "array_helpers.h"


#define suiteName tst_array_fixed_size

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
    Kjut::registerLoghandler(mockLogEntryHandler);
    mockCount = 0;
}

void resetMockLogEntryCount()
{
    mockCount = 0;
}


TEST(suiteName, test_default_constructor)
{
    registerMockLogEntryHandler();
    constexpr size_t CAPACITY= 10;
    Kjut::Array<int, CAPACITY> array;

    ASSERT_EQ(array.size(), 0);
    ASSERT_EQ(array.capacity(), CAPACITY);

    ASSERT_EQ(0, mockLogEntryHandlerInvocationCount());
}

TEST(suiteName, test_appending_and_reading_back_within_bounds)
{
    registerMockLogEntryHandler();
    constexpr size_t CAPACITY = 8;
    Kjut::Array<int, CAPACITY> array;

    for(size_t i = 0; i < CAPACITY; i++)
    {
        const bool wasInsertionSuccessful = array.append (  (i+1)*10+(i+1) );
        ASSERT_TRUE(wasInsertionSuccessful);
    }

    for(size_t i = 0; i < CAPACITY; i++)
    {
        ASSERT_EQ(
            array.at(i)
            ,
            (i+1)*10+(i+1)
            );
    }
    ASSERT_EQ(0, mockLogEntryHandlerInvocationCount());
}


TEST(suiteName, test_appending_and_reading_back_beoynd_bounds)
{
    registerMockLogEntryHandler();

    constexpr size_t CAPACITY = 8;
    Kjut::Array<int, CAPACITY> array;

    for(size_t i = 0; i < CAPACITY; i++)
    {
        const bool wasInsertionSuccessful = array.append (  (i+1)*10+(i+1) );
        ASSERT_TRUE(wasInsertionSuccessful);
    }
    {
        const size_t expectedMockCount = 1 + mockLogEntryHandlerInvocationCount();
        const bool wasInsertionSuccessful = array.append (  (CAPACITY+1)*10+(CAPACITY+1) );
        ASSERT_FALSE(wasInsertionSuccessful);
        ASSERT_EQ(expectedMockCount, mockLogEntryHandlerInvocationCount());
    }

    // Read back that the out of bounds appending, did not mess with data.
    for(size_t i = 0; i < CAPACITY; i++)
    {
        ASSERT_EQ(
            array.at(i)
            ,
            (i+1)*10+(i+1)
            );
    }
}

TEST(suiteName, test_index_operator_within_bounds)
{
    registerMockLogEntryHandler();

    constexpr size_t CAPACITY = 8;
    Kjut::Array<int, CAPACITY> array;

    for(size_t i = 0; i < CAPACITY; i++)
    {
        array.append( 1+i );
    }

    for(size_t i = 0; i < CAPACITY; i++)
    {
        ASSERT_EQ(array[i],  i + 1);
    }

    for(size_t i = 0; i < CAPACITY; i++)
    {
        array[i] = (i + 1) * 10;
    }

    for(size_t i = 0; i < CAPACITY; i++)
    {
        ASSERT_EQ(array[i],  (i + 1) * 10);
    }

    ASSERT_EQ(0, mockLogEntryHandlerInvocationCount());
}


TEST(suiteName, test_index_operator_out_of_bounds)
{
    registerMockLogEntryHandler();

    constexpr size_t CAPACITY = 8;
    Kjut::Array<int, CAPACITY> array;

    for(size_t i = 0; i < CAPACITY; i++)
    {
        array.append( 1+i );
    }

    ASSERT_LOG_COUNT_TRIGGERED(
        array[CAPACITY*2] = 17
        ,
        1);

    for(size_t i = 0; i < CAPACITY; i++)
    {
        ASSERT_EQ(array[i],  i + 1);
    }

}

TEST(suiteName, test_insertion)
{

    #define CREATE_ARRAY(name) \
    constexpr size_t CAPACITY = 8; \
        Kjut::Array<int, CAPACITY> name; \
        for(size_t i = 0; i < CAPACITY-1; i++) \
    { \
            name.append( 1+i ); \
    }

    registerMockLogEntryHandler();

    std::vector<std::vector<int>> cases = {
        {100,1,2,3,4,5,6,7},
        {1,100,2,3,4,5,6,7},
        {1,2,100,3,4,5,6,7},
        {1,2,3,100,4,5,6,7},
        {1,2,3,4,100,5,6,7},
        {1,2,3,4,5,100,6,7},
        {1,2,3,4,5,6,100,7},
        {1,2,3,4,5,6,7,100},
        // Beoynd end
        {1,2,3,4,5,6,7},
        {1,2,3,4,5,6,7},
        {1,2,3,4,5,6,7},
        {1,2,3,4,5,6,7},
    };

    for(size_t i = 0; i < cases.size(); i++)
    {
        CREATE_ARRAY(array);

        resetMockLogEntryCount();

        const bool shouldExpectTrue = (cases[i].size() >= CAPACITY);
        const bool wasInsertionSuccesful = array.insert(i, 100);
        const int expectedLogEntries = shouldExpectTrue ? 0 : 1;

        ASSERT_EQ(shouldExpectTrue, wasInsertionSuccesful);
        ASSERT_EQ(expectedLogEntries, mockLogEntryHandlerInvocationCount());
        ASSERT_KJUT_ARRAY_AND_STD_VECTOR_EQ(array, cases[i])
    }

    #undef CREATE_ARRAY
}

