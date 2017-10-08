#include <float.h>

#pragma function(floor)

#ifndef _M_IX86
#ifndef _HUGE_ENUF
#define _HUGE_ENUF 1e+300
#endif

#ifndef DBL_EXP_BIAS
#define DBL_EXP_BIAS (DBL_MAX_EXP - 1)
#endif

#define LSW(value) \
	*(unsigned long int *)&(value)
#define MSW(value) \
	*((unsigned long int *)&(value) + 1)

double __cdecl floor(double x)
{
	unsigned long int lsw;
	long int          msw;
	long int          exp;
	unsigned long int mask;
	unsigned long int u;

	lsw = LSW(x);
	msw = MSW(x);
	exp = (((unsigned long int)msw >> 20) & 0x7FF) - DBL_EXP_BIAS;
	if (exp < 20)
	{
		if (exp < 0)
		{
			if (_HUGE_ENUF + x > 0)
			{
				if (msw >= 0)
				{
					MSW(x) = LSW(x) = 0;
				}
				else if ((msw & 0x7FFFFFFF) || lsw)
				{
					LSW(x) = 0;
					MSW(x) = 0xBFF00000;
				}
			}
		}
		else
		{
			mask = (0x000FFFFF) >> exp;
			if (((msw & mask) || lsw) && _HUGE_ENUF + x > 0)
			{
				LSW(x) = 0;
				if (msw < 0)
					msw += (0x00100000) >> exp;
				MSW(x) = msw & ~mask;
			}
		}
	}
	else if (exp > 51)
	{
		if (exp == 0x400)
			x += x;
	}
	else
	{
		mask = 0xFFFFFFFFU >> (exp - 20);
		if ((lsw & mask) && _HUGE_ENUF + x > 0)
		{
			if (msw < 0)
			{
				if (exp == 20)
					MSW(x) = msw + 1;
				else
				{
					u = lsw + (1 << (52 - exp));
					if (u < lsw)
						MSW(x) = msw + 1;
					lsw = u;
				}
			}
			LSW(x) = lsw & ~mask;
		}
	}
	return x;
}
#else
__declspec(naked) double __cdecl floor(double x)
{
	__asm
	{
		push    eax                     ; Allocate temporary space
		fld     qword ptr [esp + 8]     ; Load real from stack
		fstcw   word ptr [esp + 2]      ; Save control word
		fclex                           ; Clear exceptions
		mov     word ptr [esp], 0763H   ; Rounding control word
		fldcw   word ptr [esp]          ; Set new rounding control
		frndint                         ; Round to integer
		fclex                           ; Clear exceptions
		fldcw   word ptr [esp + 2]      ; Restore control word
		pop     eax                     ; Deallocate temporary space
		ret
	}
}
#endif
