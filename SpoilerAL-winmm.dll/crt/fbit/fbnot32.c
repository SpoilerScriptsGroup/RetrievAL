#include <float.h>
#include <stdint.h>
#ifdef _MSC_VER
#include <intrin.h>
#ifdef _WIN64
#pragma intrinsic(_BitScanReverse64)
#else
#pragma intrinsic(_BitScanReverse)
#endif
#else
#include "intrinsic.h"
#endif

#ifdef __BORLANDC__
#pragma warn -8060
#endif

#define DBL_EXP_BIAS  (DBL_MAX_EXP - 1)     // 1023
#define DBL_MANT_BIT  (DBL_MANT_DIG - 1)    // 52
#define DBL_EXP_MASK  0x7FF0000000000000
#define DBL_MANT_MASK 0x000FFFFFFFFFFFFF
#define DBL_MANT_NORM 0x0010000000000000

// floating point number bitwise not
double __cdecl fbnot32(double x)
{
	int32_t exp;

	#define x *(uint64_t *)&x

	if ((exp = ((uint32_t)(x >> 32) & (DBL_EXP_MASK >> 32))) != (DBL_EXP_MASK >> 32))
	{
		exp = (exp >> (DBL_MANT_BIT - 32)) - DBL_EXP_BIAS;
		if (exp >= 0 && (exp >= DBL_MANT_BIT || !(x << (exp + (64 - DBL_MANT_BIT)))))
			*(double *)&x = ~(uint32_t)*(double *)&x;
		else
		{
			uint64_t mant;
			int32_t  shift;

			mant = (x & DBL_MANT_MASK) | DBL_MANT_NORM;
			if ((shift = (32 - 1) - exp) >= 0)
				mant = shift <= DBL_MANT_BIT ? mant >> shift : 0;
			else
				mant <<= -shift;
			if ((int64_t)x < 0 && (x & INT64_MAX))
				mant = -(int64_t)mant;
			mant = ~mant;
			exp = DBL_EXP_BIAS + 32 - 1;
			if (!(mant & DBL_MANT_NORM))
			{
				unsigned long index;

				mant &= DBL_MANT_MASK | DBL_MANT_NORM;
#ifdef _WIN64
				_BitScanReverse64(&index, mant);
#else
				if ((unsigned long)(mant >> 32))
				{
					_BitScanReverse(&index, (unsigned long)(mant >> 32));
					index += 32;
				}
				else
					_BitScanReverse(&index, (unsigned long)mant);
#endif
				index = DBL_MANT_BIT - index;
				exp -= index;
				mant <<= index;
			}
			x = ((uint64_t)exp << DBL_MANT_BIT) | (mant & DBL_MANT_MASK);
		}
	}
	else
		*(double *)&x = ((int64_t)x >= 0 || !(x & INT64_MAX)) ? UINT32_MAX : 0;

	#undef x

	return x;
}
