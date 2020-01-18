#include <errno.h>

__declspec(naked) double __cdecl _scalb(double x, long exp)
{
#ifdef _DEBUG
	errno_t * __cdecl _errno();
	#define set_errno(x) \
		__asm   fstp    qword ptr [esp]         /* Save x */ \
		__asm   call    _errno                  /* Get C errno variable pointer */ \
		__asm   mov     dword ptr [eax], x      /* Set error number */ \
		__asm   fld     qword ptr [esp]         /* Load x */
#else
	extern errno_t _terrno;
	#define set_errno(x) \
		__asm   mov     dword ptr [_terrno], x  /* Set error number */
#endif

	__asm
	{
		sub     esp, 8                      ; Allocate temporary space
		fild    dword ptr [esp + 20]        ; Load exp as integer
		fld     qword ptr [esp + 12]        ; Load real from stack
		fxam                                ; Examine st
		fstsw   ax                          ; Get the FPU status word
		test    ah, 01H                     ; NaN or infinity ?
		jnz     L1                          ; Re-direct if x is NaN or infinity
		fscale                              ; Scale by power of 2
		fstp    qword ptr [esp]             ; Cast to qword
		fld     qword ptr [esp]             ;
		fxam                                ; Examine st
		fstsw   ax                          ; Get the FPU status word
		test    ah, 01H                     ; Not NaN and infinity ?
		jz      L1                          ; Re-direct if result is not NaN and infinity
		set_errno(ERANGE)                   ; Set range error (ERANGE)
	L1:
		fstp    st(1)                       ; Set new stack top and pop
		add     esp, 8                      ; Deallocate temporary space
		ret
	}

	#undef set_errno
}
