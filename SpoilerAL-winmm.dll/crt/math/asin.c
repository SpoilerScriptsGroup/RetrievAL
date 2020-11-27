#include <errno.h>

#pragma function(asin)

__declspec(naked) double __cdecl asin(double x)
{
	double __cdecl _CIasin(/*st0 x*/);

	__asm
	{
		fld     qword ptr [esp + 4]             ; Load real from stack
		jmp     _CIasin
	}
}

__declspec(naked) double __cdecl _CIasin(/*st0 x*/)
{
	extern const double fpconst_one;
	extern const double fpconst_inf;
	#define _one fpconst_one
	#define _inf fpconst_inf

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
		fld     st(0)                           ; Load x
		fabs                                    ; Take the absolute value
		fcomp   qword ptr [_one]                ; Compare |x| with 1
		fstsw   ax                              ; Get the FPU status word
		test    ah, 45H                         ; |x| > 1 ?
		jz      L1                              ; Re-direct if |x| > 1
		fld     st(0)                           ; Load x
		fmul    st(0), st(0)                    ; Multiply (x squared)
		fld1                                    ; Load 1
		fsubr                                   ; 1 - (x squared)
		fsqrt                                   ; Square root of (1 - x squared)
		fpatan                                  ; This gives the arc sine !
		ret

		align   16
	L1:
		fstp    st(0)                           ; Set new top of stack
		set_errno(EDOM)                         ; Set domain error (EDOM)
		fld     qword ptr [_inf]                ; Load infinity
		ret
	}

	#undef _one
	#undef _inf
	#undef set_errno
}
