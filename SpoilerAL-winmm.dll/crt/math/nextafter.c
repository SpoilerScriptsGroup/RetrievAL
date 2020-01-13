#include <errno.h>
#include <stdint.h>

#define I64(x)  (*(int64_t *)&(x))
#define UI64(x) (*(uint64_t *)&(x))

double __cdecl _nextafter(double x, double y)
{
    if (((UI64(x) & INT64_MAX) > 0x7FF0000000000000) || // x is NaN
        ((UI64(y) & INT64_MAX) > 0x7FF0000000000000))   // y is NaN
        return x + y;
    if (UI64(x) == UI64(y))                             // x == y
        return y;
    if (!(UI64(x) & INT64_MAX)) {                       // x == 0
        UI64(y) &= INT64_MIN;
        UI64(y) |= 1;
        errno = ERANGE;
        return y;
    }
    if (I64(x) >= I64(y))                               // x >= y
        UI64(x)--;
    else                                                // x < y
        UI64(x)++;
    if (UI64(x) >= 0x7FF0000000000000 ||                // overflow
        UI64(x) < 0x0010000000000000)                   // underflow
        errno = ERANGE;
    return x;
}
