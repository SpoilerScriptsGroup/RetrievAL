#include <float.h>
#include <stdint.h>

#ifdef __BORLANDC__
#pragma warn -8060
#endif

#define DBL_MANT_BIT  (DBL_MANT_DIG - 1)    // 52
#define DBL_SIGN_MASK 0x8000000000000000
#define DBL_EXP_MASK  0x7FF0000000000000
#define DBL_MANT_MASK 0x000FFFFFFFFFFFFF
#define DBL_MANT_NORM 0x0010000000000000

// floating point number bitwise or
double __cdecl fbor(double x, double y)
{
	uint64_t mant;
	int32_t  exp1, exp2, shift;

	#define x *(uint64_t *)&x
	#define y *(uint64_t *)&y

	exp1 = (int32_t)((x & DBL_EXP_MASK) >> DBL_MANT_BIT);
	exp2 = (int32_t)((y & DBL_EXP_MASK) >> DBL_MANT_BIT);
	if (shift = exp1 - exp2)
	{
		if (shift >= 0)
		{
			if (shift > DBL_MANT_BIT)
				goto OR_SIGN;
		}
		else
		{
			uint64_t z;

			z = x;
			x = y;
			y = z;
			if ((shift = -shift) > DBL_MANT_BIT)
				goto OR_SIGN;
			exp2 = exp1;
		}
		mant = (y & DBL_MANT_MASK) | (exp2 ? DBL_MANT_NORM : 0);
		mant >>= shift;
		x |= mant;
	OR_SIGN:
		if (y & ~DBL_SIGN_MASK)
			x |= y & DBL_SIGN_MASK;
	}
	else
	{
		x |= y;
	}

	#undef x
	#undef y

	return x;
}
