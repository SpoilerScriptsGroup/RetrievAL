#include <errno.h>

__declspec(naked) double __cdecl atanh(double x)
{
	double __cdecl _CIlog(/*st0 x*/);

	extern const double fpconst_half;
	#define _half fpconst_half

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
		fld     qword ptr [esp + 4]         ; Load real from stack
		fld1                                ; Load real number 1
		fld     st(1)                       ; Duplicate x
		fadd                                ; Compute 1 + x
		fld1                                ; Load real number 1
		fsubrp  st(2), st(0)                ; Compute 1 - x
		fdivr                               ; Compute (1 + x) / (1 - x)
		call    _CIlog                      ; Compute the natural log(st)
		fxam                                ; Examine st
		fstsw   ax                          ; Get the FPU status word
		and     ah, 45H                     ; Isolate C0, C2 and C3
		cmp     ah, 05H                     ; Infinity ?
		je      L1                          ; Re-direct if st is infinity
		fmul    qword ptr [_half]           ; Compute st * 0.5
		ret

		align   16
	L1:
		set_errno(ERANGE)                   ; Set range error (ERANGE)
		ret
	}

	#undef _half
	#undef set_errno
}
