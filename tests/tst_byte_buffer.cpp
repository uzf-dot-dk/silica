#include <gtest/gtest.h>
#include <kjut/ByteBuffer.h>


namespace Kjut
{

}

#define suiteName tst_byte_buffer


TEST(suiteName, test_something)
{
    Kjut::Array<Kjut::Byte> readSource = {'W', 'a', 's', 's', 'u', 'p'};
    Kjut::ByteBuffer theBuffer(&readSource);
    Kjut::IODevice* iod = &theBuffer;


    Kjut::Array<Kjut::Byte> actuallyRead;
    while(iod->canReadMore())
    {
        actuallyRead.append(iod->read());
    }

    ASSERT_EQ(actuallyRead,  readSource);
}

