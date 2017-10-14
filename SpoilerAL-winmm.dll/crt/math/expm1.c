#include <math.h>

#pragma function(exp)

extern const double _half;

__declspec(naked) double __cdecl expm1(double x)
{
	static const double range = 1e-5;

	__asm
	{
		emms
		fld     qword ptr [esp + 4]     ; Load real from stack
		fld     qword ptr [range]       ; Load 1e-5
		fld     st(1)                   ; Duplicate x
		fabs                            ; Take the absolute value
		fcompp                          ; fabs(x) > 1e-5 ?
		fstsw   ax                      ; Get the FPU status word
		sahf                            ; Set flags based on test
		ja      L1                      ; Re-direct if fabs(x) > 1e-5
		fld     st(0)                   ; Duplicate x
		fld     st(0)                   ; Duplicate x
		fld     qword ptr [_half]       ; Load 0.5
		fmul                            ; Compute 0.5 * x * x + x
		fmul
		fadd
		ret
	L1:
		sub     esp, 8                  ; Allocate stack space for x
		fstp    qword ptr [esp]         ; Copy x onto stack
		call    exp                     ; Call exp
		add     esp, 8                  ; Remove x from stack
		fld1                            ; Load real number 1
		fsub                            ; Subtract
		ret
	}
}
