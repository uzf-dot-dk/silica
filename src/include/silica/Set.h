#ifndef KJUT_SET_H
#define KJUT_SET_H

#include <silica/Array.h>


#include <iostream>
#include <stdio.h>

#include <silica/Debug.h>
#include "ContainerDefinitions.h"






#ifndef DOXYGEN
namespace Kjut {  template <typename T, size_t S = 0> class Set; }
template <typename T> std::ostream &operator<<(std::ostream &os, const Kjut::Set<T> &a) ;
#endif // DOXYGEN


namespace Kjut
{

/**

\brief Implements the mathematical concept of a set. A Set<T,S> instance may have static or dynamic capacity.

### Capacity and S

Regarding capacity, Set<T,S> behaves exactly like @ref the_concept_of_container_capacity "the Array<T,S> class".


### Requirements for T

\c T must provide the following:

<table>
<tr><td><code>T()</code></td><td>A default constructor</td></tr>
<tr><td><code>~T()</code></td><td>A public destructor</td></tr>
<tr><td><code>T& operator=(const T &other)</code></td><td>The assignment operator.</td></tr>
<tr><td><code>bool operator==(const T &rhs) const </code></td><td>The comparison operator.</td></tr>
</table>


@anchor table_of_set_operations
### Set Operations

Set operations are implemented a bit differently than often seen in other frameworks. Since the copy constructor is disabled by default, oeprators that result in a new set
do not create a new set, but store the result in a provided *resultDestination*. It is the responsibility of the programmer to ensure there is capacity for the result by either using dynamically
sized \ref Set<T,S> "Set<T,S>s" or by allocating enough static capacity.

| Operation Name           | Method                | Set Notation Symbol           | Description                                           |
|--------------------------|-----------------------|-------------------------------|-------------------------------------------------------|
| Union                    | unionWith()           | A∪B                          | All elements in A, B, or both                         |
| Intersection             | intersectionWith()    | A∩B                           | Elements common to both A and B                       |
| Difference               | differenceFrom()      | A−B or A∖B                    | Elements in A not in B                                |
| Symmetric Difference     | symmetricDifference() | A△B                           | Elements in A or B, but not both                      |
| Subset                   | isSubsetOf()          | A⊆B                          | All elements of A are in B                             |
| Proper Subset            | isProperSubsetOf()    | A⊂B                          | A⊆B  and A≠B                                          |
| Superset                 | isSupersetOf()        | A⊇B                          | All elements of B are in A                             |

\note The values in a Set is not guaranteed to be stored in any particular order.


\ingroup Containers
\ingroup Core
*/
#ifdef DOXYGEN
template <typename T, size_t S>
class Set<T, S> {
#else
template <typename T>
class Set<T, 0> {
#endif
public:


    Set() = default;
    virtual ~Set() = default;
#ifndef KJUT_ENABLE_CONTAINERS_COPY_CONSTRUCTOR
    Set(const Set<T, 0> &) = delete;
#else
    This is not implemented yet
#endif

    /**
    \brief Returns the number of elements in this Set.
    \returns The number of elements in this Set.
    */
    size_t size() const
    {
        return actualValues().size();
    }

    /**
    \brief Inserts an element into this Set.

    If this Set already contains an element equal to \p element, nothing happens, otherwise, \p element is added to this Set.
    Wether an element can be added to this Set, depends on its capacity policy:

    - If this Set has static capacity, and there is no more space left, this Set is left unmodified nothing else happens.
    - if this Set has dynamic capacity, it is grown and \p element is added and this Set's size frows by one.

    \param element The new element to add.
    \returns True if \p element was added to this Set. False if not.
    */
    bool insert(const T&element)
    {
        if( contains(element))
        {
            return true;
        }
        return actualValues().append(element);
    }


    /**
    \brief Checks whether \p element is contained in this Set.
    \returns True if \p element is in this Set. False if not.
    */
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

    /**
    \brief Removes the element from this set that is equal to \p element.
    \param element The element to look for and remove
    \returns True if any element equalling \p element was removed from this Set.  False if no element equalling \p element was found.
    */
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

    /**
    \brief Clears all elements from this Set.
    \throws Any Any exception thrown in ~T().
    \note If any call to ~T() throws, the Set is left in undefined state.
    */
    void clear()
    {
        actualValues().clear();
    }


    /** \brief Returns a Array<T> with all the elements in this Set.
    \returns A const reference to an Array<T> holding the elements in this set.
    \note The order of the elements in the Array<T> is not guaranteed to conform to any order. In particular, two subsequent calls to values() may return Arrays with the elements in different order.
    */
    const Array<T>& values() const
    {
        return actualValues();
    }

    /** \brief Returns true if thw two sets are identical.

    Being identical is defined as containing exactely the same elements.
    \param rhs The set to compare this Set to.

    \returns True if the two sets are identidal. False if not.
    */
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



