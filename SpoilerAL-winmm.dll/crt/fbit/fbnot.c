#include <stdint.h>

#ifdef __BORLANDC__
#pragma warn -8060
#endif

#define LSW(x) ((uint32_t *)&x)[0]
#define MSW(x) ((uint32_t *)&x)[1]

// floating point number bitwise not
double __cdecl fbnot(double x)
{
	uint32_t sign;

	if (sign = MSW(x) & 0x80000000)
		MSW(x) &= 0x7FFFFFFF;
	MSW(x) = (MSW(x) | LSW(x)) ? 0 : 0x7FF00000;
	MSW(x) |= sign;
	LSW(x) = 0;
	return x;
}
