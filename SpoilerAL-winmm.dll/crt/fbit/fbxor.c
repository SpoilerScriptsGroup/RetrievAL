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

			if ((shift = -shift) > DBL_MANT_BIT)
				goto XOR_SIGN;
			z = x;
			x = y;
			y = z;
			exp2 = exp1;
		}
		mant = (y & DBL_MANT_MASK) | (exp2 ? DBL_MANT_NORM : 0);
		mant >>= shift;
		x ^= mant | (y & DBL_SIGN_MASK);
	}
	else
	{
		mant = (x & DBL_MANT_MASK) | (exp1 ? DBL_MANT_NORM : 0);
		if (mant ^= (y & DBL_MANT_MASK) | (exp1 ? DBL_MANT_NORM : 0))
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
			x = ((x & DBL_SIGN_MASK) & (y & DBL_SIGN_MASK)) | ((uint64_t)exp1 << DBL_MANT_BIT) | (mant & DBL_MANT_MASK);
		}
		else
		{
		XOR_SIGN:
			x ^= y & DBL_SIGN_MASK;
		}
	}

	#undef x
	#undef y

	return x;
}
