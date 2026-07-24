#ifndef SILICA_COMMUNICATION_ABSTRACT_SERIAL_PORT_H
#define SILICA_COMMUNICATION_ABSTRACT_SERIAL_PORT_H

#include <cstddef>
#include <string_view>

#include <silica/IODevice.h>

namespace Silica
{

/**
 * \brief Generic serial-port base class with platform-neutral configuration.
 *
 * AbstractSerialPort stores serial configuration and a non-owning port identifier that can
 * represent either a numeric port index or a textual device name. Concrete
 * backends are expected to derive from this class and implement the IODevice
 * contract for a specific platform or baremetal target.
 */
class AbstractSerialPort : public IODevice
{
public:
    enum class StartBits
    {
        One
    };

    enum class DataBits
    {
        Five = 5,
        Six = 6,
        Seven = 7,
        Eight = 8,
        Nine = 9
    };

    enum class StopBits
    {
        One,
        OneAndHalf,
        Two
    };

    enum class Parity
    {
        None,
        Odd,
        Even,
        Mark,
        Space
    };

    enum class FlowControl
    {
        None,
        Software,
        Hardware
    };

    struct PortId
    {
        enum class Kind
        {
            Invalid,
            Index,
            Name
        };

        PortId();

        explicit PortId(unsigned int portIndex);

        explicit PortId(std::string_view portName);

        PortId(const char *portName);

        bool isValid() const;

        bool isIndex() const;

        bool isName() const;

        std::string_view asStringView() const;

        friend bool operator==(const PortId &lhs, const PortId &rhs);

        friend bool operator!=(const PortId &lhs, const PortId &rhs);

        Kind kind;
        unsigned int index;
        const char *name;
        size_t nameLength;

    private:
        static constexpr size_t lengthOf(const char *text)
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
    };

    struct Settings
    {
        unsigned long baudRate;
        StartBits startBits;
        DataBits dataBits;
        StopBits stopBits;
        Parity parity;
        FlowControl flowControl;

        Settings();

        friend bool operator==(const Settings &lhs, const Settings &rhs);

        friend bool operator!=(const Settings &lhs, const Settings &rhs);
    };

    AbstractSerialPort();

    explicit AbstractSerialPort(const PortId &portId);

    explicit AbstractSerialPort(unsigned int portIndex);

    explicit AbstractSerialPort(std::string_view portName);

    explicit AbstractSerialPort(const char *portName);

    ~AbstractSerialPort() override;

    const PortId &portId() const;

    void setPortId(const PortId &portId);

    void setPortId(unsigned int portIndex);

    void setPortId(std::string_view portName);

    void setPortId(const char *portName);

    Settings settings() const;

    void setSettings(const Settings &settings);

    unsigned long baudRate() const;

    void setBaudRate(unsigned long baudRate);

    StartBits startBits() const;

    void setStartBits(StartBits startBits);

    DataBits dataBits() const;

    void setDataBits(DataBits dataBits);

    StopBits stopBits() const;

    void setStopBits(StopBits stopBits);

    Parity parity() const;

    void setParity(Parity parity);

    FlowControl flowControl() const;

    void setFlowControl(FlowControl flowControl);

protected:
    struct
    {
        PortId portId;
        Settings settings;
    } d;
};

}

#endif // SILICA_COMMUNICATION_ABSTRACT_SERIAL_PORT_H
