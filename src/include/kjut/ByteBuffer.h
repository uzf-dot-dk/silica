#ifndef KJUT_BYTE_BUFFER
#define KJUT_BYTE_BUFFER


#include <kjut/IODevice.h>
#include <kjut/Macros.h>
#include <kjut/ByteArray.h>

namespace Kjut
{

class ByteBuffer : public Kjut::IODevice
{

public:

    ByteBuffer() : Kjut::IODevice()
    {
        d.toReadFrom = nullptr;
        d.toWriteTo = nullptr;
        this->setCurrentOpenMode(IODevice::OpenMode::Closed);
    }

    ByteBuffer(const Array<Byte> *toReadFrom) : ByteBuffer()
    {
        d.toReadFrom = toReadFrom;
        d.nextReadIndex = 0;
        this->setCurrentOpenMode(IODevice::OpenMode::ReadOnly);
    }

    ByteBuffer(Array<Byte> *toReadFrom, Array<Byte> *toWriteTo)  : ByteBuffer()
    {
        d.toReadFrom = toReadFrom;
        d.toWriteTo = toWriteTo;
        this->setCurrentOpenMode(IODevice::OpenMode::ReadWrite);
    }

    ~ByteBuffer()
    {
        d.toReadFrom = nullptr;
        d.toWriteTo = nullptr;
    }

    Byte read() override
    {
        Byte result = 0;
        if( ! d.canReadFromDevice(this->currentOpenMode()) )
        {
            return result;
        }
        if(d.toReadFrom->size() == 0)
        {
            return result;
        }
        result = d.toReadFrom->at(d.nextReadIndex);
        d.nextReadIndex++;
        return result;
    }

    size_t read(Array<Byte> *destination) override
    {
        return 0;
    }


    bool hasRandomAccess() const override
    {
        return d.canReadFromDevice(this->currentOpenMode());
    }

    bool atEnd() const override
    {
        if( ! d.canReadFromDevice(this->currentOpenMode()) )
        {
            return true;
        }
        else
        {
            return d.nextReadIndex >= d.toReadFrom->size();
        }
    }

    size_t bytesAvailable() const override
    {
        if( ! d.canReadFromDevice(this->currentOpenMode()) )
        {
            return 0;
        }
        else
        {
            return d.toReadFrom->size();
        }
    }

    size_t bytesToWrite() const
    {
        return 0;
    }

    bool canReadLine() const override
    {
        if( ! d.canReadFromDevice(this->currentOpenMode()))
        {
            return false;
        }
        for(size_t i = 0; i < d.toReadFrom->size(); i++)
        {
            if(d.toReadFrom->at(i) == '\n')
            {
                return true;
            }
        }
        return false;
    }

    bool isWritable() const override
    {
        return this->d.canWriteToDevice(this->currentOpenMode());
    }

protected:
    void closeImplementation() override
    {
        this->setCurrentOpenMode(IODevice::OpenMode::Closed);
    }

    void openImplementation(OpenMode mode) override
    {
        this->setCurrentOpenMode(mode);
    }



    void writeByteImplementation(Byte data) override
    {
        if( ! d.canWriteToDevice(this->currentOpenMode()))
        {
            return;
        }

        if( ! d.toWriteTo->append(data) )
        {
            emit writeComplete(this, 0);
            return;
        }
        emit writeComplete(this, 1);

    }


    void writeArrayImplementation(Array<Byte> *data) override
    {
        if( ! d.canWriteToDevice(this->currentOpenMode()))
        {
            return;
        }
        size_t bytesWritten = 0;
        for(size_t i = 0; i < data->size(); i++)
        {
            if( ! d.toWriteTo->append((*data)[i]) )
            {
                break;
            }
            bytesWritten++;
        }
        emit writeComplete(this, bytesWritten);
    }


private:

    struct
    {
    public:
        const Array<Byte> *toReadFrom;
        size_t nextReadIndex;
        Array<Byte> *toWriteTo;


        bool canWriteToDevice(IODevice::OpenMode currentMode) const
        {
            if(!toWriteTo)
            {
                return false;
            }
            switch(currentMode)
            {
            case IODevice::OpenMode::WriteOnly:
            case IODevice::OpenMode::ReadWrite:
            case IODevice::OpenMode::Append:
                return true;
            case IODevice::OpenMode::ReadOnly:
            case IODevice::OpenMode::Closed:
                return false;
            };
            return false;
        }

        bool canReadFromDevice(IODevice::OpenMode currentMode) const
        {
            if(!toReadFrom)
            {
                return false;
            }
            switch(currentMode)
            {
            case IODevice::OpenMode::ReadOnly:
            case IODevice::OpenMode::ReadWrite:
                return true;
            case IODevice::OpenMode::Append:
            case IODevice::OpenMode::WriteOnly:
            case IODevice::OpenMode::Closed:
                return false;
            };
            return false;
        }
    } d;

};
}



#endif // KJUT_BYTE_BUFFER
