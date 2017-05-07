#include <float.h>

#ifndef DBL_EXP_BIAS
#define DBL_EXP_BIAS (DBL_MAX_EXP - 1)
#endif

#define LSW(value) \
	*(unsigned long int *)&(value)
#define MSW(value) \
	*((unsigned long int *)&(value) + 1)

double __cdecl round(double x)
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
			msw &= 0x80000000;
			if (exp == -1)
				msw |= (DBL_EXP_BIAS << 20);
			lsw = 0;
		}
		else
		{
			mask = 0x000FFFFF >> exp;
			if (!(msw & mask) && !lsw)
				return x;
			msw += 0x00080000 >> exp;
			msw &= ~mask;
			lsw = 0;
		}
	}
	else if (exp > 51)
	{
		if (exp == 1024)
			return x + x;
		else
			return x;
	}
	else
	{
		mask = 0xFFFFFFFF >> (exp - 20);
		if (!(lsw & mask))
			return x;
		u = lsw + (1 << (51 - exp));
		if (u < lsw)
			msw += 1;
		lsw = u;
		lsw &= ~mask;
	}
	LSW(x) = lsw;
	MSW(x) = msw;
	return x;
}
