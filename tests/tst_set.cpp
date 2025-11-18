#include <gtest/gtest.h>

#define SILICA_ARRAY_INITIAL_CAPACITY 1
#include <set>
#include <silica/Set.h>

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
    //Silica::registerLoghandler(mockLogEntryHandler);
    mockCount = 0;
}

void resetMockLogEntryCount()
{
    mockCount = 0;
}


#include <set>

namespace Silica
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


std::ostream &operator<<(std::ostream &os, const Silica::Set<int> &a) {
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

    Silica::Set<int> s;
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

    Silica::Set<int> s;
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
        Silica::Set<int> set;
        for(auto input : inputs) {
            set.insert(input);
        }
        ASSERT_EQ(set, expected);
    }

}

TEST(suiteName, test_equality_operator)
{
    {
        Silica::Set<int> s;
        s.insert(1);
        Silica::Set<int> s2;
        s2.insert(1);

        ASSERT_TRUE(s == s2);
    }


    {
        Silica::Set<int> s;
        s.insert(1);
        Silica::Set<int> s2;
        s2.insert(2);

        ASSERT_FALSE(s == s2);
    }
}



TEST(suiteName, test_clear)
{
    Silica::Set<int> s;
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

#define CREATE_SET(name, index) Silica::Set<int> name; { std::vector<int> stdv = std::get<index>(testCase); for(auto e: stdv) { name.insert(e); } }

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

            {{},          "intersection", {},         "results in", {}},
            {{},          "intersection", {4,5,6},     "results in", {}},
            {{1,2,3},     "intersection", {},          "results in", {}},
            {{1,2,3},     "intersection", {4,5,6},     "results in", {}},
            {{1,2,3},     "intersection", {3,4,5},     "results in", {3}},
            {{1,2,3},     "intersection", {2,3,4,5},   "results in", {2,3}},
            {{1,2,3,5},   "intersection", {1,2,3,4,5}, "results in", {1,2,3,5}},
            {{1,2,3,4,5}, "intersection", {1,2,3,4,5}, "results in", {1,2,3,4,5}},

            {{},          "difference", {},      "results in", {}},
            {{},          "difference", {1,2,3}, "results in", {}},
            {{1,2,3},     "difference", {},      "results in", {1,2,3}},
            {{1,2,3},     "difference", {1,2,3}, "results in", {}},
            {{1,2,3,4},   "difference", {1,2,3}, "results in", {4}},
            {{1,2,3,4,5}, "difference", {1,2,3}, "results in", {4,5}},

            {{},    "symmetric difference", {},     "results in", {}},
            {{1},   "symmetric difference", {},     "results in", {1}},
            {{},    "symmetric difference", {1},    "results in", {1}},
            {{1},   "symmetric difference", {1},    "results in", {}},
            {{1,2}, "symmetric difference", {2},    "results in", {1}},
            {{2},   "symmetric difference", {1,2},  "results in", {1}},

            };

    for( auto & testCase : cases)
    {
        CREATE_SET(A, 0);
        CREATE_SET(B, 2);
        CREATE_SET(expected, 4);
        std::string operation = std::get<1>(testCase);
        Silica::Set<int> result;
        bool wasOperationSuccessful = false;
        if(operation == "union")
        {
            wasOperationSuccessful = A.unionWith(B, result);
        }
        else if(operation == "intersection")
        {
            wasOperationSuccessful = A.intersectionWith(B, result);
        }
        else if(operation == "difference")
        {
            wasOperationSuccessful = A.differenceFrom(B, result);
        }
        else if(operation == "symmetric difference")
        {
            wasOperationSuccessful = A.symmetricDifference(B, result);
        }
        else
        {   std::cout << "'" << operation << "' is not a tested operation." << std::endl;
            FAIL();
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

#undef CREATE_SET

#define CREATE_SET(name, index) Silica::Set<int> name; { std::vector<int> stdv = std::get<index>(testCase); for(auto e: stdv) { name.insert(e); } }

TEST(suiteName, test_set_deductors)
{
    std::vector<
        std::tuple<
            std::vector<int>,       // A
            std::string,            // check
            std::vector<int>,       // B
            bool                    //expected
            >
        > cases = {
            {{},      "is subset of", {}, true},
            {{},      "is subset of", {1,2,3}, true},
            {{1,2,3}, "is subset of", {1,2,3}, true},
            {{1,2,3}, "is subset of", {1,2}, false},
            {{1,2,3}, "is subset of", {1}, false},
            {{1,2,3}, "is subset of", {0}, false},
            {{2},     "is subset of", {1}, false},

            {{},        "is proper subset of", {}, false},
            {{},        "is proper subset of", {1}, true},
            {{1},       "is proper subset of", {1}, false},
            {{1},       "is proper subset of", {1,2}, true},
            {{1,2,3,4}, "is proper subset of", {1,2,3,4}, false},
            {{1,2,3,4}, "is proper subset of", {1,2,3,4,5}, true},


            {{},      "is superset of", {}, true},
            {{1,2,3}, "is superset of", {}, true},
            {{1,2,3}, "is superset of", {1,2,3}, true},
            {{1,2},   "is superset of", {1,2,3}, false},
            {{1},     "is superset of", {1,2,3}, false},
            {{},      "is superset of", {1,2,3}, false},
            {{1},     "is superset of", {2}, false},

        };
    for( auto & testCase : cases)
    {
        CREATE_SET(A, 0);
        const std::string check = std::get<1>(testCase);
        CREATE_SET(B, 2);
        const bool expected = std::get<3>(testCase);
        bool actual;
        if( check  == "is subset of" )
        {
            actual = A.isSubsetOf(B);
        }
        else if( check  == "is proper subset of" )
        {
            actual = A.isProperSubsetOf(B);
        }
        else if( check  == "is superset of" )
        {
            actual = A.isSupersetOf(B);
        }
        else
        {
            std::cout << "'" << check << "' is not a tested check." << std::endl;
            FAIL();
        }

        const bool expectedEqualsActual = (expected == actual);

        if(!expectedEqualsActual)
        {
            std::cout << "A         " << A << std::endl;
            std::cout << "B         " << B << std::endl;
            std::cout << "Check     " << "'" << check << "'" << std::endl;
            std::cout << "Expected  " << (expected ? "True" : "False" )<< std::endl;
            std::cout << "Actual    " << (actual ? "True" : "False" ) << std::endl;
            std::cout << "Equals?   " << (expectedEqualsActual ? "True" : "False") << std::endl;
            std::cout << "------------------------------" << std::endl;
        }
        ASSERT_TRUE(expectedEqualsActual);
    }
}

#undef CREATE_SET

TEST(suiteName, test_erase)
{
    registerMockLogEntryHandler();

    Silica::Set<int> s;
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
