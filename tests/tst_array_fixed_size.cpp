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



TEST(suiteName, test_removal)
{

#define CREATE_ARRAY(name) \
    constexpr size_t CAPACITY = 8; \
        Kjut::Array<int, CAPACITY> name; \
        for(size_t i = 0; i < CAPACITY; i++) \
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

        const bool shouldExpectTrue = (cases[i].size() < CAPACITY);
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
    Kjut::Array<int, 6> ints;

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
        Kjut::Array<int, 3> array = {};
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





TEST(suiteName, test_equals_operator_simple_same_capacity)
{
    {
        Kjut::Array<int, 3> a = {};
        Kjut::Array<int, 3> b = {};
        ASSERT_EQ(a,b);
    }

    {
        Kjut::Array<int, 3> a = {1};
        Kjut::Array<int, 3> b = {1};
        ASSERT_EQ(a,b);
    }

    {
        Kjut::Array<int, 3> a = {1,2};
        Kjut::Array<int, 3> b = {1,2};
        ASSERT_EQ(a,b);
    }
    {
        Kjut::Array<int, 3> a = {1,2,3};
        Kjut::Array<int, 3> b = {1,2,3};
        ASSERT_EQ(a,b);
    }
}



TEST(suiteName, test_equals_operator_simple_different_capacity)
{
    {
        Kjut::Array<int, 7> a = {};
        Kjut::Array<int, 3> b = {};
        ASSERT_EQ(a,b);
    }

    {
        Kjut::Array<int, 7> a = {1};
        Kjut::Array<int, 3> b = {1};
        ASSERT_EQ(a,b);
    }

    {
        Kjut::Array<int, 7> a = {1,2};
        Kjut::Array<int, 3> b = {1,2};
        ASSERT_EQ(a,b);
    }
    {
        Kjut::Array<int, 7> a = {1,2,3};
        Kjut::Array<int, 3> b = {1,2,3};
        ASSERT_EQ(a,b);
    }
}

TEST(suiteName, test_equals_operator_simple_different_capacity_different_content)
{
    {
        Kjut::Array<int, 7> a = {};
        Kjut::Array<int, 3> b = {1};
        ASSERT_NE(a,b);
    }

    {
        Kjut::Array<int, 7> a = {1};
        Kjut::Array<int, 3> b = {};
        ASSERT_NE(a,b);
    }

    {
        Kjut::Array<int, 7> a = {1,0};
        Kjut::Array<int, 3> b = {1,2};
        ASSERT_NE(a,b);
    }
    {
        Kjut::Array<int, 7> a = {1,2};
        Kjut::Array<int, 3> b = {1,0};
        ASSERT_NE(a,b);
    }
}

TEST(suiteName, test_equals_operator_simple_same_capacity_different_content)
{
    {
        Kjut::Array<int, 7> a = {};
        Kjut::Array<int, 7> b = {1};
        ASSERT_NE(a,b);
    }

    {
        Kjut::Array<int, 7> a = {1};
        Kjut::Array<int, 7> b = {};
        ASSERT_NE(a,b);
    }

    {
        Kjut::Array<int, 7> a = {1,0};
        Kjut::Array<int, 7> b = {1,2};
        ASSERT_NE(a,b);
    }
    {
        Kjut::Array<int, 7> a = {1,2};
        Kjut::Array<int, 7> b = {1,0};
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

class AssignableInteger
{

public:
    AssignableInteger() { value = 0; };
    AssignableInteger(int i) { value = i; };
    AssignableInteger& operator=(const AssignableInteger& rhs)
    {
        AssignableInteger::assignedIntvalues.insert(this->value);
        value = rhs.value;
        return *this;
    };

    static std::set<int> assignedIntvalues;
    int value;
};
std::set<int> AssignableInteger::assignedIntvalues;



TEST(suiteName, test_removal_causes_assignment)
{
    Kjut::Array<AssignableInteger, 7> ints = {1,2,3};

    AssignableInteger::assignedIntvalues.clear();
    ints.remove(1);
    ASSERT_EQ(std::set<int>{2}, AssignableInteger::assignedIntvalues);
}



TEST(suiteName, test_fill_method)
{
    registerMockLogEntryHandler();
    Kjut::Array<int, 9> array;
    array.fill(27);
    ASSERT_EQ(array.size(), 9);
    for(size_t i = 0; i < array.size(); i++)
    {
        ASSERT_EQ(array[i], 27);
    }
    ASSERT_EQ(0, mockLogEntryHandlerInvocationCount());
}



TEST(suiteName, test_method_clear)
{
    Kjut::Array<DeletableInteger, 9> array = {11,22,33,44,55,66,77,88,99};
    ASSERT_EQ(array.size(), 9);
    DeletableInteger::deletedIntvalues.clear();
    ASSERT_EQ(DeletableInteger::deletedIntvalues.size(), 0);
    array.clear();
    std::set<int> expected = {11,22,33,44,55,66,77,88,99};
    ASSERT_EQ(DeletableInteger::deletedIntvalues, expected);
    ASSERT_EQ(array.size(), 0);
}


bool operator==(const std::vector<int> &lhs, const Kjut::Array<DeletableInteger> &rhs)
{
    if(rhs.size() != lhs.size())
    {
        return false;
    }
    for(size_t i = 0; i < rhs.size();i++)
    {
        if(rhs[i].value != lhs[i])
        {
            return false;
        }
    }
    return true;
}

bool operator==(const Kjut::Array<DeletableInteger> &lhs, const std::vector<int> &rhs)
{
    return rhs == lhs;
}

TEST(suiteName, test_method_remove_element_at_end_calls_destructor)
{
    Kjut::Array<DeletableInteger, 9> array = {11,22,33,44,55,66,77,88,99};
    DeletableInteger::deletedIntvalues.clear();
    array.remove(8);
    {
        std::vector<int> expected = {11,22,33,44,55,66,77,88};
        ASSERT_EQ(array, expected);
    }

    {
        std::set<int> expected = {99};
        ASSERT_EQ(DeletableInteger::deletedIntvalues, expected);
    }

}
