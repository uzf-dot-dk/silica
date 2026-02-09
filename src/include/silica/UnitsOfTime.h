#ifndef SILICA_UNITS_OF_TIME_H
#define SILICA_UNITS_OF_TIME_H

#include <stdint.h>
#include <silica/Macros.h>
namespace Silica
{

class MicroSeconds
{
public:
    explicit MicroSeconds(uint64_t value)
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

    bool operator==(const Silica::MicroSeconds & other) const
    {
        return other.d.value == this->d.value;;
    }

    bool operator==(uint64_t other) const
    {
        return other == this->d.value;;
    }

    operator uint64_t() const {
        return d.value;
    }


//private:
    friend class MilliSeconds;

    struct
    {
        uint64_t value ;
    } d;
};


class MilliSeconds
{
public:
    explicit MilliSeconds(uint64_t value)
    {
        d.value = value;
    }

    MilliSeconds(const MicroSeconds &other)
    {
        d.value = other.d.value / 1000;
    }

    MilliSeconds()
    {
        d.value = 0;
    }

    operator MicroSeconds() const {
        return MicroSeconds(d.value * 1000);
    }

    bool operator==(uint64_t other) const
    {
        return other == this->d.value;;
    }


    //private:
    friend class MicroSeconds;
    struct
    {
        uint64_t value ;
    } d;
};






}




Silica::MilliSeconds operator ""_ms(unsigned long long);
Silica::MicroSeconds operator ""_us(unsigned long long);




#endif // SILICA_UNITS_OF_TIME_H
