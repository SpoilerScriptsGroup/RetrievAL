#include <errno.h>

__declspec(naked) double __cdecl atanh(double x)
{
	double __cdecl _CIlog(/*st0 x*/);

	extern const double fpconst_half;
	#define _half fpconst_half

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
		mov     eax, dword ptr [esp + 8]
		mov     ecx, 80000000H
		and     ecx, eax
		and     eax, 7FFFFFFFH
		cmp     eax, 3FF00000H              ; |x| >= 1 ?
		jae     L1                          ; Re-direct if |x| >= 1
		fld     qword ptr [esp + 4]         ; Load real from stack
		fld1                                ; Load real number 1
		fld     st(1)                       ; Duplicate x
		fadd                                ; Compute 1 + x
		fld1                                ; Load real number 1
		fsubrp  st(2), st(0)                ; Compute 1 - x
		fdivr                               ; Compute (1 + x) / (1 - x)
		call    _CIlog                      ; Compute the natural log(st)
		fmul    qword ptr [_half]           ; Compute st * 0.5
		ret

		align   16
	L1:
		mov     edx, dword ptr [esp + 4]
		je      L2                          ; Re-direct if |x| == 1
		cmp     edx, 1                      ; x is NaN ?
		sbb     eax, 7FF00000H              ;
		jae     L4                          ; Re-direct if x is NaN
		set_errno(EDOM)                     ; Set domain error (EDOM)
		mov     ecx, 7FF80000H
		jmp     L3

		align   16
	L2:
		set_errno(ERANGE)                   ; Set range error (ERANGE)
#ifdef _DEBUG
		mov     ecx, dword ptr [esp + 8]
#endif
		or      ecx, 7FF00000H
	L3:
		mov     dword ptr [esp + 4], 0
		mov     dword ptr [esp + 8], ecx
	L4:
		fld     qword ptr [esp + 4]
		ret

	}

	#undef _half
	#undef set_errno
}
