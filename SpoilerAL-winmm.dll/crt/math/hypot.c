#include <math.h>
#include <stdint.h>

#define I64(x)      (*(int64_t *)&(x))
#define UI64(x)     (*(uint64_t *)&(x))
#define ISFINITE(x) ((UI64(x) & 0x7FF0000000000000) < 0x7FF0000000000000)
#define ISINF(x)    ((UI64(x) & 0x7FFFFFFFFFFFFFFF) == 0x7FF0000000000000)
#define ABSI64(x)   ((x) & 0x7FFFFFFFFFFFFFFF)

#pragma warning(disable:4273)

double __cdecl _hypot(double x, double y)
{
	if (ISFINITE(x) && ISFINITE(y))
	{
		int e;

		/* Determine absolute values. */
		UI64(x) = ABSI64(I64(x));
		UI64(y) = ABSI64(I64(y));

		/* Find the bigger and the smaller one. */
		if (UI64(x) < UI64(y))
		{
			uint64_t z;

			z = UI64(x);
			UI64(x) = UI64(y);
			UI64(y) = z;
		}
		/* Now 0 <= y <= x. */

		/* Write x = x * 2^e, y = y * 2^e with 0 <= y <= x < 1. */
		x = frexp(x, &e);
		y = ldexp(y, -e);

		/* Through the normalization, no unneeded overflow or underflow
		   will occur here. */
		return ldexp(sqrt(x * x + y * y), e);
	}
	else
	{
		if (ISINF(x) || ISINF(y))
			/* x or y is infinite.  Return +Infinity. */
			return HUGE_VAL;
		else
			/* x or y is NaN.  Return NaN. */
			return x + y;
	}
}
