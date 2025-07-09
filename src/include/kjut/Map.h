#ifndef KJUT_MAP_H
#define KJUT_MAP_H

#include <kjut/Array.h>

#include <iostream>
#include <stdio.h>

#include <kjut/Debug.h>

#define ContainerWarning(message, ...) createLogEntry(__LINE__, __FILE__, message, ##__VA_ARGS__)





#ifndef DOXYGEN
namespace Kjut {  template <typename K, typename V, size_t S = 0> class Map; }
template <typename K, typename V> std::ostream &operator<<(std::ostream &os, const Kjut::Map<K, V> &a) ;
#endif // DOXYGEN


namespace Kjut
{

/** \brief Map<K,V,S> provides an associative array.


\ingroup Containers
\ingroup Core

## Requirements

\c K and \c V must provide the following public oprations:
<table>
    <tr>
        <th>Operation</th>
        <th><tt>K</tt></th>
        <th><tt>V</tt></th>
    </tr>
    <tr>
        <td>Operation</td>
        <td><tt>K</tt></td>
        <td><tt>V</tt></td>
    </tr>
    <tr>
        <td>Default construcor</td>
        <td><tt>K::K()</tt></td>
        <td><tt>V::V()</tt></td>
    </tr>
    <tr>
        <td>Destructor</td>
        <td><tt>K::~K()</tt></td>
        <td><tt>V::~V()</tt></td>
    </tr>
    <tr>
        <td>Copy constructor</td>
        <td><tt>K::K(const K& other)</tt></td>
        <td><tt style="display: block; text-align: center;"> &mdash; </tt></td>
    </tr>
    <tr>
        <td>Assignment Operator</td>
        <td><tt>K& K::operator=(const &K other)</tt></td>
        <td><tt>V& V::operator=(const &V other)</tt></td>
    </tr>
    <tr>
        <td>Equals Comparison</td>
        <td><tt>bool K::operator==(const K &rhs) const</tt></td>
        <td><tt style="display: block; text-align: center;"> &mdash; </tt></td>
    </tr>
    <tr>
        <td>Less-than Comparison</td>
        <td><tt>bool K::operator<(const K &rhs) const</tt></td>
        <td><tt style="display: block; text-align: center;"> &mdash; </tt></td>
    </tr>
</table>

Map implements [Dual Capacity Policies](\ref Dual_Capacity_Policies).

To create a new map holding an association from strings to integers, simply do

```cpp
Kjut::Map<std::string, int> beverages;
// or
Kjut::Map<std::string, int, 0> beverages;   //being identiucal to above
// or
Kjut::Map<std::string, int, 10> beverages;  //which can hold ten key value pairs
```

To insert a new value in either, use insert():
```cpp
beverages.insert("coffee");
beverages.insert("water", 0);
beverages.insert("tea", 11);
```
\warning
While inserting new keys with
```cpp
beverages["newBeverage"] = 123;
```
will build and run, it will probably not do what you expect.
The sections on [adding elements to](\ref containers_adding_elements_to_containers)
containers and [indexing](\ref containers_out_of_bound_indexing) explains why.

Rading values can be sone like this
```cpp
std::cout << beverages["tea"] << std::endl;
```

and updating values can likewise be done with the operator[]()

```cpp
beverages["tea"] = 456;
```

To check if a key is present in a map, you can use contains

```cpp
if( ! beverages.contains["tea"] )
{
    std::cout << "How about a nice cup of coffee instead?" << std:endl;
}
```
Map<K,V,S> supports range based for-loops with structured bindings, so listing all key value pair is posible like this

```cpp
for(const auto& [beverageType, count]: beverages
{
    std::cout << beverageType << " => " << count << std::endl;
}
```
Note that the structured bindings are consistent from key to value, but the order in which they are iterated over, is undefined.

Removing a key/value pair is done with erase()

```cpp
beverages.erase("tea");
```



*/
#ifdef DOXYGEN
template <typename K, typename V, size_t S>
class Map<K, V, S> {
#else
template <typename K, typename V>
class Map<K, V, 0> {
#endif
public:


    /** \brief Creates a new empty Map<K,V,S> instance.
     * A new Map<K,V,S> isntance has a size of zero and no key/value pairs stored.
     */
    Map() {}

    /**
     * \brief Destroys this instance and deletes all keys and values.
     * \throws Anything Any exception thrown in \c K::~K or \c V::~V is propagated. Throwing exceptions in either \c K::~K or \c V::~V may cause destructors to be *not* called on any number of keys or values.
     */
    virtual ~Map() {}


    /** \brief Returns the capacity of this instance.
     * Depending in \c S the capacity returned may be constant or mutable. See the section on [Dual Capacity Policies](\ref Dual_Capacity_Policies) for elaboration.
     * \returns The current capacity of this instance.
     */
    size_t capacity() const;


    /** \brief Returns the size of this instance.
     * The size of a Map, is the number of Key-value pairs currently kept in the Map.
     * \returns The current size of this instance.
     */
    size_t size() const;


    /** \brief Checks is \p needle is a key in this instance.
     * \param needle The key to look for in this instance.
     * \returns True if \p needle is a key in this instance. False if not.
     */
    bool contains(const K& needle) const;


    /** \brief Erases \p key and its associated value.
     * \param needle The key to look for in this instance.
     * \returns True if \p needle is a key in this instance. False if not.
     * \throws Anything Any exception thrown in \c K::~K or \c V::~V is caught and rethrown.
     */
    void erase(const K& key);


