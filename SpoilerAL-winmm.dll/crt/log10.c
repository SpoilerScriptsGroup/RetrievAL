#pragma function(log10)

#ifndef _M_IX86
#include <math.h>

#ifndef M_LOG10E
#define M_LOG10E 0.434294481903251827651128918916605082
#endif

double __cdecl log10(double x)
{
	return log(x) * M_LOG10E;
}
#else
__declspec(naked) double __cdecl log10(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]     ; Load real from stack
		fldlg2                          ; Load log base 10 of 2
		fxch    st(1)                   ; Exchange st0, st1
		fyl2x                           ; Compute the log base 10(x)
		ret
	}
}
#endif
