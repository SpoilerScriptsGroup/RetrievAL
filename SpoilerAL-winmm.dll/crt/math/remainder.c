#include <errno.h>

__declspec(naked) double __cdecl remainder(double x, double y)
{
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
		fld     qword ptr [esp + 12]        ; Load real from stack
		ftst                                ; Compare y with zero
		fnstsw  ax                          ; Get the FPU status word
		mov     cx, ax                      ;
		fld     qword ptr [esp + 4]         ; Load real from stack
		fxam                                ; Examine st
		fnstsw  ax                          ; Get the FPU status word
		and     ch, 40H                     ; Zero ?
		and     ah, 05H                     ; NaN or infinity ?
		or      ch, ah                      ;
		jz      L1                          ; Re-direct if x is not NaN, not infinity, and y is not zero
		cmp     ah, 01H                     ; NaN ?
		je      L2                          ; Re-direct if x is NaN
		fstp    st(0)                       ; Set new top of stack
		fstp    st(0)                       ; Set new top of stack
		set_errno(EDOM)                     ; Set domain error (EDOM)
		fld     qword ptr [_nan]            ; Load NaN
		ret

		align   16
	L1:
		fprem1                              ; Get the partial remainder
		fstsw   ax                          ; Get coprocessor status
		test    ah, 04H                     ; Complete remainder ?
		jnz     L1                          ; No, go get next remainder
	L2:
		fstp    st(1)                       ; Set new stack top and pop
		ret
	}

	#undef _nan
	#undef set_errno
}
