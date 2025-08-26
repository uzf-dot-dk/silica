#include <iostream>
#include <stdio.h>

#include <string.h>

#include <kjut/Debug.h>

#include "ContainerDefinitions.h"

#ifdef _MSC_VER
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#ifndef KJUT_ARRAY_H
#define KJUT_ARRAY_H


///@cond
namespace Kjut {  template <typename T, size_t S = 0> class Array; }
template <typename T> std::ostream &operator<<(std::ostream &os, Kjut::Array<T> &a) ;
///@endcond

namespace Kjut
{


/** \brief Array<T,S> implements a linear container with iterators and random access. Array may have static or dynamic capacity.

The Array<T,S> implements an array with elements stored in adject memory for fast index based access. Based on the value of \c S, Array<T,S> implements two different allocation behaviours:

<table>
<tr>
    <th>\c S</th>
    <th>Behaviour</th>
</tr>
<tr>
    <td><code>S > 0</code></td>
    <td>
        The Array has a fixed capacity and will never grow nor shrink.<br/>
        With <code>S > 0</code> , Array<T,S> is guaranteed to not perform any dynamic allocations.

    \see append()
    </td>
</tr>
<tr>
    <td><code>S = 0</code></td>
    <td>
        The Array has a dynamic capacity and will grow if needed. <br/>
        With \c S \c = \c 0 , Array<T,S> allocates \ref KJUT_ARRAY_INITIAL_CAPACITY instances of T upon instantiation and may realloc that as required at runtime.<br />
        \see append()
    </td>
</tr>
</table>


@anchor the_concept_of_container_capacity
### The Concept of Capacity
There are three ways to declare capacities of an Array<T,S>:

```cpp
Array<int> dynamicArray;            // Will grow as needed by allocating memory
Array<int, 0> anotherDynamicArray;  // Will grow as needed by allocating memory
Array<int, 17> staticArray;         // Will never grow and never allocate any memody dynamically..
```
and they are interchangable under certain circumstances.

For example
```cpp
void print(Array<int> &a)
{
    for(const auto &element: a)
    {
        std::cout << a < " ";
    }
}

Array<int> dynamicArray;
Array<int, 0> anotherDynamicArray;
Array<int, 17> staticArray;


print(dynamicArray);
print(anotherDynamicArray);
print(staticArray);
```
will all work as expected and print the integers in each array. This works because the arrays are passed by *reference* and not by value.

\warning Passing Array<T,S> value will build but most likely crash at runtime.

### Examples

```cpp
#include <kjut/Array.h>

Array<int> integers = {6,2,4,5,3};
integers.append(1);
integers.append(7);

for(auto &i : integers)
{
    std::cout << i << "  ";
}
std::cout << "\n";

// Prints 6 2 4 5 3 1 7

std::sort(integers.begin(), integers.end());

for(auto &i : integers)
{
    std::cout << i << "  ";
}
std::cout << "\n";

// Prints 1 2 3 4 5 6 7
```

### Requirements for <code>T</code>

\c T must provide the following:

<table>
<tr><td><code>T()</code></td><td>A default constructor</td></tr>
<tr><td><code>~T()</code></td><td>A public destructor</td></tr>
<tr><td><code>T& operator=(const T &other)</code></td><td>The assignment operator.</td></tr>
</table>

\ingroup Containers
\ingroup Core

*/
#ifdef DOXYGEN
template <typename T, S>
class Array<T, S> {
#else
template <typename T>
class Array<T, 0> {
#endif

public:

    /**
    \brief Constructs a new array instance with a capacity of \c S and all elements uninitialized. */
    Array()
    {
        initialize(0, nullptr);
    }

    /**
    Destroys this array.

    All instances of T currently held by this array, is also destroyed.
    */
    ~Array()
    {

    }

#ifndef KJUT_DISABLE_CONTAINERS_INITIALIZER_LIST_CONSTRUCTOR
    /** Constructs a new array instance and initializes its content to the list.
     *
     * \note This constructor may be disabled by define the \ref KJUT_DISABLE_CONTAINERS_INITIALIZER_LIST_CONSTRUCTOR macro when including Array.h
     */
    Array(std::initializer_list<T> init) {
        initialize(0, nullptr);
        for (const T& value : init) {
            append(value);
        }
    }
#endif


#ifdef KJUT_DISABLE_CONTAINERS_COPY_CONSTRUCTOR
    Array(const Array<T, 0> &) = delete;
#else
    Array(const Array<T, 0> &);
#endif


