#include <gtest/gtest.h>
#include <kjut/Array.h>

#define suiteName tst_array_fixed_size

size_t mockCount = 0;

void mockHandler(int, const char*, const char*)
{
    mockCount++;
}

size_t mockHandlerInvocationCount()
{
    return mockCount;
}

void registerMockHandler()
{
    Kjut::registerLoghandler(mockHandler);
    mockCount = 0;
}


#define ASSERT_LOG_COUNT_TRIGGERED(expression, expectedCount) {\
    const auto oldCount = mockHandlerInvocationCount(); \
    expression; \
    const auto newCount = mockHandlerInvocationCount(); \
    ASSERT_EQ( (expectedCount) , (newCount - oldCount)); \
};


TEST(suiteName, test_default_constructor)
{
    registerMockHandler();
    constexpr size_t CAPACITY= 10;
    Kjut::Array<int, CAPACITY> array;

    ASSERT_EQ(array.size(), 0);
    ASSERT_EQ(array.capacity(), CAPACITY);

    ASSERT_EQ(0, mockHandlerInvocationCount());
}

TEST(suiteName, test_appending_and_reading_back_within_bounds)
{
    registerMockHandler();
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
    ASSERT_EQ(0, mockHandlerInvocationCount());
}


TEST(suiteName, test_appending_and_reading_back_beoynd_bounds)
{
    registerMockHandler();

    constexpr size_t CAPACITY = 8;
    Kjut::Array<int, CAPACITY> array;

    for(size_t i = 0; i < CAPACITY; i++)
    {
        const bool wasInsertionSuccessful = array.append (  (i+1)*10+(i+1) );
        ASSERT_TRUE(wasInsertionSuccessful);
    }
    {
        const size_t expectedMockCount = 1 + mockHandlerInvocationCount();
        const bool wasInsertionSuccessful = array.append (  (CAPACITY+1)*10+(CAPACITY+1) );
        ASSERT_FALSE(wasInsertionSuccessful);
        ASSERT_EQ(expectedMockCount, mockHandlerInvocationCount());
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
    registerMockHandler();

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

    ASSERT_EQ(0, mockHandlerInvocationCount());
}


TEST(suiteName, test_index_operator_out_of_bounds)
{
    registerMockHandler();

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

