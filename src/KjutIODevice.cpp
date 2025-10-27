#include <kjut/IODevice.h>

namespace Kjut
{

IODevice::IODevice()
 :
    close(std::bind(&IODevice::closeImplementation, this))
    , open(std::bind(&IODevice::openImplementation, this, std::placeholders::_1))
    , write(std::bind(&IODevice::writeImplementation, this, std::placeholders::_1))
{
}

IODevice::~IODevice()
{}



}
