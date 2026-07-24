#include <gtest/gtest.h>
#include <silica/Application.h>
#include <silica/ByteBuffer.h>

#include "SignalSpy.h"

#define suiteName tst_byte_buffer

Silica::Application app;

TEST(suiteName, test_basic_reading_with_dynamic_array_as_source)
{
    Silica::ByteArray<> readSource = {'W', 'a', 's', 's', 'u', 'p'};
    Silica::ByteBuffer theBuffer(&readSource);
    Silica::IODevice* iod = &theBuffer;


    Silica::ByteArray<> actuallyRead;
    while(iod->canReadMore())
    {
        actuallyRead.append(iod->read());
    }

    ASSERT_EQ(actuallyRead,  readSource);
}

TEST(suiteName, test_basic_writing_one_byte_at_a_time_with_dynamic_array_as_source)
{
    Silica::ByteArray<> notUsed;
    Silica::ByteArray<> destination;

    Silica::ByteBuffer theBuffer(&notUsed, &destination);
    Silica::IODevice* iod = &theBuffer;
    SignalSpy<Silica::IODevice*, size_t> writeWatcher;
    writeWatcher.spyOn(&(iod->writeComplete));

    Silica::ByteArray<> expected = {'S', 'o', 'm', 'e', ' ', 'd', 'a', 't','a'};
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
    Silica::ByteArray<> notUsed;
    Silica::ByteArray<> destination;

    Silica::ByteBuffer theBuffer(&notUsed, &destination);
    Silica::IODevice* iod = &theBuffer;
    SignalSpy<Silica::IODevice*, size_t> writeWatcher;
    writeWatcher.spyOn(&(iod->writeComplete));

    Silica::ByteArray<> expected = {'S', 'o', 'm', 'e', ' ', 'd', 'a', 't','a'};
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
    Silica::ByteArray<> readSource = {'W', 'a', 's', 's', 'u', 'p'};
    Silica::ByteBuffer theBuffer(&readSource);
    Silica::IODevice* iod = &theBuffer;


    Silica::ByteArray<> actuallyRead;
    while(iod->canReadMore())
    {
        actuallyRead.append(iod->read());
    }

    ASSERT_EQ(actuallyRead,  readSource);
}

TEST(suiteName, test_basic_writing_one_byte_at_a_time_with_static_array_as_source)
{
    Silica::ByteArray<> notUsed;
    Silica::ByteArray<> destination;

    Silica::ByteBuffer theBuffer(&notUsed, &destination);
    Silica::IODevice* iod = &theBuffer;
    SignalSpy<Silica::IODevice*, size_t> writeWatcher;
    writeWatcher.spyOn(&(iod->writeComplete));

    Silica::ByteArray<> expected = {'S', 'o', 'm', 'e', ' ', 'd', 'a', 't','a'};
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
