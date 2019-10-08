#ifndef _M_IX86
#include <stdint.h>
#include <float.h>
#include <errno.h>

#ifndef DBL_EXP_BIAS
#define DBL_EXP_BIAS (DBL_MAX_EXP - 1)
#endif

#define MSW(x) ((uint32_t *)&(x))[1]

double __cdecl frexp(double x, int *expptr)
{
	if (expptr)
	{
		int32_t exp;

		exp = 0;
		if (*(uint64_t *)&x & 0x7FFFFFFFFFFFFFFF)
		{
			uint32_t msw;

			msw = MSW(x);
			if (!(msw & 0x7FF00000))
			{
				msw &= 0x80000000;
				while (!((*(uint64_t *)&x <<= 1) & 0x0010000000000000))
					exp++;
				msw |= MSW(x);
			}
			exp += ((msw & 0x7FF00000) >> 20) - (DBL_EXP_BIAS - 1);
			MSW(x) = (msw & 0x800FFFFF) | ((DBL_EXP_BIAS - 1) << 20);
		}
		else
		{
			MSW(x) = 0;
		}
		*expptr = exp;
	}
	else
	{
		errno = EINVAL;
		*(uint64_t *)&x = 0;
	}
	return x;
}
#else
#include <errno.h>

__declspec(naked) double frexp(double x, int *expptr)
{
	extern const double fpconst_half;
	extern const double fpconst_one;
	extern const double fpconst_minus_one;
	#define _half      fpconst_half
	#define _one       fpconst_one
	#define _minus_one fpconst_minus_one

#ifdef _DEBUG
	errno_t * __cdecl _errno();
	#define set_errno(x) \
		__asm   fstp    st(0)                   /* Set new top of stack */ \
		__asm   call    _errno                  /* Get C errno variable pointer */ \
		__asm   mov     dword ptr [eax], x      /* Set error number */ \
		__asm   fld     qword ptr [esp + 4]     /* Load x */
#else
	extern errno_t _terrno;
	#define set_errno(x) \
		__asm   mov     dword ptr [_terrno], x  /* Set error number */
#endif

	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		mov     ecx, dword ptr [esp + 12]   ; Put exponent address in ecx
		test    ecx, ecx                    ; Test expptr for zero
		jz      L4                          ; Re-direct if zero
		fxam                                ; Examine st
		fstsw   ax                          ; Get the FPU status word
		and     ah, 01000101B               ; Isolate C0, C2 and C3
		cmp     ah, 01000000B               ; Zero ?
		je      L2                          ; Re-direct if x == 0
		test    ah, 00000001B               ; NaN or infinity ?
		jnz     L1                          ; Re-direct if x is NaN or infinity
		fxtract                             ; Get exponent and significand
		fmul    qword ptr [_half]           ; Significand * 0.5
		fxch                                ; Swap st, st(1)
		fadd    qword ptr [_one]            ; Increment exponent
		jmp     L3                          ; End of case
	L1:
		fld     qword ptr [_minus_one]      ; Set exponent to -1
		jmp     L3                          ; End of case
	L2:
		fldz                                ; Set exponent to zero
	L3:
		fistp   dword ptr [ecx]             ; Store result exponent and pop
		ret
	L4:
		set_errno(EINVAL)                   ; Set invalid argument (EINVAL)
		ret
	}

	#undef set_errno
}
#endif
