#include <silica/IODevice.h>

namespace Kjut
{

IODevice::IODevice()
 :
    close(std::bind(&IODevice::closeImplementation, this))
    , open(std::bind(&IODevice::openImplementation, this, std::placeholders::_1))
    , writeArray(std::bind(&IODevice::writeArrayImplementation, this, std::placeholders::_1))
    , writeByte(std::bind(&IODevice::writeByteImplementation, this, std::placeholders::_1))
{
}

IODevice::~IODevice()
{}



}
