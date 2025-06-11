#ifndef ARRAY_HELPERS_H
#define ARRAY_HELPERS_H


#define ASSERT_LOG_COUNT_TRIGGERED(expression, expectedCount) {\
const auto oldCount = mockLogEntryHandlerInvocationCount(); \
    expression; \
    const auto newCount = mockLogEntryHandlerInvocationCount(); \
    ASSERT_EQ( (expectedCount) , (newCount - oldCount)); \
};

#define ASSERT_KJUT_ARRAY_AND_STD_VECTOR_EQ(kjutarray, stdvector) \
{\
        const bool areEqual = (kjutarray == stdvector ); \
        if(!areEqual) \
    { \
            std::cout << "Arrays \n\tKjut::Array: " << kjutarray << "\n\tstd::vector: " << stdvector << "\nDiffers." << std::endl; \
            FAIL(); \
    }\
}


template <typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> &a) {
    os << "[";
    for(size_t i = 0; i < a.size(); i++)
    {
        if(i > 0)
        {
            os << ", ";
        }
        os << a[i];
    }
    os << "]";
    return os;
}



#include <vector>
template <typename T>
bool operator==(const Kjut::Array<T> &lhs, const std::vector<T> &rhs)
{
    if(lhs.size() != rhs.size())
    {
        return false;
    }

    for(size_t i = 0; i < lhs.size(); i++)
    {
        if( lhs.at(i) != rhs[i])
        {
            return false;
        }
    }
    return true;
}

#endif // ARRAY_HELPERS_H
