#include <iostream>
#include <stdio.h>

#include <kjut/Debug.h>

#define ContainerWarning(message, ...) createLogEntry(__LINE__, __FILE__, message, ##__VA_ARGS__)


#ifndef KJUT_ARRAY_INITIAL_CAPACITY
#define KJUT_ARRAY_INITIAL_CAPACITY 10
#endif


namespace Kjut {  template <typename T, size_t S = 0> class Array; }
template <typename T> std::ostream &operator<<(std::ostream &os, Kjut::Array<T> &a) ;

namespace Kjut
{



// Array<T> is the base class — Array<T, 0> inherits from Array<T>
template <typename T>
class Array<T, 0> {
public:

    Array()
    {
        initialize(0, nullptr);
    }

#ifndef KJUT_ENABLE_CONTAINERS_COPY_CONSTRUCTOR
    Array(const Array<T, 0> &) = delete;
#else
    This is not implemented yet
#endif


    size_t size() const
    {
        return d.size;
    }

    size_t capacity() const
    {
        return d.capacity;
    }

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

        T* destination = d.data+(index+1);
        T* source = d.data+index;
        const size_t count = d.size - index;
        memmove(destination, source, count*sizeof(T));
        d.data[index] = element;
        d.size++;
        return true;
    }

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

        d.data[index].~T();
        T* destination = d.data+(index);
        T* source = d.data+(index+1);
        const size_t count = d.size - index;
        memmove(destination, source, count*sizeof(T));
        d.size--;
        return true;
    }


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
        d.data[d.size] = element;
        d.size++;
        return true;
    }

    T at(size_t index) const
    {
        if(index >= d.size)
        {
            ContainerWarning("T List<T>::at(size_t) out of bound");
            return d.outOfBoundElement;
        }

        return d.data[index];
    }

    T& operator[](size_t index)
    {
        if(index >= d.size)
        {
            ContainerWarning("T& List<T>::operator[](size_t) out of bound");
            return d.outOfBoundElement;
        }
        return d.data[index];
    }

    const T& front() const;
    const T& back() const;

    void push(const T& element);
    void pop();

    const T& take(size_t index);


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

    iterator begin() { return iterator(d.data); }
    iterator end()   { return iterator(d.data + d.size); }

    const_iterator begin() const { return const_iterator(d.data); }
    const_iterator end()   const { return const_iterator(d.data + d.size); }

    const_iterator cbegin() const { return begin(); }
    const_iterator cend()   const { return end(); }



protected:

    void initialize(size_t capacity, T* data)
    {
        if(capacity == 0)
        {
            d.mayGrow = true;
            d.initialDynamicCapacity = KJUT_ARRAY_INITIAL_CAPACITY;
            d.capacity = d.initialDynamicCapacity;
            d.data = (T*)(malloc(sizeof(T)*d.initialDynamicCapacity));
        }
        else
        {
            d.mayGrow = false;
            d.capacity = capacity;
            d.data = data;
        }

        d.size = 0;
    }

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

private:

    void growCapacity()
    {
        d.data = (T*)(realloc(d.data, sizeof(T)*d.capacity*2));
        d.capacity = d.capacity*2;
    }
};


template <typename T, size_t S>
class Array : public Array<T, 0> {
public:


    Array()
    {
        this->initialize(S, data);
    }

#ifndef KJUT_ENABLE_CONTAINERS_COPY_CONSTRUCTOR
    Array(const Array<T, S> &) = delete;
#else
    This is not implemented yet
#endif

    T data[S];
};
}


template <typename T>
std::ostream &operator<<(std::ostream &os, Kjut::Array<T> &a) {
    os << "[";
    bool first = true;
    for(const T &element : a)
    {
        if(!first)
        {
            os << ", ";
        }
        os << element;
        first = false;
    }
    os << "]";
    return os;
}
