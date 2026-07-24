#ifndef SILICA_BYTE_ARRAY_H
#define SILICA_BYTE_ARRAY_H

#include <silica/Array.h>

namespace Silica
{

/** \brief ByteArray is a byte-oriented Array<unsigned char, S>.
 *
 * ByteArray provides the implemented byte-storage behavior used throughout Silica:
 * - `constData()` for direct read access to the internal buffer
 * - `setData()` to replace the current contents from a raw byte pointer
 * - `appendData()` to append raw bytes to the existing contents
 *
 * The capacity behavior comes from Array<unsigned char, S>:
 * - `S == 0` gives a dynamically growing byte array
 * - `S > 0` gives a fixed-capacity byte array
 */
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


    size_t setData( const char *data, size_t count)
    {
        return setData(reinterpret_cast<const unsigned char*>(data), count);
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

} // namespace Silica

#endif
