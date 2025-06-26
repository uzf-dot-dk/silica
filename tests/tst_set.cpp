#include <gtest/gtest.h>

#define KJUT_ARRAY_INITIAL_CAPACITY 1
#include <set>
#include <kjut/Set.h>

#define suiteName tst_set


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


#include <set>

namespace Kjut
{

template <typename T>
bool operator==(const Set<T> &lhs, const std::set<T> &rhs)
{
    if(rhs.size() != lhs.size())
    {
        return false;
    }
    return true;
}
template <typename T>
bool operator==(const std::set<T> &lhs, const Set<T> &rhs)
{
    if(rhs.size() != lhs.size())
    {
        return false;
    }
    return true;
}

template <typename T>
bool operator!=(const Set<T> &lhs, const std::set<T> &rhs)
{
    return ! (lhs == rhs);

}

template <typename T>
bool operator!=(const std::set<T> &lhs, const Set<T> &rhs)
{
    return ! (lhs == rhs);

}

}


template <typename T>
std::ostream &operator<<(std::ostream &os, const std::set<T> &a) {
    os << "(";
    bool first = true;
    for(const auto &element: a)
    {
        if(!first)
        {
            os << ", ";
        }
        os << element;
        first = false;

    }
    os << ")";
    return os;
}


std::ostream &operator<<(std::ostream &os, const Kjut::Set<int> &a) {
    os << "(";
    bool first = true;
    for(const auto &element: a.values())
    {
        if(!first)
        {
            os << ", ";
        }
        os << element;
        first = false;

    }
    os << ")";
    return os;
}





TEST(suiteName, test_default_constructor)
{
    registerMockLogEntryHandler();

    Kjut::Set<int> s;
    ASSERT_EQ(s.size(), 0);
    for(int candidate = -100; candidate < 100; candidate++)
    {
        ASSERT_FALSE(s.contains(candidate));
    }

    ASSERT_EQ(0, mockLogEntryHandlerInvocationCount());
}

TEST(suiteName, test_simple_insert)
{
    registerMockLogEntryHandler();

    Kjut::Set<int> s;
    s.insert(1);
    s.insert(1);
    s.insert(1);
    s.insert(1);
    s.insert(1);
    s.insert(1);
    s.insert(1);
    s.insert(1);
    ASSERT_EQ(s.size(), 1);
    for(int candidate = -100; candidate < 100; candidate++)
    {
        if(candidate == 1)
        {
            ASSERT_TRUE(s.contains(candidate));
        }
        else
        {
            ASSERT_FALSE(s.contains(candidate));
        }
    }

    ASSERT_EQ(0, mockLogEntryHandlerInvocationCount());
}

TEST(suiteName, test_insert)
{
    std::map<std::vector<int>, std::set<int>> cases = {
        {{}, {}},
        {{2,2,2,2,3,4,4,3,2,3}, {2,3,4}},
        {{4}, {4}},
    };

    for(const auto &[inputs, expected] : cases)
    {
        Kjut::Set<int> set;
        for(auto input : inputs) {
            set.insert(input);
        }
        ASSERT_EQ(set, expected);
    }

}

TEST(suiteName, test_equality_operator)
{
    {
        Kjut::Set<int> s;
        s.insert(1);
        Kjut::Set<int> s2;
        s2.insert(1);

        ASSERT_TRUE(s == s2);
    }


    {
        Kjut::Set<int> s;
        s.insert(1);
        Kjut::Set<int> s2;
        s2.insert(2);

        ASSERT_FALSE(s == s2);
    }
}



TEST(suiteName, test_clear)
{
    Kjut::Set<int> s;
    s.insert(1);
    s.insert(2);
    s.insert(3);

    s.clear();

    ASSERT_EQ(s.size(), 0);

    ASSERT_FALSE(s.contains(1));
    ASSERT_FALSE(s.contains(2));
    ASSERT_FALSE(s.contains(3));
}


#include <vector>

#define CREATE_SET(name, index) Kjut::Set<int> name; { std::vector<int> stdv = std::get<index>(testCase); for(auto e: stdv) { name.insert(e); } }

TEST(suiteName, test_set_operations)
{
    std::vector<
        std::tuple<
            std::vector<int>,       // A
            std::string,            //operator
            std::vector<int>,       // B
            std::string,            //Filler just for readability
            std::vector<int>> >     // Expected
        cases = {
            {{1,2,3}, "union", {1,2,3}, "results in", {1,2,3}},
            {{1,2,3}, "union", {},      "results in", {1,2,3}},
            {{},      "union", {1,2,3}, "results in", {1,2,3}},
            {{1,2,3}, "union", {2,3,4}, "results in", {1,2,3,4}},
            {{1,2,3}, "union", {4,5,6}, "results in", {1,2,3,4,5,6}},

            {{}, "intersection", {}, "results in", {}},
            {{}, "intersection", {4,5,6}, "results in", {}},
            {{1,2,3}, "intersection", {}, "results in", {}},
            {{1,2,3}, "intersection", {4,5,6}, "results in", {}},
            {{1,2,3}, "intersection", {3,4,5}, "results in", {3}},
            {{1,2,3}, "intersection", {2,3,4,5}, "results in", {2,3}},
            {{1,2,3,5}, "intersection", {1,2,3,4,5}, "results in", {1,2,3,5}},
            {{1,2,3,4,5}, "intersection", {1,2,3,4,5}, "results in", {1,2,3,4,5}},
        };

    for( auto & testCase : cases)
    {
        CREATE_SET(A, 0);
        CREATE_SET(B, 2);
        CREATE_SET(expected, 4);
        std::string operation = std::get<1>(testCase);
        Kjut::Set<int> result;
        bool wasOperationSuccessful = false;
        if(operation == "union")
        {
            wasOperationSuccessful = A.unionWith(B, result);
        }
        else if(operation == "intersection")
        {
            wasOperationSuccessful = A.intersectionWith(B, result);
        }
        else
        {
        }
        const bool expectedEqualsActual = (result == expected);
        ASSERT_TRUE(wasOperationSuccessful);
        if(!expectedEqualsActual)
        {
            std::cout << "A         " << A << std::endl;
            std::cout << "B         " << B << std::endl;
            std::cout << "Operation " << operation << std::endl;
            std::cout << "Expected  " << expected << std::endl;
            std::cout << "Actual    " << result<< std::endl;
            std::cout << "Equals?   " << (result == expected ? "true" : "false") << std::endl;
            std::cout << "------------------------------" << std::endl;
        }
        ASSERT_TRUE(expectedEqualsActual);
    }


}



TEST(suiteName, test_erase)
{
    registerMockLogEntryHandler();

    Kjut::Set<int> s;
    s.insert(1);
    s.erase(1);
    ASSERT_FALSE(s.contains(1));
    s.erase(1);
    s.erase(1);
    s.erase(1);
    s.erase(1);
    s.erase(1);
    s.erase(1);
    s.erase(1);
    s.erase(1);
    s.erase(1);
    s.erase(1);
    s.erase(1);
    s.erase(1);
    ASSERT_FALSE(s.contains(1));
    ASSERT_EQ(0, mockLogEntryHandlerInvocationCount());
}
