#include <math.h>
#include <float.h>
#include <errno.h>
#include <stdint.h>

#define static_cast(type, variable) (*(type *)&(variable))

#define DBL_MANT_BIT  (DBL_MANT_DIG - DBL_HAS_SUBNORM)
#define DBL_MANT_MASK ((UINT64_C(1) << DBL_MANT_BIT) - 1)

#pragma warning(disable:4273)

double __cdecl _hypot(double x, double y)
{
	uint64_t a, b;

	/* Determine absolute values. */
	a = static_cast(uint64_t, x) & INT64_MAX;
	b = static_cast(uint64_t, y) & INT64_MAX;

	/* Are x and y finite? */
	if (a < 0x7FF0000000000000 && b < 0x7FF0000000000000)
	{
		int e;

		/* Find the bigger and the smaller one. */
		if (a >= b)
		{
			x = static_cast(double, a);
			y = static_cast(double, b);
		}
		else
		{
			x = static_cast(double, b);
			y = static_cast(double, a);
		}
		/* Now 0 <= y <= x. */

		/* Write x = x * 2^e, y = y * 2^e with 0 <= y <= x < 1. */
		x = frexp(x, &e);
		y = ldexp(y, -e);

		/* Through the normalization, no unneeded overflow or underflow
		   will occur here. */
		x = ldexp(sqrt(x * x + y * y), e);

		/* The errno variable is set to ERANGE on overflow. */
		if (x == HUGE_VAL)
			errno = ERANGE;
	}
	else if (!(a & DBL_MANT_MASK) || !(b & DBL_MANT_MASK))
	{
		/* x or y is infinite.  Return +Infinity. */
		errno = ERANGE;
		x = HUGE_VAL;
	}
	else
	{
		/* x or y is NaN.  Return NaN. */
		errno = EDOM;
		x += y;
	}
	return x;
}
