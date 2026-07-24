#ifndef SILICA_BYTE_BUFFER
#define SILICA_BYTE_BUFFER


#include <silica/IODevice.h>
#include <silica/Macros.h>
#include <silica/ByteArray.h>

namespace Silica
{

/** \brief ByteBuffer is a simple in-memory IODevice backed by ByteArray storage.
 *
 * ByteBuffer can be used as:
 * - a read-only source backed by a `const ByteArray<>`
 * - a read-only source backed by a raw byte array and length
 * - a read-write buffer backed by two `ByteArray<>` instances
 *
 * Implemented behavior:
 * - `read()` returns the next byte from the source buffer and advances the read index
 * - `writeByte()` appends a single byte to the destination buffer
 * - `writeArray()` appends all bytes from the provided ByteArray to the destination buffer
 * - `bytesAvailable()`, `atEnd()`, `canReadLine()`, and `isWritable()` reflect the current buffer state
 * - `open()` and `close()` only update the open mode
 *
 * Not yet implemented:
 * - `read(ByteArray<> *)`
 * - `readLine(ByteArray<> *)`
 *
 * Suggested usage:
 * ```cpp
 * Silica::ByteArray<> input = {'H', 'i', '\n'};
 * Silica::ByteBuffer reader(&input);
 *
 * while (reader.canReadMore()) {
 *     auto byte = reader.read();
 *     // process byte
 * }
 *
 * Silica::ByteArray<> output;
 * Silica::ByteBuffer writer(&input, &output);
 * writer.writeByte('X');
 * writer.writeArray(&input);
 *
 * const unsigned char raw[] = {'O', 'K'};
 * Silica::ByteBuffer rawReader(raw, sizeof(raw));
 * ```
 */
class ByteBuffer : public Silica::IODevice
{

public:

    ByteBuffer() : Silica::IODevice()
    {
        d.sourceKind = SourceKind::None;
        d.toReadFrom = nullptr;
        d.toWriteTo = nullptr;
        d.nextReadIndex = 0;
        d.rawReadOnly.data = nullptr;
        d.rawReadOnly.readIndex = 0;
        d.rawReadOnly.length = 0;
        this->setCurrentOpenMode(IODevice::OpenMode::Closed);
    }

    ByteBuffer(const ByteArray<> *toReadFrom) : ByteBuffer()
    {
        d.sourceKind = SourceKind::ByteArray;
        d.toReadFrom = toReadFrom;
        d.nextReadIndex = 0;
        this->setCurrentOpenMode(IODevice::OpenMode::ReadOnly);
    }

    ByteBuffer(ByteArray<> *toReadFrom, ByteArray<> *toWriteTo)  : ByteBuffer()
    {
        d.sourceKind = SourceKind::ByteArray;
        d.toReadFrom = toReadFrom;
        d.toWriteTo = toWriteTo;
        this->setCurrentOpenMode(IODevice::OpenMode::ReadWrite);
    }

    ByteBuffer(const unsigned char *toReadFrom, size_t length) : ByteBuffer()
    {
        d.sourceKind = SourceKind::RawBytes;
        d.rawReadOnly.data = toReadFrom;
        d.rawReadOnly.length = length;
        d.rawReadOnly.readIndex = 0;
        this->setCurrentOpenMode(IODevice::OpenMode::ReadOnly);
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
        if(d.sourceKind == SourceKind::ByteArray)
        {
            if(d.nextReadIndex >= d.toReadFrom->size())
            {
                return result;
            }
            result = d.toReadFrom->at(d.nextReadIndex);
            d.nextReadIndex++;
        }
        else if(d.sourceKind == SourceKind::RawBytes)
        {
            if(d.rawReadOnly.readIndex >= d.rawReadOnly.length)
            {
                return result;
            }
            result = d.rawReadOnly.data[d.rawReadOnly.readIndex];
            d.rawReadOnly.readIndex++;
        }
        return result;
    }

    size_t read(ByteArray<> *destination) override
    {
        if( ! destination )
        {
            return 0;
        }

        if( ! d.canReadFromDevice(this->currentOpenMode()) )
        {
            return 0;
        }

        size_t bytesRead = 0;
        while(!this->atEnd())
        {
            destination->append(this->read());
            bytesRead++;
        }
        return bytesRead;
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
        else if(d.sourceKind == SourceKind::ByteArray)
        {
            return d.nextReadIndex >= d.toReadFrom->size();
        }
        else if(d.sourceKind == SourceKind::RawBytes)
        {
            return d.rawReadOnly.readIndex >= d.rawReadOnly.length;
        }
        return true;
    }

    size_t bytesAvailable() const override
    {
        if( ! d.canReadFromDevice(this->currentOpenMode()) )
        {
            return 0;
        }
        else if(d.sourceKind == SourceKind::ByteArray)
        {
            return d.toReadFrom->size();
        }
        else if(d.sourceKind == SourceKind::RawBytes)
        {
            return d.rawReadOnly.length;
        }
        return 0;
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
        if(d.sourceKind == SourceKind::ByteArray)
        {
            for(size_t i = 0; i < d.toReadFrom->size(); i++)
            {
                if(d.toReadFrom->at(i) == '\n')
                {
                    return true;
                }
            }
            return false;
        }
        if(d.sourceKind == SourceKind::RawBytes)
        {
            for(size_t i = 0; i < d.rawReadOnly.length; i++)
            {
                if(d.rawReadOnly.data[i] == '\n')
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool isWritable() const override
    {
        return this->d.canWriteToDevice(this->currentOpenMode());
    }

    size_t readLine(Silica::ByteArray<> *destination) override
    {
        if( ! destination )
        {
            return 0;
        }

        if( ! d.canReadFromDevice(this->currentOpenMode()) )
        {
            return 0;
        }

        size_t bytesRead = 0;
        while(!this->atEnd())
        {
            const Byte next = this->read();
            destination->append(next);
            bytesRead++;
            if(next == '\n')
            {
                break;
            }
        }
        return bytesRead;
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


    void writeArrayImplementation(ByteArray<> *data) override
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

    enum class SourceKind
    {
        None,
        ByteArray,
        RawBytes
    };

    struct
    {
    public:
        SourceKind sourceKind = SourceKind::None;
        const ByteArray<> *toReadFrom;
        struct
        {
            const unsigned char *data = nullptr;
            size_t length = 0;
            size_t readIndex = 0;
        } rawReadOnly;
        size_t nextReadIndex;
        ByteArray<> *toWriteTo;


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
            if(sourceKind == SourceKind::ByteArray)
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
            if(sourceKind == SourceKind::RawBytes)
            {
                if(!rawReadOnly.data)
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
            return false;
        }
    } d;

};
}



#endif // SILICA_BYTE_BUFFER