    /**
    \brief Calculates the union of \c this Set with \p B.

    In this method, \c this is the \c A operand when consulting the \ref table_of_set_operations "table of operations".

    \param B The set to calculate the union with.
    \param resultDestination The Set<T> to store the resulting union in.
    \returns True if all elements in the resulting union could be added to \p resultDestination. False if not.
    */
    bool unionWith(const Set<T> &B, Set<T> &resultDestination) const
    {
        resultDestination.clear();
        for(auto const &element: this->actualValues())
        {
            if ( ! resultDestination.insert(element) )
            {
                return false;
            }
        }
        for(auto const &element: B.actualValues())
        {
            if ( ! resultDestination.insert(element) )
            {
                return false;
            }
        }
        return true;
    }


    /**
    \brief Calculates the interesction of \c this Set with \p B.

    In this method, \c this is the \c A operand when consulting the \ref table_of_set_operations "table of operations".

    \param B The set to calculate the intersection with.
    \param resultDestination The Set<T> to store the resulting intersection in.
    \returns True if all elements in the resulting intersection could be added to \p resultDestination. False if not.
    */
    bool intersectionWith(const Set<T> &B, Set<T> &resultDestination) const
    {
        // Elements common to both A and B
        resultDestination.clear();
        for(auto const &element: this->actualValues())
        {
            if(B.contains(element))
            {
                if ( ! resultDestination.insert(element) )
                {
                    return false;
                }
            }
        }
        for(auto const &element: B.actualValues())
        {
            if(this->contains(element))
            {
                if ( ! resultDestination.insert(element) )
                {
                    return false;
                }
            }
        }
        return true;
    }


    /**
    \brief Calculates the difference from \c this Set with \p B.

    In this method, \c this is the \c A operand when consulting the \ref table_of_set_operations "table of operations".

    \param B The set to calculate the difference from.
    \param resultDestination The Set<T> to store the resulting difference in.
    \returns True if all elements in the resulting difference could be added to \p resultDestination. False if not.
    */
    bool differenceFrom(const Set<T> &B, Set<T> &resultDestination) const
    {
        //Elements in A not in B
        resultDestination.clear();
        for(auto const &element: this->actualValues())
        {
            if( ! B.contains(element))
            {
                if ( ! resultDestination.insert(element) )
                {
                    return false;
                }
            }
        }
        return true;
    }


    /**
    \brief Calculates the symmetric difference from \c this Set with \p B.

    In this method, \c this is the \c A operand when consulting the \ref table_of_set_operations "table of operations".

    \param B The set to calculate the symmetric difference from.
    \param resultDestination The Set<T> to store the resulting symmetric difference in.
    \returns True if all elements in the resulting symmetric difference could be added to \p resultDestination. False if not.
    */
    bool symmetricDifference(const Set<T> &B, Set<T> &resultDestination) const
    {
        resultDestination.clear();
        for(auto const &element: this->actualValues())
        {
            if( ! B.contains(element))
            {
                if ( ! resultDestination.insert(element) )
                {
                    return false;
                }
            }
        }
        for(auto const &element: B.actualValues())
        {
            if( ! this->contains(element))
            {
                if ( ! resultDestination.insert(element) )
                {
                    return false;
                }
            }
        }
        return true;
    }

    /**
    \brief Calculates whether \c this Set is a subset of \p B.

    In this method, \c this is the \c A operand when consulting the \ref table_of_set_operations "table of operations".

    \param B The set to check, whether this Set is a subset of.
    \returns True if this Set is a subset of B. False if not.
    */
    bool isSubsetOf(const Set<T> &B) const
    {
        for(auto const &element: this->actualValues())
        {
            if( ! B.contains(element))
            {
                return false;
            }
        }
        return true;
    }


    /**
    \brief Calculates whether \c this Set is a proper subset of \p B.

    In this method, \c this is the \c A operand when consulting the \ref table_of_set_operations "table of operations".

    \param B The set to check, whether this Set is a proper subset of.
    \returns True if this Set is a proper subset of B. False if not.
    */
    bool isProperSubsetOf(const Set<T> &B) const
    {
        if( ! this->isSubsetOf(B) )
        {
            return false;
        }
        return B.size() != size();
    }

    /**
    \brief Calculates whether \c this Set is a superset of \p B.

    In this method, \c this is the \c A operand when consulting the \ref table_of_set_operations "table of operations".

    \param B The set to check, whether this Set is a superset of.
    \returns True if this Set is a superset  of B. False if not.
    */
    bool isSupersetOf(const Set<T> &B) const
    {
        return B.isSubsetOf(*this);
    }






///@cond
protected:
    inline virtual Array<T> &actualValues() const { return dynamicValues ; }

private:
    mutable Array<T> dynamicValues;
///@endcond

};

#ifndef DOXYGEN

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
#endif // DOXYGEN
}




#endif // KJUT_SET_H
