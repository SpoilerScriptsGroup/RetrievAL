#include <errno.h>

#ifdef _DEBUG
errno_t * __cdecl _errno();
#define __errno(x) \
	__asm   call    _errno                  /* Get C errno variable pointer */ \
	__asm   mov     dword ptr [eax], x      /* Set error number */
#else
extern errno_t _terrno;
#define __errno(x) \
	__asm   mov     dword ptr [_terrno], x  /* Set error number */
#endif

extern const double _one;

__declspec(naked) double __cdecl exp2(double x)
{
	__asm
	{
		emms
		fld     qword ptr [esp + 4]     ; Load real from stack
		fxam                            ; Examine st
		fstsw   ax                      ; Get the FPU status word
		and     ah, 01000101B           ; Isolate C0, C2 and C3
		cmp     ah, 01000000B           ; Zero ?
		je      L1                      ; Re-direct if x == 0
		test    ah, 00000001B           ; NaN or infinity ?
		jnz     L2                      ; Re-direct if x is NaN or infinity
		fld     st(0)                   ; Duplicate x
		frndint                         ; Round to integer
		fsub    st(1), st(0)            ; Subtract
		fxch                            ; Exchange st, st(1)
		f2xm1                           ; Compute 2 to the (x - 1)
		fadd    qword ptr [_one]        ; 2 to the x
		fscale                          ; Scale by power of 2
		fstp    st(1)                   ; Set new stack top and pop
		fxam                            ; Examine st
		fstp    qword ptr [esp + 4]     ; Save x, 'fxam' is require the load memory
		fld     qword ptr [esp + 4]     ; Load x
		fstsw   ax                      ; Get the FPU status word
		and     ah, 01000101B           ; Isolate C0, C2 and C3
		test    ah, 00000001B           ; NaN or infinity ?
		jnz     L3                      ; Re-direct if x is NaN or infinity (overflow)
		cmp     ah, 01000000B           ; Zero ?
		je      L3                      ; Re-direct if x is zero (underflow)
	L1:
		ret
	L2:
		__errno(EDOM)                   ; Set domain error (EDOM)
		ret
	L3:
		__errno(ERANGE)                 ; Set range error (ERANGE)
		ret
	}
}
