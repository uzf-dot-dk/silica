#include <gtest/gtest.h>
#include <kjut/ByteBuffer.h>


namespace Kjut
{

}

#define suiteName tst_byte_buffer


TEST(suiteName, test_basic_reading_with_dynamic_array_as_source)
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

TEST(suiteName, test_basic_writing_one_byte_at_a_time_with_dynamic_array_as_source)
{
    Kjut::Array<Kjut::Byte> notUsed;
    Kjut::Array<Kjut::Byte> destination;

    Kjut::ByteBuffer theBuffer(&notUsed, &destination);
    Kjut::IODevice* iod = &theBuffer;

    Kjut::Array<Kjut::Byte> expected = {'S', 'o', 'm', 'e', ' ', 'd', 'a', 't','a'};
    for(unsigned char c: expected)
    {
        iod->writeByte(c);
    }

    ASSERT_EQ(destination,  expected);
}

TEST(suiteName, test_basic_writing_an_array_with_dynamic_array_as_source)
{
    Kjut::Array<Kjut::Byte> notUsed;
    Kjut::Array<Kjut::Byte> destination;

    Kjut::ByteBuffer theBuffer(&notUsed, &destination);
    Kjut::IODevice* iod = &theBuffer;

    Kjut::Array<Kjut::Byte> expected = {'S', 'o', 'm', 'e', ' ', 'd', 'a', 't','a'};
    iod->writeArray(&expected);
    ASSERT_EQ(destination,  expected);
}



