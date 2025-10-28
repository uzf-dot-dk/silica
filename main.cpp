#include <kjut/Application.h>
#include <kjut/CoarseTimer.h>
#include <kjut/SignalSlot.h>
#include <kjut/IODevice.h>
#include <kjut/ByteBuffer.h>




/*
template <typename R, typename E>
class Result
{

    Result() = delete;

    Result(const R &r)
    {
        d.r = r;
        d.isError = false;
    }

    Result(const E &e)
    {
        d.e = e;
        d.isError = true;
    }

    virtual ~Result() = delete;

    explicit operator bool() const
    {
        return d.isError;
    }

    operator R() const
    {
        return d.r;
    }

    operator E() const
    {
        return d.e;
    }


private:

    struct
    {
        R r;
        E e;
        bool isError;
    } d;


};
*/


void print(Kjut::Array<Kjut::Byte> &a)
{
    printf("{");
    int i = 0;
    for(auto e: a)
    {
        if(i > 0)
        {
            printf(", ");
        }
        i = 1;
        if((e >= 32) && (e <= 126))
        {
            printf("'%c'", e);
        }
        else
        {
            printf("%2X", e);
        }
    }
    printf("}\n");
}


int main(int argc, char *argv[])
{
/*
    Kjut::Array<Kjut::Byte> implicitGrowingData;
    Kjut::Array<Kjut::Byte, 0> explicitGrowingData;
    Kjut::Array<Kjut::Byte, 10> fixedSizeData;

    Kjut::IODevice* iod;
    Kjut::ByteBuffer myiod;
    iod = &myiod;

    iod->write(&implicitGrowingData);
    iod->write(&explicitGrowingData);
    iod->write(&fixedSizeData);
*/
    Kjut::Array<Kjut::Byte> readSource = {'H', 'e', 'j'};
    Kjut::Array<Kjut::Byte> writeData = {'D', 'a', 'v'};
    Kjut::Array<Kjut::Byte> writeDestination;
    Kjut::ByteBuffer myiod(&readSource, &writeDestination);
    Kjut::IODevice* iod = &myiod;

    iod->writeArray(&writeData);
    print(writeDestination);

    while(iod->canReadMore())
    {
        printf("Read %c\n", iod->read());
    }

    return 0;

}
