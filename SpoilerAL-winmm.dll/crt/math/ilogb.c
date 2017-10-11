#include <math.h>
#include <float.h>
#include <limits.h>

#ifndef DBL_EXP_BIAS
#define DBL_EXP_BIAS (DBL_MAX_EXP - 1)
#endif

#define BITS(value) *(unsigned long long *)&(value)
#define LSW(x) ((unsigned long int *)&(x))[0]
#define MSW(x) ((unsigned long int *)&(x))[1]

int __cdecl _ilogb(double x)
{
	long int e;

	e = ((MSW(x) >> 20) & 0x7FF);
	if (!e)
		if (BITS(x) <<= 12)
		{
			e = -DBL_EXP_BIAS;
			if ((long long)BITS(x) >= 0)
				do
					e--;
				while ((long long)(BITS(x) <<= 1) >= 0);
		}
		else
			e = FP_ILOGB0;
	else if (e == 0x7FF)
		e = (LSW(x) || (MSW(x) & 0x000FFFFF)) ? FP_ILOGBNAN : INT_MAX;
	else
		e -= DBL_EXP_BIAS;
	return e;
}
