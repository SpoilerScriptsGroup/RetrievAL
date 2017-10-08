#pragma function(tanh)

__declspec(naked) double __cdecl tanh(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]     ; Load real from stack
		fld     st(0)                   ; Duplicate stack top
		fadd                            ; Compute 2 * x
		fldl2e                          ; Load log base 2(e)
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
		fld1                            ; Load constant 1
		fadd                            ; Compute exp(2*x)+1
		fld1                            ; Load the constant 1
		fld1                            ; Load the constant 1
		fadd                            ; Set divisor to 2
		fdivr                           ; Compute 2/(exp(2*x)+1)
		fld1                            ; Load constant 1
		fsubr                           ; Compute the hyperbolic tangent
		ret
	}
}
