#include <gtest/gtest.h>

#include <silica/Application.h>
#include <silica/communication/AbstractSerialPort.h>

#define suiteName tst_abstract_serial_port

Silica::Application app;

class TestSerialPort : public Silica::AbstractSerialPort
{
public:
    TestSerialPort()
        : AbstractSerialPort("UART0")
    {
    }

    bool hasRandomAccess() const override
    {
        return false;
    }

    bool atEnd() const override
    {
        return true;
    }

    size_t bytesAvailable() const override
    {
        return 0;
    }

    bool canReadLine() const override
    {
        return false;
    }

    size_t readLine(Silica::ByteArray<> *) override
    {
        return 0;
    }

    bool isWritable() const override
    {
        return false;
    }

    Silica::Byte read() override
    {
        return 0;
    }

    size_t read(Silica::ByteArray<> *) override
    {
        return 0;
    }

protected:
    void closeImplementation() override {}
    void openImplementation(OpenMode) override {}
    void writeArrayImplementation(Silica::ByteArray<> *) override {}
    void writeByteImplementation(Silica::Byte) override {}
};

TEST(suiteName, test_defaults_and_port_id)
{
    TestSerialPort port;

    ASSERT_TRUE(port.portId().isName());
    ASSERT_EQ(port.portId().asStringView(), std::string_view("UART0"));
    ASSERT_EQ(port.baudRate(), 9600ul);
    ASSERT_EQ(port.startBits(), Silica::AbstractSerialPort::StartBits::One);
    ASSERT_EQ(port.dataBits(), Silica::AbstractSerialPort::DataBits::Eight);
    ASSERT_EQ(port.stopBits(), Silica::AbstractSerialPort::StopBits::One);
    ASSERT_EQ(port.parity(), Silica::AbstractSerialPort::Parity::None);
    ASSERT_EQ(port.flowControl(), Silica::AbstractSerialPort::FlowControl::None);
    ASSERT_EQ(port.currentOpenMode(), Silica::IODevice::OpenMode::Closed);
}

TEST(suiteName, test_setting_serial_configuration)
{
    TestSerialPort port;
    Silica::AbstractSerialPort::Settings settings;

    settings.baudRate = 115200;
    settings.startBits = Silica::AbstractSerialPort::StartBits::One;
    settings.dataBits = Silica::AbstractSerialPort::DataBits::Seven;
    settings.stopBits = Silica::AbstractSerialPort::StopBits::Two;
    settings.parity = Silica::AbstractSerialPort::Parity::Even;
    settings.flowControl = Silica::AbstractSerialPort::FlowControl::Hardware;

    port.setSettings(settings);
    ASSERT_EQ(port.settings(), settings);

    port.setPortId(3u);
    ASSERT_TRUE(port.portId().isIndex());
    ASSERT_EQ(port.portId(), Silica::AbstractSerialPort::PortId(3u));

    port.setPortId("COM3");
    ASSERT_TRUE(port.portId().isName());
    ASSERT_EQ(port.portId(), Silica::AbstractSerialPort::PortId("COM3"));

    port.setBaudRate(57600);
    port.setStartBits(Silica::AbstractSerialPort::StartBits::One);
    port.setDataBits(Silica::AbstractSerialPort::DataBits::Nine);
    port.setStopBits(Silica::AbstractSerialPort::StopBits::OneAndHalf);
    port.setParity(Silica::AbstractSerialPort::Parity::Mark);
    port.setFlowControl(Silica::AbstractSerialPort::FlowControl::Software);

    ASSERT_EQ(port.baudRate(), 57600ul);
    ASSERT_EQ(port.startBits(), Silica::AbstractSerialPort::StartBits::One);
    ASSERT_EQ(port.dataBits(), Silica::AbstractSerialPort::DataBits::Nine);
    ASSERT_EQ(port.stopBits(), Silica::AbstractSerialPort::StopBits::OneAndHalf);
    ASSERT_EQ(port.parity(), Silica::AbstractSerialPort::Parity::Mark);
    ASSERT_EQ(port.flowControl(), Silica::AbstractSerialPort::FlowControl::Software);
}
