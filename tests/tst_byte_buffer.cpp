#include <gtest/gtest.h>
#include <kjut/ByteBuffer.h>


namespace Kjut
{

}

#define suiteName tst_byte_buffer


template <typename ...Ts>
class SignalSpy
{
public:
    void spyOn(Kjut::Signal<Ts...> *signal)
    {
        signal->connectTo([=](Ts... args){
            d.invocations.push_back(std::tuple<Ts...>(args...));
        });
    }

    size_t invocationCount() const
    {
        return d.invocations.size();
    }

    std::vector<std::tuple<Ts...>> & invocations()
    {
        return d.invocations;
    }


private:
    struct
    {
        std::vector<std::tuple<Ts...>> invocations;
    } d;
};



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
    SignalSpy<Kjut::IODevice*, size_t> writeWatcher;
    writeWatcher.spyOn(&(iod->writeComplete));

    Kjut::Array<Kjut::Byte> expected = {'S', 'o', 'm', 'e', ' ', 'd', 'a', 't','a'};
    for(unsigned char c: expected)
    {
        iod->writeByte(c);
    }

    ASSERT_EQ(destination,  expected);
    ASSERT_EQ(writeWatcher.invocationCount(), expected.size());
    for(auto invocation : writeWatcher.invocations())
    {
        size_t byteCount = std::get<size_t>(invocation);
        Kjut::IODevice * emitter = std::get<Kjut::IODevice *>(invocation);
        ASSERT_EQ(byteCount, 1);
        ASSERT_EQ(emitter, iod);
    }



}

TEST(suiteName, test_basic_writing_an_array_with_dynamic_array_as_source)
{
    Kjut::Array<Kjut::Byte> notUsed;
    Kjut::Array<Kjut::Byte> destination;

    Kjut::ByteBuffer theBuffer(&notUsed, &destination);
    Kjut::IODevice* iod = &theBuffer;
    SignalSpy<Kjut::IODevice*, size_t> writeWatcher;
    writeWatcher.spyOn(&(iod->writeComplete));

    Kjut::Array<Kjut::Byte> expected = {'S', 'o', 'm', 'e', ' ', 'd', 'a', 't','a'};
    iod->writeArray(&expected);

    ASSERT_EQ(destination,  expected);

    ASSERT_EQ(writeWatcher.invocationCount(), 1);
    auto invocation = writeWatcher.invocations()[0];
    size_t byteCount = std::get<size_t>(invocation);
    Kjut::IODevice * emitter = std::get<Kjut::IODevice *>(invocation);
    ASSERT_EQ(byteCount, expected.size());
    ASSERT_EQ(emitter, iod);
}



