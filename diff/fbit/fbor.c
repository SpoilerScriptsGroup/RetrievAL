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
	#define x             *(uint64_t *)&x
	#define y             *(uint64_t *)&y
	#define DBL_SIGN_WORD (DBL_SIGN_MASK >> ((sizeof(uint64_t) - sizeof(uintptr_t)) * 8))
	#define MSW(a)        *(uintptr_t *)((char *)&(a) + sizeof(uint64_t) - sizeof(uintptr_t))

	uint64_t  r;
	uintptr_t sign;
	int32_t   exp, shift;
	uint64_t  mant;

	sign = ((int64_t)x < 0 && (x &= INT64_MAX)) ? DBL_SIGN_WORD : 0;
	if ((int64_t)y < 0 && (y &= INT64_MAX))
		sign |= DBL_SIGN_WORD;
	if (MSW(x) < MSW(y))
	{
		uint64_t z;

		z = x;
		x = y;
		y = z;
	}
	r = 0;
	exp = (int32_t)(y >> DBL_MANT_BIT);
	if (shift = (int32_t)(x >> DBL_MANT_BIT) - exp)
	{
		if (shift > DBL_MANT_BIT)
			goto APPEND_SIGN;
		mant = (y & DBL_MANT_MASK) | (exp ? DBL_MANT_NORM : 0);
		mant >>= shift;
		r = x | mant;
	}
	else
	{
		r = x | y;
	}

APPEND_SIGN:
	r |= (uint64_t)sign << ((sizeof(uint64_t) - sizeof(uintptr_t)) * 8);

	return *(double *)&r;

	#undef x
	#undef y
	#undef DBL_SIGN_WORD
	#undef MSW
}
