#ifndef ARRAY_HELPERS_H
#define ARRAY_HELPERS_H


#define ASSERT_LOG_COUNT_TRIGGERED(expression, expectedCount) {\
const auto oldCount = mockLogEntryHandlerInvocationCount(); \
    expression; \
    const auto newCount = mockLogEntryHandlerInvocationCount(); \
    ASSERT_EQ( (expectedCount) , (newCount - oldCount)); \
};

#define ASSERT_SILICA_ARRAY_AND_STD_VECTOR_EQ(silicaarray, stdvector) \
{\
        const bool areEqual = (silicaarray == stdvector ); \
        if(!areEqual) \
    { \
            std::cout << "Arrays \n\tSilica::Array: " << silicaarray << "\n\tstd::vector: " << stdvector << "\nDiffers." << std::endl; \
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



template <typename T>
std::ostream &operator<<(std::ostream &os, Silica::Array<T> &a) {
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
bool operator==(const Silica::Array<T> &lhs, const std::vector<T> &rhs)
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
