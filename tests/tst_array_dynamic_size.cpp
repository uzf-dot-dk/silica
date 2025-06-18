#include <gtest/gtest.h>

#define KJUT_ARRAY_INITIAL_CAPACITY 1

#include <kjut/Array.hpp>

#include "array_helpers.h"


#define suiteName tst_array_dynamic_size

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
    Kjut::Array<int> array;

    constexpr size_t CAPACITY = 8;

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

TEST(suiteName, test_index_operator_within_bounds)
{
    registerMockLogEntryHandler();

    constexpr size_t CAPACITY = 8;
    Kjut::Array<int> array;

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
    Kjut::Array<int> array;

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
    constexpr size_t CONTENT_SIZE = 8; \
        Kjut::Array<int> name; \
        for(size_t i = 0; i < CONTENT_SIZE-1; i++) \
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

        const bool shouldExpectTrue = (cases[i].size() >= CONTENT_SIZE);
        const bool wasInsertionSuccesful = array.insert(i, 100);
        const int expectedLogEntries = shouldExpectTrue ? 0 : 1;

        ASSERT_EQ(shouldExpectTrue, wasInsertionSuccesful);
        ASSERT_EQ(expectedLogEntries, mockLogEntryHandlerInvocationCount());
        ASSERT_KJUT_ARRAY_AND_STD_VECTOR_EQ(array, cases[i])
    }

    #undef CREATE_ARRAY
}



TEST(suiteName, test_removal)
{

#define CREATE_ARRAY(name) \
    constexpr size_t CONTENT_SIZE = 8; \
        Kjut::Array<int> name; \
        for(size_t i = 0; i < CONTENT_SIZE; i++) \
    { \
            name.append( 1+i ); \
    }

    registerMockLogEntryHandler();

    std::vector<std::vector<int>> cases = {
        {2,3,4,5,6,7,8},
        {1,3,4,5,6,7,8},
        {1,2,4,5,6,7,8},
        {1,2,3,5,6,7,8},
        {1,2,3,4,6,7,8},
        {1,2,3,4,5,7,8},
        {1,2,3,4,5,6,8},
        {1,2,3,4,5,6,7},
        //Beyond end
        {1,2,3,4,5,6,7,8},
        {1,2,3,4,5,6,7,8},
        {1,2,3,4,5,6,7,8},
        {1,2,3,4,5,6,7,8},
        {1,2,3,4,5,6,7,8},
        {1,2,3,4,5,6,7,8},

    };

    for(size_t i = 0; i < cases.size(); i++)
    {

        CREATE_ARRAY(array);

        resetMockLogEntryCount();

        const bool shouldExpectTrue = (cases[i].size() < CONTENT_SIZE);
        const bool wasRemovalSuccesful = array.remove(i);
        const int expectedLogEntries = shouldExpectTrue ? 0 : 1;

        ASSERT_EQ(shouldExpectTrue, wasRemovalSuccesful);
        ASSERT_EQ(expectedLogEntries, mockLogEntryHandlerInvocationCount());
        ASSERT_KJUT_ARRAY_AND_STD_VECTOR_EQ(array, cases[i])
    }

#undef CREATE_ARRAY
}


class DeletableInteger
{
public:
    DeletableInteger() { value = 0; };
    DeletableInteger(int i) { value = i; };
    ~DeletableInteger() {
        DeletableInteger::deletedIntvalues.insert(this->value);
    }

public:
    static std::set<int> deletedIntvalues;
    int value;
};
std::set<int> DeletableInteger::deletedIntvalues;

std::ostream &operator<<(std::ostream &os, DeletableInteger &i) {
    os << "I" << i.value;
    return os;
}

TEST(suiteName, test_removal_causes_deletion)
{
    Kjut::Array<DeletableInteger> ints = {1,2,3};

    DeletableInteger::deletedIntvalues.clear();
    ints.remove(1);
    ASSERT_EQ(std::set<int>{2}, DeletableInteger::deletedIntvalues);
}


TEST(suiteName, test_removal_of_pointers_works)
{
    DeletableInteger::deletedIntvalues.clear();

    DeletableInteger *one, *two, *three;
    Kjut::Array<DeletableInteger*> ints = {
        one = new DeletableInteger(1),
        two = new DeletableInteger(2),
        three = new DeletableInteger(3)
    };

    DeletableInteger::deletedIntvalues.clear();
    ints.remove(1);
    ASSERT_EQ(std::set<int>{}, DeletableInteger::deletedIntvalues);

    delete one;
    delete two;
    delete three;

}

TEST(suiteName, test_deletion_deletes_content)
{
    {

        Kjut::Array<DeletableInteger> ints = {1,2,3};
        DeletableInteger::deletedIntvalues.clear();
    }
    const int expectedDeletions = 3 // the three elements in the array
                                + 1; //For the `outOfBoundElement`
    ASSERT_EQ(expectedDeletions, DeletableInteger::deletedIntvalues.size());
}


TEST(suiteName, test_iterator_builds_with_stdlib)
{
    Kjut::Array<int> ints;

    std::sort(ints.begin(), ints.end());

    for(const auto &T : ints)
    {}

}

