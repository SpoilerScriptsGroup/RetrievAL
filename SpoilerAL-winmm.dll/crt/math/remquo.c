#include <errno.h>

__declspec(naked) double __cdecl remquo(double x, double y, int *quo)
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
		fld     qword ptr [esp + 12]            ; Load real from stack
		ftst                                    ; Compare y with zero
		fstsw   ax                              ; Get the FPU status word
		mov     cx, ax                          ;
		fld     qword ptr [esp + 4]             ; Load real from stack
		fxam                                    ; Examine st
		fstsw   ax                              ; Get the FPU status word
		and     ch, 40H                         ; Zero ?
		and     ah, 05H                         ; NaN or infinity ?
		or      ch, ah                          ;
		jnz     L1                              ; Re-direct if x is NaN, infinity, or y is zero
		fprem1                                  ; Get the partial remainder
		fstsw   ax                              ; Get coprocessor status
		test    ah, 04H                         ; Complete remainder ?
		jnz     L1                              ; No, go get next remainder
		fstp    st(1)                           ; Set new stack top and pop
		shr     eax, 6                          ; Extract the three low-order bits of the quotient from C0, C3, C1.
		mov     ecx, eax
		and     eax, 0108H
		ror     eax, 7
		or      ecx, eax
		mov     edx, dword ptr [esp + 8]
		rol     eax, 4
		or      eax, ecx
		mov     ecx, dword ptr [esp + 16]       ; Negate the quotient bits if x*y<0.  Avoid using an unpredictable branch.
		and     eax, 7
		xor     ecx, edx
		sar     ecx, 32
		xor     eax, ecx
		and     ecx, 1
		add     eax, ecx
		mov     ecx, dword ptr [esp + 20]       ; Store the quotient and return.
		mov     dword ptr [ecx], eax
		ret

		align   16
	L1:
		cmp     ah, 01H                         ; NaN ?
		je      L2                              ; Re-direct if x is NaN
		fstp    st(0)                           ; Set new top of stack
		set_errno(EDOM)                         ; Set domain error (EDOM)
		fld     qword ptr [_nan]                ; Load NaN
	L2:
		fstp    st(1)                           ; Set new stack top and pop
		ret
	}

	#undef _nan
	#undef set_errno
}