    #ifdef DOXYGEN
    /**
    \brief Fills the array with \c S copies of \c e. Existing elements are overwritten.
    \param e The element to copy into the array.
    \note This method is only available for arrays with static size, meaning \c S \c != \c 0.
    */
    void fill(const T& e) ;
    #endif

    /**
    \brief Returns the number of \c T instances in the array.
    \returns The number of \c T instances in the array.
    */
    size_t size() const
    {
        return d.size;
    }

    /**
    \brief Returns the current capacity of the array.
    For static sized arrays, this equals \c S, and for dynamic arrays the capacity is the current capacity.
    \returns The number of \c T instances in the array.
    */
    size_t capacity() const
    {
        return d.capacity;
    }

    /**
    \brief Removes all elements from this Array.

    \returns The number of \c T instances in the array.
    \throws Any Any exception thrown in ~T()
    \note The Array is emptied from the back maintaining the size and content in the process. Should a call to ~T() throw any exception, this Array instance is promised to be intact.
    */
    void clear()
    {
        for(ssize_t i = d.size-1; i >= 0; i--)
        {
            d.size--;
            d.data[i].~T();
        }
    }



    /**
    \brief Copies \p element into a given position in the array, moving other elements as needed.

    A copy of \p element is inserted into the array at \p index. All elements from \p index and to \ref size() is moved to one index higher.

    Calling \c insert( \ref size(), T ) is equal to \ref append().

    In cases where the capacity of the array has been depleted one of two things can happen:

    - For dynamic sized arrays, the capacity is grown, and \c insert proceeds as expected.
    - For statically sized arrays, nothing happens and false is returned.

    \param element The element to copy into the array.
    \returns True if the element could be inserted/appended. False if there was no capacity left to insert \p element.
    \throws Any exception that T thows in its assignment operator.
    \see append()
    */
    bool insert(size_t index, const T &element)
    {
        if(index == d.size)
        {
            return append(element);
        }
        else if(index > d.size)
        {
            ContainerWarning("bool Array<T>::insert(size_t index, const T &element) beyond size is not supported.");
            return false;
        }

        if(d.size >= d.capacity)
        {
            if(d.mayGrow)
            {
                growCapacity();
            }
            else
            {
                ContainerWarning("bool Array<T>::insert(size_t index, const T &element) full and array cannot grow.");
                return false;
            }
        }

        const size_t count = this->d.size - index;

        if constexpr (std::is_trivially_copyable<T>::value)
        {
            T* destination = d.data+(index+1);
            T* source = d.data+index;
            const size_t count = d.size - index;
            memmove(destination, source, count*sizeof(T));
            d.data[index] = element;
            d.size++;
        }
        else {
            for (size_t i = d.size; i > index;  i-- )
            {
                d.data[i] = std::move(d.data[i - 1]);
            }
            d.data[index] = element;
            d.size++;
        }



        return true;
    }


    /** \brief Removes the index'th element from the array.
     *
       The element at \p index is removed from the array and deleted. After that, the array has its size decremented.

       If the element to remove, throws any exceptions, it is caught and then rethrown after resizing has happened. The array is guaranteed to have element \p index removed and size decremented.

       \param index The index of the element to remove.
       \returns True if the element could be removed and false if not. Reasons for not being able to remove an element can be if \p index is our of bounds.
       \throws Anything Anything that \c T thows in its descructor, assignment operator or move operator. If anything is thrown, the state of this Array is undefined.
    */
    bool remove(size_t index)
    {
        if(index < 0)
        {
            ContainerWarning("bool Array<T>::remove(%d) is not possible", (int)(index));
            return false;
        }
        else if(index >= d.size)
        {
            ContainerWarning("bool Array<T>::remove(%d) is not possible (size = %d)", (int)(index), (int)(d.size));
            return false;
        }

        if( index == d.size-1)
        {
            this->d.size--;
            d.data[this->d.size].~T();
        }
        else
        {
            const size_t count = this->d.size - index;

            if constexpr (std::is_trivially_copyable<T>::value)
            {
                T* destination = this->d.data+(index);
                T* source = this->d.data+(index+1);
                memmove(destination, source, count * sizeof(T));
            }
            else {
                for (size_t i = index; i < index+count-1; i++ )
                {
                    d.data[i] = std::move(d.data[i + 1]);
                }
            }
            this->d.size--;
        }

        return true;
    }

