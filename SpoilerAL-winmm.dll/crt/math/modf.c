#include <float.h>

#ifndef DBL_EXP_BIAS
#define DBL_EXP_BIAS (DBL_MAX_EXP - 1)
#endif

#define LSW(value) \
	*(unsigned long int *)&(value)
#define MSW(value) \
	*((unsigned long int *)&(value) + 1)

double __cdecl modf(double x, double *intptr)
{
	unsigned long int lsw, msw;
	long int          exp;
	unsigned long int mask;

	lsw = LSW(x);
	msw = MSW(x);
	exp = ((msw >> 20) & 0x7FF) - DBL_EXP_BIAS;
	if (exp < 20)
	{
		if (exp < 0)
		{
			LSW(*intptr) = 0;
			MSW(*intptr) = msw & 0x80000000;
		}
		else
		{
			mask = 0x000FFFFF >> exp;
			if (!(msw & mask) && !lsw)
			{
				*intptr = x;
				LSW(x) = 0;
				MSW(x) = msw & 0x80000000;
			}
			else
			{
				LSW(*intptr) = 0;
				MSW(*intptr) = msw & ~mask;
				x -= *intptr;
			}
		}
	}
	else if (exp > 51)
	{
		const double one = 1.0;

		*intptr = x * one;
		LSW(x) = 0;
		MSW(x) = msw & 0x80000000;
	}
	else
	{
		mask = 0xFFFFFFFF >> (exp - 20);
		if (!(lsw & mask))
		{
			*intptr = x;
			LSW(x) = 0;
			MSW(x) = msw & 0x80000000;
		}
		else
		{
			LSW(*intptr) = lsw & ~mask;
			MSW(*intptr) = msw;
			x -= *intptr;
		}
	}
	return x;
}
