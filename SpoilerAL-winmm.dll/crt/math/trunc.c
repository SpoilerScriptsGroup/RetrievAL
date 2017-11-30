#ifndef _M_IX86
#include <float.h>

#ifndef _HUGE_ENUF
#define _HUGE_ENUF 1e+300
#endif

#ifndef DBL_EXP_BIAS
#define DBL_EXP_BIAS (DBL_MAX_EXP - 1)
#endif

#define LSW(x) ((unsigned long int *)&(x))[0]
#define MSW(x) ((unsigned long int *)&(x))[1]

double __cdecl trunc(double x)
{
	unsigned long int lsw;
	long int          msw;
	long int          exp;
	unsigned long int mask;

	lsw = LSW(x);
	msw = MSW(x);
	exp = (((unsigned long int)msw >> 20) & 0x7FF) - DBL_EXP_BIAS;
	if (exp < 20)
	{
		if (exp < 0)
		{
			if (_HUGE_ENUF + x > 0)
			{
				LSW(x) = 0;
				MSW(x) = msw & 0x80000000;
			}
		}
		else
		{
			mask = 0x000FFFFFU >> exp;
			if ((msw & mask) || lsw)
			{
				if (_HUGE_ENUF + x > 0)
				{
					LSW(x) = 0;
					MSW(x) = msw & ~mask;
				}
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
			LSW(x) = lsw & ~mask;
	}
	return x;
}
#else
__declspec(naked) double __cdecl trunc(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		fstcw   word ptr [esp + 4]          ; Save control word
		fclex                               ; Clear exceptions
		mov     word ptr [esp + 8], 0F63H   ; Rounding control word
		fldcw   word ptr [esp + 8]          ; Set new rounding control
		frndint                             ; Round to integer
		fclex                               ; Clear exceptions
		fldcw   word ptr [esp + 4]          ; Restore control word
		ret
	}
}
#endif
