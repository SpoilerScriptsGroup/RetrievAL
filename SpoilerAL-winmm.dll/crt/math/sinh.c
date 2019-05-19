#pragma function(sinh)

#include <errno.h>

__declspec(naked) double __cdecl sinh(double x)
{
	double __cdecl _CIsinh(/*st0 x*/);

	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		jmp     _CIsinh
	}
}

__declspec(naked) double __cdecl _CIsinh(/*st0 x*/)
{
	extern const double _half;
	extern const double _one;

#ifdef _DEBUG
	errno_t * __cdecl _errno();
	#define set_errno(x) \
		__asm   sub     esp, 8                  /* Allocate temporary space */ \
		__asm   fstp    qword ptr [esp]         /* Save x */ \
		__asm   call    _errno                  /* Get C errno variable pointer */ \
		__asm   mov     dword ptr [eax], x      /* Set error number */ \
		__asm   fld     qword ptr [esp]         /* Load x */ \
		__asm   add     esp, 8                  /* Deallocate temporary space */
#else
	extern errno_t _terrno;
	#define set_errno(x) \
		__asm   mov     dword ptr [_terrno], x  /* Set error number */
#endif

	__asm
	{
		fxam                                ; Examine st
		fstsw   ax                          ; Get the FPU status word
		test    ah, 00000001B               ; NaN or infinity ?
		jnz     L2                          ; Re-direct if x is NaN or infinity
		fld     st(0)                       ; Duplicate argument
		fldl2e                              ; Load log base 2(e)
		fmul                                ; Multiply x * log base 2(e)
		fld     st(0)                       ; Duplicate result
		frndint                             ; Round to integer
		fsub    st(1), st(0)                ; Subtract
		fxch                                ; Exchange st, st(1)
		f2xm1                               ; Compute 2 to the (x - 1)
		fadd    qword ptr [_one]            ; 2 to the x
		fscale                              ; Compute exp(-x)
		fstp    st(1)                       ; Set new stack top and pop
		fxch                                ; Exchange st, st(1)
		fchs                                ; Set x = -x
		fldl2e                              ; Load log base 2(e)
		fmul                                ; Multiply x * log base 2(e)
		fld     st(0)                       ; Duplicate result
		frndint                             ; Round to integer
		fsub    st(1), st(0)                ; Subtract
		fxch                                ; Exchange st, st(1)
		f2xm1                               ; Compute 2 to the (x - 1)
		fadd    qword ptr [_one]            ; 2 to the x
		fscale                              ; Scale by power of 2
		fstp    st(1)                       ; Set new stack top and pop
		fsub                                ; Compute exp(x) - exp(-x)
		fmul    qword ptr [_half]           ; Compute the hyperbolic sine
		fst     qword ptr [esp - 8]         ; Save x, cast to qword
		fld     qword ptr [esp - 8]         ; Load x
		fxam                                ; Examine st
		fstsw   ax                          ; Get the FPU status word
		and     ah, 01000101B               ; Isolate C0, C2 and C3
		cmp     ah, 00000101B               ; Infinity ?
		je      L1                          ; Re-direct if x is infinity
		fstp    st(0)                       ; Set new top of stack
		ret
	L1:
		fstp    st(1)                       ; Set new stack top and pop
	L2:
		set_errno(ERANGE)                   ; Set range error (ERANGE)
		ret
	}

	#undef set_errno
}
