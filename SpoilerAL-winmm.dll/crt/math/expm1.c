#include <math.h>

#pragma function(exp)

__declspec(naked) double __cdecl expm1(double x)
{
	static const double range = 1e-5;
	static const double half  = 0.5;

	__asm
	{
		fld     qword ptr [esp + 4]     ; Load real from stack
		fld     qword ptr [range]       ; Load 1e-5
		fcomp                           ; 1e-5 < x  ?
		fstsw   ax                      ; Get the FPU status word
		sahf                            ; Set flags based on test
		jb      L1                      ; Re-direct if 1e-5 < x
 		fld     st(0)                   ; Duplicate x
 		fld     st(0)                   ; Duplicate x
		fld     qword ptr [half]        ; Load 0.5
		fmulp   st(1), st(0)            ; Compute 0.5 * x * x + x
		fmulp   st(1), st(0)
		faddp   st(1), st(0)
		ret
	L1:
		sub     esp, 8                  ; Allocate stack space for x
		fstp    qword ptr [esp]         ; Copy x onto stack
		call    exp                     ; Call exp
		add     esp, 8                  ; Remove x from stack
		fld1                            ; Load real number 1
		fsubp   st(1), st(0)            ; Subtract
		ret
	}
}
