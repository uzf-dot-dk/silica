#ifndef KJUT_UNITS_OF_TIME_H
#define KJUT_UNITS_OF_TIME_H

#include <stdint.h>

namespace Kjut
{

class MicroSeconds
{
public:
    MicroSeconds(uint64_t value)
    {
        d.value = value;
    }

    MicroSeconds & operator=(uint64_t value)
    {
        d.value = value;
        return *this;
    }

    bool operator>=(const MicroSeconds &rhs)
    {
        return this->d.value >= rhs.d.value;
    }

    MicroSeconds operator+(const MicroSeconds &rhs) const
    {
        return MicroSeconds(this->d.value + rhs.d.value);
    }

//private:
    struct
    {
        uint64_t value ;
    } d;
};


class MilliSeconds
{
public:
    MilliSeconds(uint64_t value)
    {
        d.value = value;
    }

    operator MicroSeconds() const {
        return d.value * 1000;
    }

//private:
    struct
    {
        uint64_t value ;
    } d;
};

}
#endif // KJUT_UNITS_OF_TIME_H
