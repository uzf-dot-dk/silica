#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>
#include <silica/Array.h>

namespace Kjut
{

/**
 * \brief Specifies the behavior of a RingBuffer when it runs out of available space.
 *
 * This enum controls how the RingBuffer handles overflow conditions during push operations.
 */
enum class OverflowPolicy
{
    /**
     * When data is added using RingBuffer::push() and no space is available,
     * the new data is discarded, leaving the RingBuffer unchanged.
     */
    SkipNewData = 0,

    /**
     * When data is added using RingBuffer::push() and no space is available,
     * the oldest data in the RingBuffer is overwritten and lost.
     */
    OverwriteOldestData
};



/**
 * \brief RingBuffer implements a ring buffer for S elements of type T.

### Examples

```cpp
#include <kjut/RingBuffer.h?>

int main(int argc, char *argv[])
{
    Kjut::RingBuffer<int, 4> rb;

    rb.setOverflowPolicy(Kjut::OverflowPolicy::OverwriteOldestData);
    rb.push(1);
    rb.push(2);
    rb.push(3);
    rb.push(4);
    rb.push(5);
    rb.push(6);
    rb.push(7);
    rb.push(8);
    rb.push(9);

    ASSERT_EQ(rb.size(), 4);
    ASSERT_EQ(6, rb.peek());
    ASSERT_TRUE(rb.pop());

    ASSERT_EQ(rb.size(), 3);
    ASSERT_EQ(7, rb.peek());
    ASSERT_TRUE(rb.pop());

    ASSERT_EQ(rb.size(), 2);
    ASSERT_EQ(8, rb.peek());
    ASSERT_TRUE(rb.pop());

    ASSERT_EQ(rb.size(), 1);
    ASSERT_EQ(9, rb.peek());
    ASSERT_TRUE(rb.pop());

    ASSERT_EQ(rb.size(), 0);

    return 0;
}

```

## Thread Safety

RingBuffer is thread safe iff you use the OverflowPolicy::SkipNewData policy.
Inder this condition, only the thread calling push() will update tehe tail and only the
thread calling peek() and pop().

\anchor overflow_section
## Overflows

In RingBuffer context, an overflow is what happens when more elements are added than the instance can hold. In such a case, the \c overRunCallback is invoked with the current parameters, and what happens depends on the set OverflowPolicy:

OverflowPolicy       | Actions
-------------------- | -------------
SkipNewData          | The new data for which there are no available place are ignored and the RingBuffer instance is not modified.
OverwriteOldestData  | The oldest element in the RingBuffer is overwritten and thus lost. The new data is added and becomes the new end of the RingBuffer.


## Requirements for T

\c T must provide the following:

<table>
<tr><td><code>T()</code></td><td>A default constructor</td></tr>
<tr><td><code>~T()</code></td><td>A public destructor</td></tr>
<tr><td><code>T& operator=(const T &other)</code></td><td>The assignment operator.</td></tr>
</table>


\ingroup Containers
\ingroup Core

*/

template <typename T, size_t S>
class RingBuffer
{
    static_assert(S > 0, "Ringbuffers must have a size larger than 0.");

public:

    /** \brief Creates an empty RingBuffer.
    An empty ringbuffer has a fixed capacity and cannot grow nor shrink and supports adding data and taking data.
     */
    RingBuffer();

    /** \brief Cleans up a RingBuffer.
    All content from the RingBuffer instance is attempted deleted. The order in which the elements are attempted deleted is not guaranteed and no exceptions are caught or handled in the destructor.
     */
    virtual ~RingBuffer();

    /**  \brief Pushes a T onto the end of the ringbuffer calling the assignment operator on any existing element being replaced.
    Even if \c T& \c T::operator=(const T &other) throws, maintenance of the Ringbuffer is guaranteed and it has hed and tail consistent.
    \throws Anything Any exception thrown in \c T& \c operator=(const T &other) is caught and retrown after proper bookkeeping.
    */
    bool push(const T &element);

    /** \brief Returns a reference to the firstmost element in the buffer.
    This method is const and does not change the RingBuffer.
    \note Even if the RingBuffer is empty, a reference to a garbage element is returned. The content of this element is undefined. A entry in the
    \param element A const reference to the element of type \c T to push.
    \returns A reference to the first element in the buffer or a reference to a garbage element.
    */
    const T &peek() const;

