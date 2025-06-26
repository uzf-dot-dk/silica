#ifndef KJUT_SET_H
#define KJUT_SET_H

#include <kjut/Array.h>


#include <iostream>
#include <stdio.h>

#include <kjut/Debug.h>

#define ContainerWarning(message, ...) createLogEntry(__LINE__, __FILE__, message, ##__VA_ARGS__)






namespace Kjut {  template <typename T, size_t S = 0> class Set; }
template <typename T> std::ostream &operator<<(std::ostream &os, const Kjut::Set<T> &a) ;



namespace Kjut
{



/**

\brief Implements the mathematical concept of a set.


|     No
| Implemented | Operation Name       | Set Notation Symbol           | Description                                           | Analogous Math Operator             |
|-------------|----------------------|-------------------------------|-------------------------------------------------------|-------------------------------------|
|     No      | Union                | A∪B                          | All elements in A, B, or both                         | + (loosely, additive combination)   |
|     No      | Intersection         | A∩B                           | Elements common to both A and B                       | * (loosely, multiplicative overlap) |
|     No      | Difference           | A−B or A∖B                    | Elements in A not in B                                | -                                   |
|     No      | Symmetric Difference | A△B                           | Elements in A or B, but not both                      | ⊕ (exclusive OR / XOR)              |
|     No      | Cartesian Product    | A×B                           | All ordered pairs (a,b) where a∈A and b∈Bb           | × or * (ordered pair generation)    |
|     No      | Complement           | A'                            | Elements not in A, relative to a universal set U      | ¬ or logical NOT                    |
|     No      | Subset               | A⊆B                          | All elements of A are in B                             | —                                  |
|     No      | Proper Subset        | A⊂B                          | A⊆B  and A≠B                                          | —                                  |
|     No      | Superset             | A⊇B                          | All elements of B are in A                             | —                                  |


## Requirements for T

\c T must provide the following:

<table>
<tr><td><code>T()</code></td><td>A default constructor</td></tr>
<tr><td><code>~T()</code></td><td>A public destructor</td></tr>
<tr><td><code>T& operator=(const T &other)</code></td><td>The assignment operator.</td></tr>
<tr><td><code>bool operator==(const T &rhs) const </code></td><td>The comparison operator.</td></tr>
</table>


\ingroup Containers
*/
template <typename T>
class Set<T, 0> {
public:


    Set() = default;
    virtual ~Set() = default;
#ifndef KJUT_ENABLE_CONTAINERS_COPY_CONSTRUCTOR
    Set(const Set<T, 0> &) = delete;
#else
    This is not implemented yet
#endif

    size_t size() const
    {
        return actualValues().size();
    }

    bool insert(const T&element)
    {
        if( contains(element))
        {
            return true;
        }
        return actualValues().append(element);
    }

    bool contains(const T&element) const
    {
        for(const T & candidate : actualValues())
        {
            if(candidate == element)
            {
                return true;
            }
        }
        return false;
    }

    bool erase(const T&element)
    {
        for(size_t i = 0 ; i < actualValues().size(); i++)
        {
            if(actualValues()[i] == element)
            {
                actualValues().remove(i);
                return true;
            }
        }
        return false;
    }


    const Array<T>& values() const
    {
        return actualValues();
    }

    bool operator==(const Set<T> &rhs) const
    {
        if(this->size() != rhs.size())
        {
            return false;
        }
        for(const T & candidate : actualValues())
        {
            if( ! rhs.contains(candidate))
            {
                return false;
            }
        }
        return true;
    }

///@cond
protected:
    inline virtual Array<T> &actualValues() const { return dynamicValues ; }
///@endcond

private:
    mutable Array<T> dynamicValues;

};


template <typename T, size_t S>
class Set : public Set<T, 0> {
public:
    Set() = default;
    ~Set() = default;
#ifndef KJUT_ENABLE_CONTAINERS_COPY_CONSTRUCTOR
    Set(const Set<T, S> &) = delete;
#else
    This is not implemented yet
#endif

protected:
    inline virtual Array<T> &actualValues() const override { return staticValues ; }

private:
    mutable Array<T,S> staticValues;

};
}




#endif // KJUT_SET_H
