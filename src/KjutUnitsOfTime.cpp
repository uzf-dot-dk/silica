#include <kjut/UnitsOfTime.h>



Kjut::MilliSeconds operator ""_ms(unsigned long long value)
{
    return Kjut::MilliSeconds(value);
}

Kjut::MicroSeconds operator ""_us(unsigned long long value)
{
    return Kjut::MicroSeconds(value);
}