    /** \brief Deletes the first elemenet in the buffer and decrements its size promoting next element to the head.

    Even if ~T() throws, maintenance of the Ringbuffer is guaranteed and it has hed and tail consistent.
    \returns True if an element was popped and false if the RungBuffer is empty an nothing could be popped.
    \throws Any exception thrown in ~T()..
    */
    bool pop() ;

    /** \brief Returns the number of T instances currently in the RingBuffer.
    \return The number of T instances currently in the RingBuffer.
    */
    size_t size() const;

    /** \brief Returns the number of T instances this Ringbuffer can currently have pushed before running out of space.
    \return Returns the number of T instances this Ringbuffer can currently have pushed before running out of space.
    */
    size_t sizeAvailable() const;


    /** \brief Returns maximum the number of elements this RingBuffer instance can hold. This is effectively \c S.
     \returns The maximum number of elements this RingBuffer instance can hold.
    */
    size_t capacity() const;

    /** \brief Sets the function pointer to call in case of an \link overflow_section overflow \endlink.

    \param overRunCallback The function to call in case of overflow.
    \param buffer A reference to the RingBuffer instance in which the overflow occurred.
    \param currentHeadIndex Current index of the head when overflow occurred.
    \param currentTailIndex Current index of the tail when overflow occurred.
    \param element A reference to the element that caused the overflow.
*/
    void setOverRunCallBack(void (*overRunCallback)(const RingBuffer & buffer, size_t currentHeadIndex, size_t currentTailIndex, const T& element));

    /** \brief Returns the current OverflowPolicy.
    \returns The current OverflowPolicy of this RingBuffer instance.
    \see The section on \link overflow_section overflow \endlink.*/
    OverflowPolicy overflowPolicy() const { return d.overflowPolicy;}

    /** \brief Sets the OverflowPolicy.
    \param The new OverfloPolicy.
    \see The section on \link overflow_section overflow \endlink. */
    void  setOverflowPolicy(OverflowPolicy newPolicy) { d.overflowPolicy = newPolicy; }

//private:
///@cond

    SILICA_VOLATILE struct
    {
        void (*overflowCallback)(const RingBuffer &, size_t currentHeadIndex, size_t currentTailIndex, const T& element) = nullptr;
        Array<T, S+1> data;
        size_t headIndex = 0;
        size_t tailIndex = 0;
        const size_t Capacity = S + 1;
        OverflowPolicy overflowPolicy = OverflowPolicy::OverwriteOldestData;
    } d;
///@endcond
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
    size_t nextHeadIndex = d.headIndex;
    if (next == d.headIndex) {
        if (d.overflowPolicy == OverflowPolicy::SkipNewData)
        {
            if(d.overflowCallback)
            {
                d.overflowCallback(*this, d.headIndex, d.tailIndex, element);
            }
            return false;
        }
        else if (d.overflowPolicy == OverflowPolicy::OverwriteOldestData)
        {
            if(d.overflowCallback)
            {
                d.overflowCallback(*this, d.headIndex, d.tailIndex, element);
            }
            nextHeadIndex = (d.headIndex + 1) % d.Capacity;
        }
    }


    try
    {
        d.data[d.tailIndex] = element;
        d.tailIndex = (d.tailIndex + 1) % d.Capacity;
        d.headIndex = nextHeadIndex;
    }
    catch(...)
    {
        //Do not update change the buffer. The element is now in undefined state.
        throw;
    }


    return true;
}



template <typename T, size_t S>
const T &RingBuffer<T,S>::peek() const
{
    if (d.headIndex == d.tailIndex) {
        return d.data[S+1]; //This is the 'default' element. Garbage.
    }
    return d.data[d.headIndex];
}

template <typename T, size_t S>
bool RingBuffer<T,S>::pop()
{
    if (d.headIndex == d.tailIndex)
    {
        return false; // buffer is empty
    }
    try
    {
        d.data[d.headIndex] = T();
        d.headIndex = (d.headIndex+ 1) % d.Capacity;
    }
    catch (...)
    {
        d.headIndex = (d.headIndex+ 1) % d.Capacity;
        throw;
    }
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
void RingBuffer<T,S>::setOverRunCallBack(void (*overRunCallback)(const RingBuffer &, size_t currentHeadIndex, size_t currentTailIndex, const T& element))
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
