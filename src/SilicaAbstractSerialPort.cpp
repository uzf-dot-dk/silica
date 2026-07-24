#include <silica/communication/AbstractSerialPort.h>

namespace Silica
{

namespace
{
size_t cStringLength(const char *text)
{
    if (!text)
    {
        return 0;
    }

    size_t length = 0;
    while (text[length] != '\0')
    {
        ++length;
    }
    return length;
}
}

AbstractSerialPort::PortId::PortId()
    : kind(Kind::Invalid)
    , index(0)
    , name(nullptr)
    , nameLength(0)
{
}

AbstractSerialPort::PortId::PortId(unsigned int portIndex)
    : kind(Kind::Index)
    , index(portIndex)
    , name(nullptr)
    , nameLength(0)
{
}

AbstractSerialPort::PortId::PortId(std::string_view portName)
    : kind(Kind::Name)
    , index(0)
    , name(portName.data())
    , nameLength(portName.size())
{
}

AbstractSerialPort::PortId::PortId(const char *portName)
    : PortId(std::string_view(portName ? portName : "", cStringLength(portName)))
{
}

bool AbstractSerialPort::PortId::isValid() const
{
    return kind != Kind::Invalid;
}

bool AbstractSerialPort::PortId::isIndex() const
{
    return kind == Kind::Index;
}

bool AbstractSerialPort::PortId::isName() const
{
    return kind == Kind::Name;
}

std::string_view AbstractSerialPort::PortId::asStringView() const
{
    if (!isName() || !name)
    {
        return std::string_view();
    }
    return std::string_view(name, nameLength);
}

bool operator==(const AbstractSerialPort::PortId &lhs, const AbstractSerialPort::PortId &rhs)
{
    if (lhs.kind != rhs.kind)
    {
        return false;
    }

    switch (lhs.kind)
    {
    case AbstractSerialPort::PortId::Kind::Invalid:
        return true;
    case AbstractSerialPort::PortId::Kind::Index:
        return lhs.index == rhs.index;
    case AbstractSerialPort::PortId::Kind::Name:
        return lhs.asStringView() == rhs.asStringView();
    }

    return false;
}

bool operator!=(const AbstractSerialPort::PortId &lhs, const AbstractSerialPort::PortId &rhs)
{
    return !(lhs == rhs);
}

AbstractSerialPort::Settings::Settings()
    : baudRate(9600)
    , startBits(StartBits::One)
    , dataBits(DataBits::Eight)
    , stopBits(StopBits::One)
    , parity(Parity::None)
    , flowControl(FlowControl::None)
{
}

bool operator==(const AbstractSerialPort::Settings &lhs, const AbstractSerialPort::Settings &rhs)
{
    return lhs.baudRate == rhs.baudRate
        && lhs.startBits == rhs.startBits
        && lhs.dataBits == rhs.dataBits
        && lhs.stopBits == rhs.stopBits
        && lhs.parity == rhs.parity
        && lhs.flowControl == rhs.flowControl;
}

bool operator!=(const AbstractSerialPort::Settings &lhs, const AbstractSerialPort::Settings &rhs)
{
    return !(lhs == rhs);
}

AbstractSerialPort::AbstractSerialPort()
    : AbstractSerialPort(PortId())
{
}

AbstractSerialPort::AbstractSerialPort(const PortId &portId)
    : IODevice()
{
    d.portId = portId;
    d.settings = Settings();
    setCurrentOpenMode(IODevice::OpenMode::Closed);
}

AbstractSerialPort::AbstractSerialPort(unsigned int portIndex)
    : AbstractSerialPort(PortId(portIndex))
{
}

AbstractSerialPort::AbstractSerialPort(std::string_view portName)
    : AbstractSerialPort(PortId(portName))
{
}

AbstractSerialPort::AbstractSerialPort(const char *portName)
    : AbstractSerialPort(PortId(portName))
{
}

AbstractSerialPort::~AbstractSerialPort() = default;

const AbstractSerialPort::PortId &AbstractSerialPort::portId() const
{
    return d.portId;
}

void AbstractSerialPort::setPortId(const AbstractSerialPort::PortId &portId)
{
    d.portId = portId;
}

void AbstractSerialPort::setPortId(unsigned int portIndex)
{
    d.portId = PortId(portIndex);
}

void AbstractSerialPort::setPortId(std::string_view portName)
{
    d.portId = PortId(portName);
}

void AbstractSerialPort::setPortId(const char *portName)
{
    d.portId = PortId(portName);
}

AbstractSerialPort::Settings AbstractSerialPort::settings() const
{
    return d.settings;
}

void AbstractSerialPort::setSettings(const Settings &settings)
{
    d.settings = settings;
}

unsigned long AbstractSerialPort::baudRate() const
{
    return d.settings.baudRate;
}

void AbstractSerialPort::setBaudRate(unsigned long baudRate)
{
    d.settings.baudRate = baudRate;
}

AbstractSerialPort::StartBits AbstractSerialPort::startBits() const
{
    return d.settings.startBits;
}

void AbstractSerialPort::setStartBits(StartBits startBits)
{
    d.settings.startBits = startBits;
}

AbstractSerialPort::DataBits AbstractSerialPort::dataBits() const
{
    return d.settings.dataBits;
}

void AbstractSerialPort::setDataBits(DataBits dataBits)
{
    d.settings.dataBits = dataBits;
}

AbstractSerialPort::StopBits AbstractSerialPort::stopBits() const
{
    return d.settings.stopBits;
}

void AbstractSerialPort::setStopBits(StopBits stopBits)
{
    d.settings.stopBits = stopBits;
}

AbstractSerialPort::Parity AbstractSerialPort::parity() const
{
    return d.settings.parity;
}

void AbstractSerialPort::setParity(Parity parity)
{
    d.settings.parity = parity;
}

AbstractSerialPort::FlowControl AbstractSerialPort::flowControl() const
{
    return d.settings.flowControl;
}

void AbstractSerialPort::setFlowControl(FlowControl flowControl)
{
    d.settings.flowControl = flowControl;
}

}
