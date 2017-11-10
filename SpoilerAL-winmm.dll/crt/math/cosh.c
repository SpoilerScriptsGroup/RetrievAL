#pragma function(cosh)

extern const double _half;
extern const double _one;

__declspec(naked) double __cdecl cosh(double x)
{
	__asm
	{
		emms
		fld     qword ptr [esp + 4]     ; Load real from stack
		fldl2e                          ; Load log base 2(e)
		fmul                            ; Multiply x * log base 2(e)
		fld     st(0)                   ; Duplicate result
		frndint                         ; Round to integer
		fsub    st(1), st(0)            ; Subtract
		fxch                            ; Exchange st, st(1)
		f2xm1                           ; Compute 2 to the (x - 1)
		fadd    qword ptr [_one]        ; 2 to the x
		fscale                          ; Compute exp(-x)
		fstp    st(1)                   ; Set new stack top and pop
		fld     qword ptr [esp + 4]     ; Load real from stack
		fchs                            ; Set x = -x
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
		fadd                            ; Compute exp(x) + exp(-x)
		fmul    qword ptr [_half]       ; Compute the hyperbolic cosine
		ret
	}
}
