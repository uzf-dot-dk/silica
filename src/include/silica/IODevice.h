#ifndef SILICA_IO_DEVICE_H
#define SILICA_IO_DEVICE_H

#include <silica/EventGenerator.h>
#include <silica/ByteArray.h>
#include <silica/SignalSlot.h>

namespace Silica
{

typedef unsigned char Byte;

class IODevice : public EventGenerator
{
public:

    enum class CloseReason
    {
    };

    enum class OpenMode
    {
        WriteOnly  = 0x01,
        ReadOnly   = 0x02,
        ReadWrite  = WriteOnly | ReadOnly,
        Append     = 0x10,
        Closed
    };

    IODevice();
    virtual ~IODevice();

	virtual bool hasRandomAccess() const =  0;
    virtual bool atEnd() const = 0;
    bool canReadMore() const
    {
        const bool notIsAatend = !atEnd();
        const bool isThisReadable = isReadable();
        return notIsAatend && isThisReadable;
 }

    /** Returns the number of bytes, that are ready for reading.
      * Any non-zero number returned, also indicates that this IODevice is readable.
      * @returns The number of bytes that are available for reading.
      */
    virtual size_t bytesAvailable() const = 0;


    virtual bool canReadLine() const = 0;
    virtual bool isWritable() const = 0;

    /** Returns true if this device can be read from.
     *  Note, this method will not indicate whether any bytes are avaliable for reding.
     *  @returns True if this device can be read from. False if not.
     *  @see canReadmore() */
    virtual bool isReadable() const { return ((int)currentOpenMode() & (int)OpenMode::ReadOnly); }
    virtual Byte read() = 0;
    virtual size_t read(Array<Byte> *destination) = 0;

    Slot<> close;
    Slot<OpenMode> open;
    Slot<Array<Byte> *> writeArray;
    Slot<Byte> writeByte;


    Signal<IODevice*, Array<Byte>*> dataReady;
    Signal<IODevice*, size_t> writeComplete;

    Signal<IODevice*, CloseReason> closed;
    Signal<IODevice*> opened;

    OpenMode currentOpenMode() const
    {
        return d.currentOpenMode;
    }


protected:

    virtual void closeImplementation() = 0;
    virtual void openImplementation(OpenMode) = 0;
    virtual void writeArrayImplementation(Array<Byte> *data) = 0;
    virtual void writeByteImplementation(Byte data) = 0;

    void setCurrentOpenMode(OpenMode newOpenMode)
    {
        d.currentOpenMode = newOpenMode;
    }

private:

    void visit() override {}

    struct
    {
        OpenMode currentOpenMode;

    } d;
};

}
#endif // SILICA_IO_DEVICE_H
