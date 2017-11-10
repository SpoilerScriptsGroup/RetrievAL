#pragma function(tanh)

extern const double _one;
extern const double _two;

__declspec(naked) double __cdecl tanh(double x)
{
	__asm
	{
		emms
		fld     qword ptr [esp + 4]     ; Load real from stack
		fld     st(0)                   ; Duplicate stack top
		fadd                            ; Compute 2 * x
		fldl2e                          ; Load log base 2(e)
		fmul                            ; Multiply x * log base 2(e)
		fld     st(0)                   ; Duplicate result
		frndint                         ; Round to integer
		fsub    st(1), st(0)            ; Subtract
		fxch                            ; Exchange st, st(1)
		f2xm1                           ; Compute 2 to the (x - 1)
		fadd    qword ptr [_one]        ; 2 to the x
		fscale                          ; Scale by power of 2
		fstp    st(1)                   ; Set new stack top and pop
		fadd    qword ptr [_one]        ; Compute exp(2*x)+1
		fld     qword ptr [_two]        ; Load the constant 2
		fdivr                           ; Compute 2/(exp(2*x)+1)
		fld1                            ; Load constant 1
		fsubr                           ; Compute the hyperbolic tangent
		ret
	}
}
