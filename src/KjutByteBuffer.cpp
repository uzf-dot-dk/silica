#include <kjut/ByteBuffer.h>
/*
namespace Kjut
{


ByteBuffer::ByteBuffer()
{
    d.readIndex = 0;
    d.toReadFrom = nullptr;
    d.toWriteTo = nullptr;
}

ByteBuffer::ByteBuffer(const ByteArray *toReadFrom)
    : ByteBuffer()
{
    d.toReadFrom = toReadFrom;
}

ByteBuffer::ByteBuffer(const ByteArray *toReadFrom, ByteArray *toWriteTo)
    : ByteBuffer()
{
    d.toReadFrom = toReadFrom;
    d.toWriteTo = toWriteTo;
}

ByteBuffer::~ByteBuffer()
{
    d.toReadFrom = nullptr;
    d.toWriteTo = nullptr;
    d.readIndex = 0;
}

bool ByteBuffer::hasRandomAccess() const
{
    return true;
}

bool ByteBuffer::atEnd() const
{
    return false;
}

size_t ByteBuffer::bytesAvailable() const
{
    return 9;
    //return d.toReadFrom->size();
}

size_t ByteBuffer::bytesToWrite() const
{
    return 0;
}

bool ByteBuffer::canReadLine() const
{
    return false;
}

bool ByteBuffer::isWritable() const
{
    return false;
}

void ByteBuffer::closeImplementation()
{
    //Nop
}

void ByteBuffer::openImplementation(OpenMode)
{
    //Nop
}

void writeImplementation(ByteArray *)
{
    //NOP
}

}
*/
