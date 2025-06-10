#include <iostream>

#include <stdio.h>

#include <kjut/Debug.h>

#define ContainerWarning(message, ...) createLogEntry(__LINE__, __FILE__, message, __VA_ARGS__)


#ifndef KJUT_ARRAY_INITIAL_CAPACITY
#define KJUT_ARRAY_INITIAL_CAPACITY 10
#endif

namespace Kjut
{

// Forward declare Array<T>
template <typename T, size_t S = 0>
class Array;

// Array<T> is the base class — Array<T, 0> inherits from Array<T>
template <typename T>
class Array<T, 0> {
public:

    Array()
    {
        initialize(0, nullptr);
    }

    size_t size() const
    {
        return d.size;
    }

    size_t capacity() const
    {
        return d.capacity;
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
                ContainerWarning("bool List<T>::append(const T& element) out of bound. List cannot grow.", 2);
                return false;
            }
        }
        d.data[d.size] = element;
        d.size++;
        return true;
    }

    T at(size_t index)
    {
        if(index >= d.size)
        {
            ContainerWarning("T List<T>::at(size_t) out of bound", 0);
            return d.outOfBoundElement;
        }

        return d.data[index];
    }

    T& operator[](size_t index)
    {
        if(index >= d.size)
        {
            ContainerWarning("T& List<T>::operator[](size_t) out of bound", 0);
            return d.outOfBoundElement;
        }
        return d.data[index];
    }

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

    T data[S];
};
}
