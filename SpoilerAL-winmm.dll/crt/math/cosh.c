#pragma function(cosh)

__declspec(naked) double __cdecl cosh(double x)
{
	__asm
	{
		sub     esp, 8                  ; Allocate temporary space
		fld     qword ptr [esp + 12]    ; Load real from stack
		fchs                            ; Set x = -x
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
		fstp    qword ptr [esp]         ; Save exp(-x)
		fld     qword ptr [esp + 12]    ; Load real from stack
		fldl2e                          ; Load log base 2(e)
		fmulp   st(1), st(0)            ; Multiply x * log base 2(e)
		fst     st(1)                   ; Push result
		frndint                         ; Round to integer
		fsub    st(1), st(0)            ; Subtract
		fxch                            ; Exchange st, st(1)
		f2xm1                           ; Compute 2 to the (x - 1)
		fld1                            ; Load real number 1
		fadd                            ; 2 to the x
		fscale                          ; Compute exp(-x)
		fstp    st(1)                   ; Set new stack top and pop
		fld     qword ptr [esp]         ; Get exp(x)
		fadd                            ; Compute exp(x) + exp(-x)
		fld1                            ; Load the constant 1
		fld1                            ; Load the constant 1
		fadd                            ; Set divisor to 2
		fdiv                            ; Compute the hyperbolic cosine
		add     esp, 8                  ; Deallocate temporary space
		ret
	}
}
