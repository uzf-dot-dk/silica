#include <silica/UnitsOfTime.h>



Silica::MilliSeconds operator ""_ms(unsigned long long value)
{
    return Silica::MilliSeconds(value);
}

Silica::MicroSeconds operator ""_us(unsigned long long value)
{
    return Silica::MicroSeconds(value);
}

