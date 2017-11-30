#ifndef _M_IX86
#include <float.h>

#ifndef DBL_EXP_BIAS
#define DBL_EXP_BIAS (DBL_MAX_EXP - 1)
#endif

#define LSW(x) ((unsigned long int *)&(x))[0]
#define MSW(x) ((unsigned long int *)&(x))[1]

double __cdecl round(double x)
{
	unsigned long int lsw, msw;
	long int          exp;
	unsigned long int mask;
	unsigned long int u;

	lsw = LSW(x);
	msw = MSW(x);
	exp = ((msw >> 20) & 0x7FF) - DBL_EXP_BIAS;
	if (exp < 20)
	{
		if (exp < 0)
		{
			msw &= 0x80000000;
			if (exp == -1)
				msw |= (DBL_EXP_BIAS << 20);
			MSW(x) = msw;
			LSW(x) = 0;
		}
		else
		{
			mask = 0x000FFFFF >> exp;
			if ((msw & mask) || lsw)
			{
				msw += 0x00080000 >> exp;
				MSW(x) = msw & ~mask;
				LSW(x) = 0;
			}
		}
	}
	else if (exp > 51)
	{
		if (exp == 1024)
			x += x;
	}
	else
	{
		mask = 0xFFFFFFFF >> (exp - 20);
		if (lsw & mask)
		{
			u = lsw + (1 << (51 - exp));
			if (u < lsw)
				MSW(x) = msw + 1;
			LSW(x) = u & ~mask;
		}
	}
	return x;
}
#else
__declspec(naked) double __cdecl round(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		fstcw   word ptr [esp + 4]          ; Save control word
		fclex                               ; Clear exceptions
		mov     word ptr [esp + 8], 0363H   ; Rounding control word
		fldcw   word ptr [esp + 8]          ; Set new rounding control
		frndint                             ; Round to integer
		fclex                               ; Clear exceptions
		fldcw   word ptr [esp + 4]          ; Restore control word
		ret
	}
}
#endif
