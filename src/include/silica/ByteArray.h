#ifndef SILICA_BYTE_ARRAY_H
#define SILICA_BYTE_ARRAY_H

#include <silica/Array.h>

namespace Kjut
{


template <size_t S = 0>
class ByteArray : public Array<unsigned char, S> {
public:
    using Base = Array<unsigned char, S>;
    using Base::Base;  // inherit constructors if you add any later

    const unsigned char* constData() const { return this->d.data; }

    size_t setData( const unsigned char *data, size_t count)
    {
        this->clear();
        size_t bytes_appended = 0;

        for(size_t i = 0; i < count; i++)
        {
            if(this->append(data[i]))
            {
                bytes_appended++;
            }
            else
            {
                break;
            }
        }
        return bytes_appended;
    }

    size_t appendData( const unsigned char *data, size_t count)
    {
        size_t bytes_appended = 0;

        for(size_t i = 0; i < count; i++)
        {
            if(this->append(data[i]))
            {
                bytes_appended++;
            }
            else
            {
                break;
            }
        }
        return bytes_appended;
    }

};

} // namespace Kjut

#endif
