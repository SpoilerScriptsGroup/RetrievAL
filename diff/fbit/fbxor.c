#include <float.h>
#include <stdint.h>
#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(_BitScanReverse)
#else
#include "intrinsic.h"
#endif

#ifdef __BORLANDC__
#pragma warn -8060
#endif

#define DBL_MANT_BIT  (DBL_MANT_DIG - 1)    // 52
#define DBL_SIGN_MASK 0x8000000000000000
#define DBL_EXP_MASK  0x7FF0000000000000
#define DBL_MANT_MASK 0x000FFFFFFFFFFFFF
#define DBL_MANT_NORM 0x0010000000000000

// floating point number bitwise xor
double __cdecl fbxor(double x, double y)
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
				goto XOR_SIGN;
		}
		else
		{
			uint64_t z;

			z = x;
			x = y;
			y = z;
			if ((shift = -shift) > DBL_MANT_BIT)
				goto XOR_SIGN;
			exp2 = exp1;
		}
		mant = (y & DBL_MANT_MASK) | (exp2 ? DBL_MANT_NORM : 0);
		mant >>= shift;
		x ^= mant | (y & DBL_SIGN_MASK);
		goto DONE;

	XOR_SIGN:
		x ^= y & DBL_SIGN_MASK;
	}
	else
	{
		uint32_t sign;

		sign = (uint32_t)((x & DBL_SIGN_MASK) >> 32) ^ (uint32_t)((y & DBL_SIGN_MASK) >> 32);
		mant = x & DBL_MANT_MASK;
		if (mant ^= y & DBL_MANT_MASK)
		{
			unsigned long index;

			if ((unsigned long)(mant >> 32))
			{
				_BitScanReverse(&index, (unsigned long)(mant >> 32));
				index += 32;
			}
			else
			{
				_BitScanReverse(&index, (unsigned long)mant);
			}
			if (index = DBL_MANT_BIT - index)
			{
				exp1 -= index;
				mant <<= index;
			}
			x = ((uint64_t)sign << 32) | ((uint64_t)exp1 << DBL_MANT_BIT) | (mant & DBL_MANT_MASK);
		}
		else
		{
			x = (uint64_t)sign << 32;
		}
	}
DONE:

	#undef x
	#undef y

	return x;
}
