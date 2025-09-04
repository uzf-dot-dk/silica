#ifndef KJUT_BYTE_ARRAY_H
#define KJUT_BYTE_ARRAY_H

#include <kjut/Array.h>

namespace Kjut
{


/** \brief An sequence of chars that provide the safety and convenience of an Array, but allows access to the ras `char *` as well.*/
template <size_t S = 0>
class ByteArray : public Array<char, S>
{
public:

    ByteArray()
        : Array<char, S>()
    {}

    ByteArray(const char *data, size_t count)
    {
        for(size_t i = 0; i < count; i++)
        {
            this->append(data[i]);
        }
    }

    ~ByteArray()
    {

    }

    /** \brief Returns a `const char*` to a continous array of the entire content.

     For example:
     ```cpp
    Kjut::ByteArray ba;
    ba.append('F');
    ba.append('O');
    ba.append('O');
    ba.append('\0');

    ASSERT_EQ(ba.size(), 4);
    ASSERT_STREQ(ba.constData(), "FOO");
    ```

    Notice, that the value 0 carries no special meaning for a ByteArray, and as such adds one to the size.
    */

    const char *constData()
    {
        return &(this->at(0));
    }

};

}

#endif // KJUT_BYTE_ARRAY_H
