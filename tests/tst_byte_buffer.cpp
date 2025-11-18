#include <gtest/gtest.h>
#include <silica/Application.h>
#include <silica/ByteBuffer.h>

#include "SignalSpy.h"

#define suiteName tst_byte_buffer

Silica::Application app;

TEST(suiteName, test_basic_reading_with_dynamic_array_as_source)
{
    Silica::Array<Silica::Byte> readSource = {'W', 'a', 's', 's', 'u', 'p'};
    Silica::ByteBuffer theBuffer(&readSource);
    Silica::IODevice* iod = &theBuffer;


    Silica::Array<Silica::Byte> actuallyRead;
    while(iod->canReadMore())
    {
        actuallyRead.append(iod->read());
    }

    ASSERT_EQ(actuallyRead,  readSource);
}

TEST(suiteName, test_basic_writing_one_byte_at_a_time_with_dynamic_array_as_source)
{
    Silica::Array<Silica::Byte> notUsed;
    Silica::Array<Silica::Byte> destination;

    Silica::ByteBuffer theBuffer(&notUsed, &destination);
    Silica::IODevice* iod = &theBuffer;
    SignalSpy<Silica::IODevice*, size_t> writeWatcher;
    writeWatcher.spyOn(&(iod->writeComplete));

    Silica::Array<Silica::Byte> expected = {'S', 'o', 'm', 'e', ' ', 'd', 'a', 't','a'};
    for(unsigned char c: expected)
    {
        iod->writeByte(c);
    }

    ASSERT_EQ(destination,  expected);
    ASSERT_EQ(writeWatcher.invocationCount(), expected.size());
    for(auto invocation : writeWatcher.invocations())
    {
        size_t byteCount = std::get<size_t>(invocation);
        Silica::IODevice * emitter = std::get<Silica::IODevice *>(invocation);
        ASSERT_EQ(byteCount, 1);
        ASSERT_EQ(emitter, iod);
    }
}

TEST(suiteName, test_basic_writing_an_array_with_dynamic_array_as_source)
{
    Silica::Array<Silica::Byte> notUsed;
    Silica::Array<Silica::Byte> destination;

    Silica::ByteBuffer theBuffer(&notUsed, &destination);
    Silica::IODevice* iod = &theBuffer;
    SignalSpy<Silica::IODevice*, size_t> writeWatcher;
    writeWatcher.spyOn(&(iod->writeComplete));

    Silica::Array<Silica::Byte> expected = {'S', 'o', 'm', 'e', ' ', 'd', 'a', 't','a'};
    iod->writeArray(&expected);

    ASSERT_EQ(destination,  expected);

    ASSERT_EQ(writeWatcher.invocationCount(), 1);
    auto invocation = writeWatcher.invocations()[0];
    size_t byteCount = std::get<size_t>(invocation);
    Silica::IODevice * emitter = std::get<Silica::IODevice *>(invocation);
    ASSERT_EQ(byteCount, expected.size());
    ASSERT_EQ(emitter, iod);
}




TEST(suiteName, test_basic_reading_with_static_array_as_source)
{
    Silica::Array<Silica::Byte, 6> readSource = {'W', 'a', 's', 's', 'u', 'p'};
    Silica::ByteBuffer theBuffer(&readSource);
    Silica::IODevice* iod = &theBuffer;


    Silica::Array<Silica::Byte, 6> actuallyRead;
    while(iod->canReadMore())
    {
        actuallyRead.append(iod->read());
    }

    ASSERT_EQ(actuallyRead,  readSource);
}

TEST(suiteName, test_basic_writing_one_byte_at_a_time_with_static_array_as_source)
{
    Silica::Array<Silica::Byte, 20> notUsed;
    Silica::Array<Silica::Byte, 20> destination;

    Silica::ByteBuffer theBuffer(&notUsed, &destination);
    Silica::IODevice* iod = &theBuffer;
    SignalSpy<Silica::IODevice*, size_t> writeWatcher;
    writeWatcher.spyOn(&(iod->writeComplete));

    Silica::Array<Silica::Byte, 20> expected = {'S', 'o', 'm', 'e', ' ', 'd', 'a', 't','a'};
    for(unsigned char c: expected)
    {
        iod->writeByte(c);
    }

    ASSERT_EQ(destination,  expected);
    ASSERT_EQ(writeWatcher.invocationCount(), expected.size());
    for(auto invocation : writeWatcher.invocations())
    {
        size_t byteCount = std::get<size_t>(invocation);
        Silica::IODevice * emitter = std::get<Silica::IODevice *>(invocation);
        ASSERT_EQ(byteCount, 1);
        ASSERT_EQ(emitter, iod);
    }
}

