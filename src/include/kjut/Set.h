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

| Operation Name       | Set Notation Symbol           | Description                                           | Analogous Math Operator             |
|----------------------|-------------------------------|-------------------------------------------------------|-------------------------------------|
| Union                | A∪B                          | All elements in A, B, or both                         | + (loosely, additive combination)   |
| Intersection         | A∩B                           | Elements common to both A and B                       | * (loosely, multiplicative overlap) |
| Difference           | A−B or A∖B                    | Elements in A not in B                                | -                                   |
| Symmetric Difference | A△B                           | Elements in A or B, but not both                      | ⊕ (exclusive OR / XOR)              |
| Cartesian Product    | A×B                           | All ordered pairs (a,b) where a∈A and b∈Bb           | × or * (ordered pair generation)    |
| Complement           | AcA^c                         | Elements not in A, relative to a universal set U      | ¬ or logical NOT                    |
| Subset               | A⊆B                          | All elements of A are in B                             | —                                  |
| Proper Subset        | A⊂B                          | A⊆B  and A≠B                                          | —                                  |
| Superset             | A⊇B                          | All elements of B are in A                             | —                                  |

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


protected:
    inline virtual Array<T> &actualValues() const { return dynamicValues ; }

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
