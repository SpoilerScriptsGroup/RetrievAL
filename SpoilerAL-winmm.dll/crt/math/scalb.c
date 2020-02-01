#include <errno.h>

__declspec(naked) double __cdecl _scalb(double x, long exp)
{
#ifdef _DEBUG
	errno_t * __cdecl _errno();
	#define set_errno(x) \
		__asm   fstp    qword ptr [esp + 4]     /* Save x */ \
		__asm   call    _errno                  /* Get C errno variable pointer */ \
		__asm   fld     qword ptr [esp + 4]     /* Load x */ \
		__asm   mov     dword ptr [eax], x      /* Set error number */
#else
	extern errno_t _terrno;
	#define set_errno(x) \
		__asm   mov     dword ptr [_terrno], x  /* Set error number */
#endif

	__asm
	{
		fild    dword ptr [esp + 12]            ; Load exp as integer
		fld     qword ptr [esp + 4]             ; Load real from stack
		fxam                                    ; Examine st
		fstsw   ax                              ; Get the FPU status word
		test    ah, 01H                         ; NaN or infinity ?
		jnz     L1                              ; Re-direct if x is NaN or infinity
		fscale                                  ; Scale by power of 2
		fstp    qword ptr [esp - 8]             ; Cast to qword
		fld     qword ptr [esp - 8]             ;
		fxam                                    ; Examine st
		fstsw   ax                              ; Get the FPU status word
		test    ah, 01H                         ; Not NaN and infinity ?
		jz      L1                              ; Re-direct if result is not NaN, not infinity
		set_errno(ERANGE)                       ; Set range error (ERANGE)
	L1:
		fstp    st(1)                           ; Set new stack top and pop
		ret
	}

	#undef set_errno
}
