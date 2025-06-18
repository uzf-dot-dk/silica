#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>
#include <kjut/Array.hpp>

namespace Kjut
{

enum class OverflowPolicy
{
    SkipNewData = 0,
    OverwriteOldestData
};


template <typename T, size_t S>
class RingBuffer
{
    static_assert(S > 0, "Ringbuffers must have a size larger than 0.");

public:



    RingBuffer();
    virtual ~RingBuffer();

    /// \throws Any exception thrown in ~T(). ~T() may be called if overflowPolicy is OverwriteOldestData.
    /// Even if ~T() throws, maintenance of the Ringbuffer is guaranteed and it has hed and tail consistent.
    bool push(const T &element);
    const T &peek() const;

    /// \throws Any exception thrown in ~T()..
    /// Even if ~T() throws, maintenance of the Ringbuffer is guaranteed and it has hed and tail consistent.
    bool pop() ;

    size_t size() const;
    size_t sizeAvailable() const;
    size_t capacity() const;
    void setOverRunCallBack(void (*overRunCallback)(const RingBuffer &));

    OverflowPolicy overflowPolicy() const { return d.overflowPolicy;}
    void  setOverflowPolicy(OverflowPolicy newPolicy) { d.overflowPolicy = newPolicy; }

//private:


    mutable struct
    {

        void incrementHeadIndexWithPotentialWraparound()
        {
            headIndex++;
            if(headIndex >= S)
            {
                headIndex = 0;
            }
        }
        void incrementTailIndexWithPotentialWraparound()
        {
            tailIndex++;
            if(tailIndex >= S)
            {
                tailIndex = 0;
            }
        }
        void (*overflowCallback)(const RingBuffer &) = nullptr;
        Array<T, S+1> data;
        size_t headIndex = 0;
        size_t tailIndex = 0;
        const size_t Capacity = S + 1;
        OverflowPolicy overflowPolicy = OverflowPolicy::OverwriteOldestData;
    } d;
};



template <typename T, size_t S>
RingBuffer<T,S>::RingBuffer()
{
    d.data.fill(T());
}

template <typename T, size_t S>
RingBuffer<T,S>::~RingBuffer()
{}


template <typename T, size_t S>
bool RingBuffer<T,S>::push(const T &element)
{
    size_t next = (d.tailIndex + 1) % d.Capacity;

    if (next == d.headIndex) {
        if (d.overflowPolicy == OverflowPolicy::SkipNewData)
        {
            if(d.overflowCallback)
            {
                d.overflowCallback(*this);
            }
            return false;
        }
        else if (d.overflowPolicy == OverflowPolicy::OverwriteOldestData)
        {
            if(d.overflowCallback)
            {
                d.overflowCallback(*this);
            }
            d.headIndex = (d.headIndex + 1) % d.Capacity;
        }
    }

#define RINGBUFFER_PUSH_BOOK_KEEPING \
    d.tailIndex = (d.tailIndex + 1) % d.Capacity;

    try
    {
        d.data[d.tailIndex] = element;
        RINGBUFFER_PUSH_BOOK_KEEPING;
    }
    catch(...)
    {
        RINGBUFFER_PUSH_BOOK_KEEPING;
        throw;
    }
#undef RINGBUFFER_PUSH_BOOK_KEEPING


    return true;
}



template <typename T, size_t S>
const T &RingBuffer<T,S>::peek() const
{
    if (d.headIndex == d.tailIndex) {
        throw std::runtime_error("Buffer is empty");    //MWJ: Log and return default/.
    }
    return d.data[d.headIndex];
}

template <typename T, size_t S>
bool RingBuffer<T,S>::pop()
{
    if (d.headIndex == d.tailIndex) {
        return false; // buffer is empty
    }
    d.headIndex = (d.headIndex+ 1) % d.Capacity;    //MWJ: Destroy
    return true;
}

template <typename T, size_t S>
size_t RingBuffer<T,S>::size() const
{
    return (d.tailIndex + d.Capacity - d.headIndex) % d.Capacity;
}

template <typename T, size_t S>
    size_t RingBuffer<T,S>::capacity() const
{
    return S;
}

template <typename T, size_t S>
size_t RingBuffer<T,S>::sizeAvailable() const
{
    return S - size();
}


template <typename T, size_t S>
void RingBuffer<T,S>::setOverRunCallBack(void (*overRunCallback)(const RingBuffer &))
{
    d.overflowCallback = overRunCallback;
}


}




#include <iostream>
#include <iomanip>


template <typename T, size_t S>
std::ostream &operator<<(std::ostream &os, const Kjut::RingBuffer<T, S> &b) {

    os << "[";
    for(size_t i = 0; i < S; i++)
    {
        if(i > 0)
        {
            os << " , ";
        }
        os << std::setw(6) << b.d.data[i];
    }
    os << "  ]";
    os << "\n";

    os << " ";
    for(size_t i = 0; i < S; i++)
    {
        if(i > 0)
        {
            os << "   ";
        }
        if(i == b.d.headIndex)
        {
            os << "  HEAD";
        }
        else
        {
            os << "      ";
        }
    }
    os << "  ";
    os << "\n";

    os << " ";
    for(size_t i = 0; i < S; i++)
    {
        if(i > 0)
        {
            os << "   ";
        }
        if(i == b.d.tailIndex)
        {
            os << "  TAIL";
        }
        else
        {
            os << "      ";
        }
    }
    os << "  ";
    os << "\n";



    return os;
}


#endif // RINGBUFFER_H
