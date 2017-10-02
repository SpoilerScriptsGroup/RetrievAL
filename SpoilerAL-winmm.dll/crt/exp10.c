#ifndef _M_IX86
#include <math.h>

double __cdecl exp10(double x)
{
	return pow(10.0 x);
}
#else
__declspec(naked) double __cdecl exp10(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]     ; Load real from stack
		sub     esp, 8                  ; Allocate temporary space
		fldl2t                          ; Load log base 2 of 10
		fmulp   st(1), st(0)            ; Multiply x * log base 2(e)
		fst     st(1)                   ; Push result
		frndint                         ; Round to integer
		fsub    st(1), st(0)            ; Subtract
		fxch                            ; Exchange st, st(1)
		f2xm1                           ; Compute 2 to the (x - 1)
		fld1                            ; Load real number 1
		fadd                            ; 2 to the x
		fscale                          ; Scale by power of 2
		fstp    st(1)                   ; Set new stack top and pop
		fst     qword ptr [esp]         ; Throw away scale factor
		add     esp, 8                  ; Deallocate temporary space
		ret
	}
}
#endif