    /**
    \brief Appends a copy of \p element to the array.

    If the array has its capacity depleted, one of two things can happen:

    - For dynamic sized arrays, the capacity is grown, and append() proceeds as expected.
    - For statically sized arrays, nothing happens and false is returned.

    \param element The \c T instance to append.
    \throws Any exception that T thows in its copy constructor if anything is throsn in Ts copy constructor, the Array is intact and this callto append() has no effeect.
    */
    bool append(const T &element)
    {
        if(d.size >= d.capacity)
        {
            if(d.mayGrow)
            {
                growCapacity();
            }
            else
            {
                ContainerWarning("bool List<T>::append(const T& element) out of bound. List cannot grow.");
                return false;
            }
        }
        new (&d.data[d.size]) T(element);
        d.size++;
        return true;
    }


    /**
     \brief Returns the element at the given index.

      Returns a \c const& to the element at \p index, or a \c const& to a garbage \c T instance if \p index is out of bounds.

      \param index The position of the element to access.
      \returns A \c const& to the element at \p index, or a \c const& to a garbage \c T instance if \p index is out of bounds.
    */
    const T& at(size_t index) const
    {
        if(index >= d.size)
        {
            ContainerWarning("T List<T>::at(size_t) out of bound");
            return d.outOfBoundElement;
        }

        return d.data[index];
    }

    /**
     \brief Returns a reference to the element at the given index.

      Returns a mutable reference to the element at \p index, or to a garbage \c T instance if \p index is out of bounds.

      \param index The position of the element to access.
      \returns A mutable reference to the element at \p index, or to a garbage \c T instance if \p index is out of bounds.
    */
    T& operator[](size_t index)
    {
        if(index >= d.size)
        {
            ContainerWarning("T& List<T>::operator[](size_t) out of bound");
            return d.outOfBoundElement;
        }
        return d.data[index];
    }

    /**
     \brief Returns the element at the given index.

      Returns a \c const& to the element at \p index, or a \c const& to a garbage \c T instance if \p index is out of bounds.

      \param index The position of the element to access.
      \returns A \c const& to the element at \p index, or a \c const& to a garbage \c T instance if \p index is out of bounds.
    */
    const T& operator[](size_t index) const
    {
        if(index >= d.size)
        {
            ContainerWarning("const T& List<T>::operator[](size_t) const out of bound");
            return d.outOfBoundElement;
        }
        return d.data[index];
    }

    /**
    \brief Removes the first element from the Array.

    This is identical to calling \ref remove(0).
    \return The same as remove(0).
    \see remove()

    */
    bool pop() { return remove(0);}


    /**
    \brief Appends an element to the end of the array.

    This is identical to calling \c append(element)

    \returns The same as \ref append()
    \see append()
    */
    bool push(const T &element) { return append(element); }


    ///@cond INCLUDE_CLASS_ITERATORS
    // Custom iterator class
    class Iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        Iterator(pointer ptr = nullptr) : ptr_(ptr) {}

        reference operator*() const { return *ptr_; }
        pointer operator->() const { return ptr_; }

        Iterator& operator++() { ++ptr_; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

        Iterator& operator--() { --ptr_; return *this; }
        Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; }

        Iterator operator+(difference_type n) const { return Iterator(ptr_ + n); }
        Iterator operator-(difference_type n) const { return Iterator(ptr_ - n); }
        difference_type operator-(const Iterator& other) const { return ptr_ - other.ptr_; }

        Iterator& operator+=(difference_type n) { ptr_ += n; return *this; }
        Iterator& operator-=(difference_type n) { ptr_ -= n; return *this; }

        reference operator[](difference_type n) const { return *(ptr_ + n); }

