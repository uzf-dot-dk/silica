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

TEST(suiteName, test_basic_reading_from_raw_byte_array_source)
{
    const unsigned char rawSource[] = {'R', 'a', 'w', '\n', 'D', 'a', 't', 'a'};
    Silica::ByteBuffer theBuffer(rawSource, sizeof(rawSource));
    Silica::IODevice * iod = &theBuffer;

    ASSERT_TRUE(iod->isReadable());
    ASSERT_FALSE(iod->isWritable());
    ASSERT_EQ(iod->bytesAvailable(), sizeof(rawSource));
    ASSERT_TRUE(iod->canReadLine());

    Silica::ByteArray<> actuallyRead;
    while(iod->canReadMore())
    {
        actuallyRead.append(iod->read());
    }

    Silica::ByteArray<> expected = {'R', 'a', 'w', '\n', 'D', 'a', 't', 'a'};
    ASSERT_EQ(actuallyRead, expected);
    ASSERT_TRUE(iod->atEnd());
}

TEST(suiteName, test_read_into_byte_array_consumes_entire_source)
{
    Silica::ByteArray<> readSource = {'A', 'l', 'p', 'h', 'a'};
    Silica::ByteBuffer buffer(&readSource);
    Silica::ByteArray<> destination;

    ASSERT_EQ(buffer.read(&destination), readSource.size());
    ASSERT_EQ(destination, readSource);
    ASSERT_TRUE(buffer.atEnd());
}

TEST(suiteName, test_read_line_consumes_through_newline)
{
    Silica::ByteArray<> readSource = {'F', 'i', 'r', 's', 't', '\n', 'S', 'e', 'c', 'o', 'n', 'd'};
    Silica::ByteBuffer buffer(&readSource);
    Silica::ByteArray<> destination;

    ASSERT_EQ(buffer.readLine(&destination), 6u);
    ASSERT_EQ(destination, (Silica::ByteArray<>{'F', 'i', 'r', 's', 't', '\n'}));
    ASSERT_FALSE(buffer.atEnd());

    ASSERT_EQ(buffer.readLine(&destination), 6u);
    ASSERT_EQ(destination, (Silica::ByteArray<>{'F', 'i', 'r', 's', 't', '\n', 'S', 'e', 'c', 'o', 'n', 'd'}));
    ASSERT_TRUE(buffer.atEnd());
}

TEST(suiteName, test_read_line_from_raw_source_without_newline_reads_to_end)
{
    const unsigned char rawSource[] = {'N', 'o', 'N', 'e', 'w', 'l', 'i', 'n', 'e'};
    Silica::ByteBuffer buffer(rawSource, sizeof(rawSource));
    Silica::ByteArray<> destination;

    ASSERT_FALSE(buffer.canReadLine());
    ASSERT_EQ(buffer.readLine(&destination), sizeof(rawSource));
    ASSERT_EQ(destination, (Silica::ByteArray<>{'N', 'o', 'N', 'e', 'w', 'l', 'i', 'n', 'e'}));
    ASSERT_TRUE(buffer.atEnd());
}