    /** \brief Returns a reference to an existing value.
     *
     *  operator[]() returns a \c T reference if \p key is already in this map instance. If \p key is
     *  not already in this instance, a reference to the garbage element is returned.
     *  \note This method can not be used to add new keys to the Map. Use insert() to add new keys.
     *  \param key The key to look up and retruen the value for.
     *  \returns A reference to the element \p key maps to or a reference to the garbage element.
     *  \see insert()
     */
    V& operator[](const K& key);


    /** \brief Inserts a new key into this instance.
     * Tries to insert a new \p key and \p value into this map and return true if an insertion could be made, and false if not.
     * \param key The new key to insert
     * \param value The value to match the \p key.
     * \returns True if the key/value pair could be inserted. False if not.
     */
    bool insert(const K &key, const V& value = V());


    /** \brief Returns a list of all values currently in this instance.
     *  \note The order of the values is undefined.
     *  \returns A list of all values currently in this instance.
     */
    const Array<V> &values() const { return actualValues(); }

    /** \brief Returns a list of all keys currently in this instance.
     *  \note The order of the keys is undefined.
     *  \returns A list of all keys currently in this instance.
     */
    const Array<K> &keys() const { return actualKeys(); }
///@cond

protected:
    inline virtual Array<V> &actualValues() const { return dynamicValues ; }
    inline virtual Array<K> &actualKeys() const { return dynamicKeys; }

private:

    inline virtual Array<V> &actualMutableValues() { return dynamicValues ; }

    ssize_t indexOfKey(const K& key) const;
    bool tryToAddKey(const K& key);

    mutable Array<K> dynamicKeys;
    mutable Array<V> dynamicValues;

    V garbage;

///@endcond


///@cond ITERATORS

public:
    // Iterator class
    class Iterator {
    public:

        Iterator(Array<K> &ki, Array<V> &vi, size_t idx)
            :    keys(ki), values(vi), index(idx)
        {
        }

        Iterator& operator++() {
            index++;
            return *this;
        }

        std::pair< const K&, V&> operator*() const {
            return {keys[index], values[index]};
        }

        bool operator!=(const Iterator& other) const {
            return this->index != other.index;
        }

    private:
        Array<K> &keys;
        Array<V> &values;
        size_t index;
    };
///@endcond

    Iterator begin() {
        return Iterator(
            const_cast<Array<K> &>(actualKeys()),
            const_cast<Array<V> &>(actualValues()),
            0
            );
    }

    Iterator end() {
        return Iterator(
            const_cast<Array<K> &>(actualKeys()),
            const_cast<Array<V> &>(actualValues()),
            actualKeys().size()
            );

    }


};

template <typename K, typename V>
ssize_t  Map<K, V, 0>::indexOfKey(const K& needle) const{
    ssize_t currentIndex = 0;
    for(const auto &candidate: actualKeys())
    {
        if(candidate == needle)
        {
            return currentIndex;
        }
        currentIndex ++;
    }
    return -1;
}


template <typename K, typename V>
bool Map<K, V, 0>::contains(const K& needle) const{
    return indexOfKey(needle) >= 0;
}

template <typename K, typename V>
size_t Map<K, V, 0>::capacity() const{
    return actualValues().capacity();
}

template <typename K, typename V>
size_t Map<K, V, 0>::size() const{
    return actualValues().size();
}


template <typename K, typename V>
bool Map<K, V, 0>::insert(const K &key, const V& value)
{
    const ssize_t indexOfKey = this->indexOfKey(key);
    if(indexOfKey >= 0)
    {
        return false;
    }
    if( ! actualValues().append(value))
    {
        return false;
    }
    if( ! actualKeys().append(key))
    {
        return false;
    }
    return true;
}


template <typename K, typename V>
void Map<K, V, 0>::erase(const K &key)
{
    const ssize_t indexOfKey = this->indexOfKey(key);
    if(indexOfKey < 0)
    {
        return;
    }
    //KJUT_TRY
        actualValues().remove(indexOfKey);
   // KJUT_CATCH_ALL
        //Nop
   // KJUT_TRY_CATCH_END

   // KJUT_TRY
        actualKeys().remove(indexOfKey);
   // KJUT_CATCH_ALL
        //Nop
   // KJUT_TRY_CATCH_END


}


template <typename K, typename V>
V& Map<K, V, 0>::operator[](const K& key)
{
    const ssize_t indexOfKey = this->indexOfKey(key);
    if(indexOfKey < 0)
    {
        return garbage;
    }
    else
    {
        return actualValues()[indexOfKey];
    }
}


#ifndef DOXYGEN

template <typename K, typename V, size_t S>
class Map : public Map<K, V, 0> {
public:
    Map() = default;
    ~Map() = default;
#ifndef KJUT_ENABLE_CONTAINERS_COPY_CONSTRUCTOR
    Map(const Map<K, V, S> &) = delete;
#else
    This is not implemented yet
#endif

protected:
    inline Array<V> &actualValues() const override { return staticValues ; }
    inline Array<K> &actualKeys() const override { return staticKeys; }

private:
    mutable Array<K, S> staticKeys;
    mutable Array<V, S> staticValues;

};
#endif // DOXYGEN

}




#endif // KJUT_SET_H