TEST(suiteName, test_forward_iterator)
{
    Kjut::Array<int> ints = {11,22,33,44};

    std::vector<int> copy;
    for(const int &i: ints)
    {
        copy.push_back(i);
    }
    ASSERT_KJUT_ARRAY_AND_STD_VECTOR_EQ(ints, copy);
}

TEST(suiteName, test_forward_iterator_on_empty_array)
{
    Kjut::Array<int> ints;

    std::vector<int> copy;
    for(const int &i: ints)
    {
        copy.push_back(i);
    }

    ASSERT_EQ(copy.size(), 0);
    ASSERT_KJUT_ARRAY_AND_STD_VECTOR_EQ(ints, copy);
}

#ifndef KJUT_DISABLE_CONTAINERS_INITIALIZER_LIST_CONSTRUCTOR
TEST(suiteName, test_list_initializer)
{
    {
        Kjut::Array<int> array = {};
        std::vector<int> expected = {};
        ASSERT_KJUT_ARRAY_AND_STD_VECTOR_EQ(array, expected);
    }
    {
        Kjut::Array<int, 3> array = {1,2,3};
        std::vector<int> expected = {1,2,3};
        ASSERT_KJUT_ARRAY_AND_STD_VECTOR_EQ(array, expected);
    }
    {
        Kjut::Array<int, 3> array = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
        std::vector<int> expected = {1,2,3};
        ASSERT_KJUT_ARRAY_AND_STD_VECTOR_EQ(array, expected);
    }
}
#endif





TEST(suiteName, test_equals_operator_simple_same_size)
{
    {
        Kjut::Array<int> a = {};
        Kjut::Array<int> b = {};
        ASSERT_EQ(a,b);
    }

    {
        Kjut::Array<int> a = {1};
        Kjut::Array<int> b = {1};
        ASSERT_EQ(a,b);
    }

    {
        Kjut::Array<int> a = {1,2};
        Kjut::Array<int> b = {1,2};
        ASSERT_EQ(a,b);
    }
    {
        Kjut::Array<int> a = {1,2,3};
        Kjut::Array<int> b = {1,2,3};
        ASSERT_EQ(a,b);
    }
}



TEST(suiteName, test_equals_operator_simple_fixed_and_dynamic_capacity)
{
    {
        Kjut::Array<int> a = {};
        Kjut::Array<int, 3> b = {};
        ASSERT_EQ(a,b);
    }

    {
        Kjut::Array<int> a = {1};
        Kjut::Array<int, 3> b = {1};
        ASSERT_EQ(a,b);
    }

    {
        Kjut::Array<int> a = {1,2};
        Kjut::Array<int, 3> b = {1,2};
        ASSERT_EQ(a,b);
    }
    {
        Kjut::Array<int> a = {1,2,3};
        Kjut::Array<int, 3> b = {1,2,3};
        ASSERT_EQ(a,b);
    }
}


TEST(suiteName, test_equals_operator_simple_different_capacity_different_content)
{
    {
        Kjut::Array<int> a = {};
        Kjut::Array<int, 3> b = {1};
        ASSERT_NE(a,b);
    }

    {
        Kjut::Array<int, 7> a = {1};
        Kjut::Array<int> b = {};
        ASSERT_NE(a,b);
    }

    {
        Kjut::Array<int> a = {1,0};
        Kjut::Array<int, 3> b = {1,2};
        ASSERT_NE(a,b);
    }
    {
        Kjut::Array<int, 7> a = {1,2};
        Kjut::Array<int> b = {1,0};
        ASSERT_NE(a,b);
    }
}

TEST(suiteName, test_equals_operator_simple_same_capacity_different_content)
{
    {
        Kjut::Array<int> a = {};
        Kjut::Array<int> b = {1};
        ASSERT_NE(a,b);
    }

    {
        Kjut::Array<int> a = {1};
        Kjut::Array<int> b = {};
        ASSERT_NE(a,b);
    }

    {
        Kjut::Array<int> a = {1,0};
        Kjut::Array<int> b = {1,2};
        ASSERT_NE(a,b);
    }
    {
        Kjut::Array<int> a = {1,2};
        Kjut::Array<int> b = {1,0};
        ASSERT_NE(a,b);
    }
}


class ThrowDuringDesctuctors
{
public:

    ThrowDuringDesctuctors() { this->value = -1; shouldThrow = false; }
    ThrowDuringDesctuctors(int value) { this->value = value; shouldThrow = false; }
    ~ThrowDuringDesctuctors() noexcept(false) { if(shouldThrow) { throw std::string("Throw, throw, throw"); }}

    int value;
    bool shouldThrow = false;

};

TEST(suiteName, test_exceptions_thrown_in_T_descructors_are_handled_in_remove)
{
    Kjut::Array<ThrowDuringDesctuctors> a;
    a.append(ThrowDuringDesctuctors(11));
    a.append(ThrowDuringDesctuctors(22));
    a.append(ThrowDuringDesctuctors(33));

    a[0].shouldThrow = true;

    ASSERT_THROW(a.remove(0), std::string);

    ASSERT_EQ(a.size(), 2);
    ASSERT_EQ(a[0].value, 22);
    ASSERT_EQ(a[1].value, 33);
}
