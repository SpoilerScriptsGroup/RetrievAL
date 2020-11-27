#include <errno.h>

__declspec(naked) double __cdecl acosh(double x)
{
	double __cdecl _CIlog(/*st0 x*/);

	extern const double fpconst_nan;
	#define _nan fpconst_nan

#ifdef _DEBUG
	errno_t * __cdecl _errno();
	#define set_errno(x) \
		__asm   call    _errno                  /* Get C errno variable pointer */ \
		__asm   mov     dword ptr [eax], x      /* Set error number */
#else
	extern errno_t _terrno;
	#define set_errno(x) \
		__asm   mov     dword ptr [_terrno], x  /* Set error number */
#endif

	__asm
	{
		fld     qword ptr [esp + 4]             ; Load real from stack
		fld1                                    ; Load real number 1
		fcom                                    ; Compare 1 with x
		fstsw   ax                              ; Get the FPU status word
		test    ah, 45H                         ; 1 > x ?
		jz      L1                              ; Re-direct if 1 > x
		fld     st(1)                           ; Duplicate st1
		fmul    st(0), st(0)                    ; Compute st * st
		fsubr                                   ; Decrement
		fsqrt                                   ; Take the square root
		fadd                                    ; 2 to the st
		jmp     _CIlog                          ; Compute the natural log(st)

		align   16
	L1:
		fstp    st(0)                           ; Set new top of stack
		fstp    st(0)                           ; Set new top of stack
		set_errno(EDOM)                         ; Set domain error (EDOM)
		fld     qword ptr [_nan]                ; Load NaN
		ret
	}

	#undef _nan
	#undef set_errno
}
