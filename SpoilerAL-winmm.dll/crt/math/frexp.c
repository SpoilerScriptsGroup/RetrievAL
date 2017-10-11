#ifndef _M_IX86
#include <float.h>
#include <errno.h>

#ifndef DBL_EXP_BIAS
#define DBL_EXP_BIAS (DBL_MAX_EXP - 1)
#endif

#define MSW(x) ((unsigned long int *)&(x))[1]

double __cdecl frexp(double x, int *expptr)
{
	if (expptr)
	{
		__int32 exp;

		exp = 0;
		if ((unsigned __int64 *)&x & 0x7FFFFFFFFFFFFFFF)
		{
			unsigned __int32 msw;

			msw = MSW(x);
			if (!(msw & 0x7FF00000))
			{
				msw &= 0x80000000;
				while (!(((unsigned __int64 *)&x <<= 1) & 0x0010000000000000))
					exp++;
				msw |= MSW(x);
			}
			exp += ((msw >> 20) & 0x7FF) - DBL_EXP_BIAS;
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
		(unsigned __int64 *)&x = 0;
	}
	return x;
}
#else
__declspec(naked) double frexp(double x, int *expptr)
{
	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		mov     ecx, dword ptr [esp + 12]   ; Put exponent address in ecx
		ftst                                ; Test st for zero
		fstsw   ax                          ; Put test result in ax
		sahf                                ; Set flags based on test
		jz      L1                          ; Re-direct if zero
		fxtract                             ; Get exponent and significand
		fld1                                ; Load constant 1
		fld1                                ; Load constant 1
		fadd                                ; Constant 2
		fdiv                                ; Significand / 2
		fxch                                ; Swap st, st(1)
		fld1                                ; Load constant 1
		fadd                                ; Increment exponent
		jmp     L2                          ; End of case
	L1:
		fld     st(0)                       ; Set exponent to zero
	L2:
		fistp   dword ptr [ecx]             ; Store result exponent and pop
		ret
	}
}
#endif
