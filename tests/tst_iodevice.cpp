#include <gtest/gtest.h>

#include <silica/Application.h>
#include <silica/ByteArray.h>
#include <silica/IODevice.h>

#include "SignalSpy.h"

#define suiteName tst_iodevice

Silica::Application app;

class TestIODevice : public Silica::IODevice
{
public:
    TestIODevice()
    {
        setCurrentOpenMode(OpenMode::Closed);
    }

    bool hasRandomAccess() const override
    {
        return d.hasRandomAccess;
    }

    bool atEnd() const override
    {
        return d.atEnd;
    }

    size_t bytesAvailable() const override
    {
        return d.bytesAvailableValue;
    }

    bool canReadLine() const override
    {
        return d.canReadLine;
    }

    size_t readLine(Silica::ByteArray<> *destination) override
    {
        d.readLineCalls++;
        if(destination)
        {
            destination->append('L');
            destination->append('N');
        }
        return 2;
    }

    bool isWritable() const override
    {
        return d.isWritable;
    }

    Silica::Byte read() override
    {
        d.readCalls++;
        return d.nextByte++;
    }

    size_t read(Silica::ByteArray<> *destination) override
    {
        d.readArrayCalls++;
        if(destination)
        {
            destination->append('A');
            destination->append('R');
            destination->append('R');
        }
        return 3;
    }

    int openCalls() const { return d.openCalls; }
    int closeCalls() const { return d.closeCalls; }
    int writeByteCalls() const { return d.writeByteCalls; }
    int writeArrayCalls() const { return d.writeArrayCalls; }
    int readCalls() const { return d.readCalls; }
    int readArrayCalls() const { return d.readArrayCalls; }
    int readLineCalls() const { return d.readLineCalls; }

    void setReadableState(bool readable, bool atEndState)
    {
        d.isWritable = false;
        d.hasRandomAccess = true;
        d.atEnd = atEndState;
        if(readable)
        {
            setCurrentOpenMode(OpenMode::ReadOnly);
        }
        else
        {
            setCurrentOpenMode(OpenMode::WriteOnly);
        }
    }

    void setWritableState(bool writable)
    {
        d.isWritable = writable;
    }

protected:
    void closeImplementation() override
    {
        d.closeCalls++;
        setCurrentOpenMode(OpenMode::Closed);
        emit closed(this, CloseReason{});
    }

    void openImplementation(OpenMode mode) override
    {
        d.openCalls++;
        setCurrentOpenMode(mode);
        emit opened(this);
    }

    void writeArrayImplementation(Silica::ByteArray<> *data) override
    {
        d.writeArrayCalls++;
        const size_t bytesWritten = data ? data->size() : 0;
        emit writeComplete(this, bytesWritten);
    }

    void writeByteImplementation(Silica::Byte) override
    {
        d.writeByteCalls++;
        emit writeComplete(this, 1);
    }

private:
    struct
    {
        bool hasRandomAccess = true;
        bool atEnd = false;
        bool canReadLine = false;
        bool isWritable = true;
        size_t bytesAvailableValue = 0;
        Silica::Byte nextByte = 0;
        int openCalls = 0;
        int closeCalls = 0;
        int writeByteCalls = 0;
        int writeArrayCalls = 0;
        int readCalls = 0;
        int readArrayCalls = 0;
        int readLineCalls = 0;
    } d;
};

TEST(suiteName, constructor_initializes_closed_and_slots_dispatch)
{
    TestIODevice device;

    ASSERT_EQ(device.currentOpenMode(), Silica::IODevice::OpenMode::Closed);

    SignalSpy<Silica::IODevice*> openedSpy;
    openedSpy.spyOn(&device.opened);

    SignalSpy<Silica::IODevice*, Silica::IODevice::CloseReason> closedSpy;
    closedSpy.spyOn(&device.closed);

    SignalSpy<Silica::IODevice*, size_t> writeSpy;
    writeSpy.spyOn(&device.writeComplete);

    device.open(Silica::IODevice::OpenMode::ReadOnly);
    ASSERT_EQ(device.currentOpenMode(), Silica::IODevice::OpenMode::ReadOnly);
    ASSERT_EQ(device.openCalls(), 1);
    ASSERT_EQ(openedSpy.invocationCount(), 1);

    Silica::ByteArray<> destination;
    device.writeByte(7);
    device.writeArray(&destination);
    ASSERT_EQ(device.writeByteCalls(), 1);
    ASSERT_EQ(device.writeArrayCalls(), 1);
    ASSERT_EQ(writeSpy.invocationCount(), 2);

    device.close();
    ASSERT_EQ(device.currentOpenMode(), Silica::IODevice::OpenMode::Closed);
    ASSERT_EQ(device.closeCalls(), 1);
    ASSERT_EQ(closedSpy.invocationCount(), 1);
}

TEST(suiteName, canReadMore_reflects_open_mode_and_at_end_state)
{
    TestIODevice device;

    device.setReadableState(true, false);
    ASSERT_TRUE(device.isReadable());
    ASSERT_TRUE(device.canReadMore());

    device.setReadableState(true, true);
    ASSERT_FALSE(device.canReadMore());

    device.setReadableState(false, false);
    ASSERT_FALSE(device.isReadable());
    ASSERT_FALSE(device.canReadMore());
}

TEST(suiteName, read_helpers_delegate_to_implementation)
{
    TestIODevice device;
    device.setReadableState(true, false);

    Silica::ByteArray<> destination;

    ASSERT_EQ(device.read(), 0);
    ASSERT_EQ(device.readCalls(), 1);

    ASSERT_EQ(device.read(&destination), 3u);
    ASSERT_EQ(device.readArrayCalls(), 1);
    ASSERT_EQ(destination.size(), 3u);

    ASSERT_EQ(device.readLine(&destination), 2u);
    ASSERT_EQ(device.readLineCalls(), 1);
    ASSERT_EQ(destination.size(), 5u);
}

template <size_t S>
void storeSize(size_t *destination, Silica::ByteArray<S> &ba)
{
    (*destination) = ba.size();
}

TEST(suiteName, test_substitution_of_implicit_0_explicit_0_and_explicit_S)
{
    Silica::ByteArray<10> fixedTenLong;
    Silica::ByteArray<0> explicitlyGrowing;
    Silica::ByteArray<> implicitlyGrowing;

    for(size_t i = 0 ; i < 10; i++)
    {
        unsigned char value = 0x30 + i;
        fixedTenLong.append(value);
        explicitlyGrowing.append(value);
        implicitlyGrowing.append(value);
    }

    size_t size;

    storeSize(&size, fixedTenLong);
    ASSERT_EQ(size, 10);

    storeSize(&size, explicitlyGrowing);
    ASSERT_EQ(size, 10);

    storeSize(&size, implicitlyGrowing);
    ASSERT_EQ(size, 10);
}