        bool operator==(const Iterator& other) const { return ptr_ == other.ptr_; }
        bool operator!=(const Iterator& other) const { return ptr_ != other.ptr_; }
        bool operator<(const Iterator& other) const { return ptr_ < other.ptr_; }
        bool operator>(const Iterator& other) const { return ptr_ > other.ptr_; }
        bool operator<=(const Iterator& other) const { return ptr_ <= other.ptr_; }
        bool operator>=(const Iterator& other) const { return ptr_ >= other.ptr_; }

    private:
        pointer ptr_;
    };

    using iterator = Iterator;
    using const_iterator = Iterator; // Optional: make a separate const_iterator for true const-correctness
    ///@endcond

    /**
    \brief Returns an iterator pointing to the beginning of this array.
    \returns Aniterator pointing to the beginning of this array.
    */
    iterator begin() { return iterator(d.data); }

    /**
    \brief Returns an iterator pointing to the end of this array.
    \returns Aniterator pointing to the end of this array.
    */
    iterator end()   { return iterator(d.data + d.size); }

    /**
    \brief Returns a const iterator pointing to the beginning of this array.
    \returns A const iterator pointing to the beginning of this array.
    */
    const_iterator begin() const { return const_iterator(d.data); }

    /**
    \brief Returns a const iterator pointing to the end of this array.
    \returns A const iterator pointing to the end of this array.
    */
    const_iterator end()   const { return const_iterator(d.data + d.size); }

    /**
    \brief Returns a const iterator pointing to the beginning of this array.
    \returns A const iterator pointing to the beginning of this array.
    */
    const_iterator cbegin() const { return begin(); }

    /**
    \brief Returns a const iterator pointing to the end of this array.
    \returns A const iterator pointing to the end of this array.
    */
    const_iterator cend()   const { return end(); }



protected:
///@cond
    void initialize(size_t capacity, T* data)
    {
        if(capacity == 0)
        {
            d.mayGrow = true;
            d.initialDynamicCapacity = KJUT_ARRAY_INITIAL_CAPACITY;
            d.capacity = d.initialDynamicCapacity;
            d.data = static_cast<T*>(malloc( sizeof(T)*d.initialDynamicCapacity ));
        }
        else
        {
            d.mayGrow = false;
            d.capacity = capacity;
            d.data = data;
        }

        d.size = 0;
    }
///@endcond

    ///@cond OPAQUE_STRUCTS
    typedef struct Private
    {
        size_t capacity;
        size_t initialDynamicCapacity;
        size_t size;
        bool mayGrow;
        T * data;
        T outOfBoundElement = {};

        ~Private()
        {
            for(size_t i = 0 ; i < size; i++)
            {
                data[i].~T();
            }
            if(mayGrow)
            {
                free(data);
            }
        }
    } Private;

    Private d;
    ///@endcond

private:

    void growCapacity()
    {
        d.data = (T*)(realloc(d.data, sizeof(T)*d.capacity*2));
        d.capacity = d.capacity*2;
    }
};

template <typename T>
bool operator==(const Array<T> &lhs, const Array<T> &rhs)
{
    if(lhs.size() != rhs.size())
    {
        return false;
    }
    auto rhsit = rhs.cbegin();
    for(auto lhsit = lhs.cbegin(); lhsit != lhs.cend(); lhsit++)
    {
        if( ! (*lhsit == *rhsit) )
        {
            return false;
        }
        rhsit++;
    }
    return true;
}

template <typename T>
bool operator!=(const Array<T> &lhs, const Array<T> &rhs)
{
    return ! (lhs == rhs);
}

///@cond

template <typename T, size_t S>
class Array : public Array<T, 0> {
public:


    Array()
    {
        this->initialize(S, data);
    }

#ifndef KJUT_DISABLE_CONTAINERS_INITIALIZER_LIST_CONSTRUCTOR
    Array(std::initializer_list<T> init) {
        this->initialize(S, data);
        for (const T& value : init) {
            this->append(value);
        }
    }
#endif


    template <size_t U = S, typename std::enable_if<U != 0, int>::type = 0>
    void fill(const T& e) {
        for(size_t i = 0; i < S; i++)
        {
            data[i] = e;
        }
        this->d.size = S;
    }


#ifndef KJUT_DISABLE_CONTAINERS_COPY_CONSTRUCTOR
    Array(const Array<T, S> &) = delete;
#endif

    T data[S];
};

///@endcond
}

#endif // KJUT_ARRAY_H
