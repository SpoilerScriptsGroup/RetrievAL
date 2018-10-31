#pragma function(tanh)

__declspec(naked) double __cdecl tanh(double x)
{
	double __cdecl _CItanh(/*st0 x*/);

	__asm
	{
		fld     qword ptr [esp + 4]     ; Load real from stack
		jmp     _CItanh
	}
}

__declspec(naked) double __cdecl _CItanh(/*st0 x*/)
{
	extern const double _one;
	extern const double _two;

	__asm
	{
		fadd    st(0), st(0)            ; Compute 2 * x
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
