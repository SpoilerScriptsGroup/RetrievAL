#include <float.h>

#ifndef DBL_EXP_BIAS
#define DBL_EXP_BIAS (DBL_MAX_EXP - 1)
#endif

#define MSW(value) \
	*((unsigned long int *)&(value) + 1)

double __cdecl frexp(double x, int *expptr)
{
	if (x)
	{
		unsigned long int msw;

		msw = MSW(x);
		*expptr = ((msw >> 20) & 0x7FF) - DBL_EXP_BIAS;
		MSW(x) = (msw & 0x800FFFFF) | ((DBL_EXP_BIAS - 1) << 20);
	}
	else
	{
		*expptr = 0;
		x = 0;
	}
	return x;
}
