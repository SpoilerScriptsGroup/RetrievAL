#ifndef _M_IX86
#include <stdint.h>
#include <float.h>

#ifndef DBL_EXP_BIAS
#ifdef _DBIAS
#define DBL_EXP_BIAS (_DBIAS + 1)
#else
#define DBL_EXP_BIAS (DBL_MAX_EXP - 1)
#endif
#endif

double __cdecl frexp(double x, int *expptr)
{
	uintptr_t sign;
	int32_t exp;

	sign = (*(uint64_t *)&x >> (64 - sizeof(uintptr_t) * 8)) & INTPTR_MIN;
	*(uint64_t *)&x &= INT64_MAX;
	if (*(uint64_t *)&x & 0x7FFFFFFFFFFFFFFF)
	{
		if (*(uint64_t *)&x < 0x7FF0000000000000)
		{
			exp = (*(uint64_t *)&x & 0x7FF0000000000000) >> 52;
			*(uint64_t *)&x &= 0x000FFFFFFFFFFFFF;
			if (!exp)
			{
				while (!((*(uint64_t *)&x <<= 1) & 0x0010000000000000))
					exp--;
				*(uint64_t *)&x &= 0x000FFFFFFFFFFFFF;
			}
			*(uint64_t *)&x |= (uint64_t)(DBL_EXP_BIAS - 1) << 52;
			exp -= DBL_EXP_BIAS - 1;
		}
		else
		{
			exp = -1;
		}
	}
	else
	{
		exp = 0;
	}
	*(uint64_t *)&x |= (uint64_t)sign << (64 - sizeof(uintptr_t) * 8);
	*expptr = exp;
	return x;
}
#else
__declspec(naked) double frexp(double x, int *expptr)
{
	extern const double fpconst_half;
	extern const double fpconst_one;
	#define _half fpconst_half
	#define _one  fpconst_one

	__asm
	{
		fld     qword ptr [esp + 4]             ; Load real from stack
		mov     ecx, dword ptr [esp + 12]       ; Put exponent address in ecx
		fxam                                    ; Examine st
		fstsw   ax                              ; Get the FPU status word
		and     ah, 45H                         ; Isolate C0, C2 and C3
		cmp     ah, 40H                         ; Zero ?
		je      L1                              ; Re-direct if x == 0
		test    ah, 01H                         ; NaN or infinity ?
		jnz     L2                              ; Re-direct if x is NaN or infinity
		fxtract                                 ; Get exponent and significand
		fmul    qword ptr [_half]               ; Significand * 0.5
		fxch                                    ; Swap st, st(1)
		fadd    qword ptr [_one]                ; Increment exponent
		fistp   dword ptr [ecx]                 ; Store result exponent and pop
		ret

		align   16
	L1:
		mov     dword ptr [ecx], 0              ; Store exponent to zero
		ret

		align   16
	L2:
		mov     dword ptr [ecx], -1             ; Store exponent to -1
		ret
	}

	#undef _half
	#undef _one
}
#endif
