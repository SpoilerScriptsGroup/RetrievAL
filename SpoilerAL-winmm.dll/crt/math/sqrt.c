#include <errno.h>

#pragma function(sqrt)

__declspec(naked) double __cdecl sqrt(double x)
{
	double __cdecl _CIsqrt(/*st0 x*/);

	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		jmp     _CIsqrt
	}
}

__declspec(naked) double __cdecl _CIsqrt(/*st0 x*/)
{
	extern const double fpconst_nan_ind;
	#define _nan_ind fpconst_nan_ind

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
		ftst                                ; Compare x with zero
		fnstsw  ax                          ; Get the FPU status word
		test    ah, 01H                     ; x < 0 ?
		jnz     L1                          ; Re-direct if x < 0
		fsqrt                               ; Take the square root
		ret

		align   16
	L1:
		fstp    st(0)                       ; Set new top of stack
		set_errno(EDOM)                     ; Set domain error (EDOM)
		fld     qword ptr [_nan_ind]        ; Load NaN(indeterminate)
		ret
	}

	#undef set_errno
}
