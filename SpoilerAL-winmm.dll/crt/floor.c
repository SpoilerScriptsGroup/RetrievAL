#include <float.h>

#pragma function(floor)

#ifndef _HUGE_ENUF
#define _HUGE_ENUF 1e+300
#endif

#ifndef DBL_EXP_BIAS
#define DBL_EXP_BIAS (DBL_MAX_EXP - 1)
#endif

#define LSW(value) \
	*(unsigned long int *)&(value)
#define MSW(value) \
	*((unsigned long int *)&(value) + 1)

double __cdecl floor(double x)
{
	unsigned long int lsw, msw;
	long int          exp;
	unsigned long int mask;
	unsigned long int u;

	lsw = LSW(x);
	msw = MSW(x);
	exp = ((msw >> 20) & 0x7FF) - DBL_EXP_BIAS;
	if (exp < 20)
	{
		if (exp < 0)
		{
			if (_HUGE_ENUF + x > 0)
			{
				if (msw >= 0)
				{
					msw = lsw = 0;
				}
				else if ((msw & 0x7FFFFFFF) || lsw)
				{
					lsw = 0;
					msw = 0xBFF00000;
				}
			}
		}
		else
		{
			mask = (0x000FFFFF) >> exp;
			if (!(msw & mask) && !lsw)
				return x;
			if (_HUGE_ENUF + x > 0)
			{
				lsw = 0;
				if (msw < 0)
					msw += (0x00100000) >> exp;
				msw &= ~mask;
			}
		}
	}
	else if (exp > 51)
	{
		if (exp == 0x400)
			return x + x;
		else
			return x;
	}
	else
	{
		mask = 0xFFFFFFFFU >> (exp - 20);
		if (!(lsw & mask))
			return x;
		if (_HUGE_ENUF + x > 0)
		{
			if (msw < 0)
			{
				if (exp == 20)
					msw++;
				else
				{
					u = lsw + (1 << (52 - exp));
					if (u < lsw)
						msw++;
					lsw = u;
				}
			}
			lsw &= ~mask;
		}
	}
	LSW(x) = lsw;
	MSW(x) = msw;
	return x;
}
