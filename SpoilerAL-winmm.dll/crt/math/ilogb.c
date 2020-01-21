#ifndef _M_IX86
#include <math.h>
#include <float.h>
#include <limits.h>
#include <stdint.h>
#if defined(_MSC_VER) && _MSC_VER >= 1310
#include <intrin.h>
#if defined(_M_IX86)
#pragma intrinsic(_BitScanReverse)
#elif defined(_M_X64)
#pragma intrinsic(_BitScanReverse64)
#endif
#endif

#ifndef DBL_EXP_BIAS
#ifdef _DBIAS
#define DBL_EXP_BIAS (_DBIAS + 1)
#else
#define DBL_EXP_BIAS (DBL_MAX_EXP - 1)
#endif
#endif
#define DBL_MANT_BIT  (DBL_MANT_DIG - DBL_HAS_SUBNORM)
#define DBL_MANT_MASK ((1ULL << DBL_MANT_BIT) - 1)
#define DBL_SIGN_BIT  1
#define DBL_SIGN_MASK 1
#define DBL_EXP_BIT   (64 - DBL_SIGN_BIT - DBL_MANT_BIT)
#define DBL_EXP_MASK  ((1 << DBL_EXP_BIT) - 1)

#define BITS(x) *(uint64_t *)&(x)
#define LSW(x) ((uint32_t *)&(x))[0]
#define MSW(x) ((uint32_t *)&(x))[1]

int __cdecl ilogb(double x)
{
#if INTPTR_MAX >= INT64_MAX
	int64_t i;
	int32_t e;

	if (e = (i = BITS(x) & INT64_MAX) >> DBL_MANT_BIT)
	{
		if (e != DBL_EXP_MASK)
			return e - DBL_EXP_BIAS;
#if FP_ILOGBNAN != INT_MAX
		else if (i & DBL_MANT_MASK)
			return FP_ILOGBNAN;
#endif
		else
			return INT_MAX;
	}
	if (BITS(x))
	{
#if defined(_MSC_VER) && _MSC_VER >= 1310 && defined(_M_X64)
		_BitScanReverse64((unsigned long *)&e, i);
		return e - DBL_EXP_BIAS - DBL_MANT_BIT + 1;
#else
		e = -DBL_EXP_BIAS;
		for (i <<= 64 - DBL_MANT_BIT; i >= 0; i <<= 1)
			e--;
		return e;
#endif
	}
	return FP_ILOGB0;
#else
	uint32_t msw, lsw;
	int32_t e;

	msw = MSW(x);
	lsw = LSW(x);
	if (e = (msw &= INT32_MAX) >> (DBL_MANT_BIT - 32))
	{
		if (e != DBL_EXP_MASK)
			return e - DBL_EXP_BIAS;
#if FP_ILOGBNAN != INT_MAX
		else if ((msw & (DBL_MANT_MASK >> 32)) || lsw)
			return FP_ILOGBNAN;
#endif
		else
			return INT_MAX;
	}
	else
	{
#if defined(_MSC_VER) && _MSC_VER >= 1310 && defined(_M_IX86)
		if (msw)
		{
			_BitScanReverse((unsigned long *)&e, msw);
			return e - DBL_EXP_BIAS - DBL_MANT_BIT + 33;
		}
		if (lsw)
		{
			_BitScanReverse((unsigned long *)&e, lsw);
			return e - DBL_EXP_BIAS - DBL_MANT_BIT + 1;
		}
		return FP_ILOGB0;
#else
		int32_t i;

		if (msw)
		{
			i = msw << (64 - DBL_MANT_BIT);
			e = -DBL_EXP_BIAS;
		}
		else if (lsw)
		{
			i = lsw;
			e = -DBL_EXP_BIAS - DBL_MANT_BIT + 32;
		}
		else
			return FP_ILOGB0;
		for (; i >= 0; i <<= 1)
			e--;
		return e;
#endif
	}
#endif
}
#else
__declspec(naked) int __cdecl ilogb(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]
		fxam                                /* Is NaN or +-Inf?  */
		fstsw   ax
		and     ah, 45H
		cmp     ah, 05H
		je      L1                          /* Is +-Inf, jump.  */
		cmp     ah, 40H
		je      L2                          /* Is +-0, jump.  */
		fxtract
		push    eax
		fstp    st(0)
		fistp   dword ptr [esp]
		fwait
		pop     eax
		ret

		align   16
	L1:
		fstp    st(0)
		mov     eax, 7FFFFFFFH
		ret

		align   16
	L2:
		fstp    st(0)
		mov     eax, 80000000H              /* FP_ILOGB0  */
		ret
	}
}
#endif
