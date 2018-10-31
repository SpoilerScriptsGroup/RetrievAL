#include <stdint.h>
#include <math.h>
#include <errno.h>

__declspec(naked) double __cdecl log1p(double x)
{
	extern const double _minus_inf;
	extern const double _nan;
	extern const double _one;

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
		#define x (esp + 4)

		; x > -1.0 ... 0xBFF000000000

		mov     eax, dword ptr [x]
		mov     edx, dword ptr [x + 4]
		mov     ecx, edx
		and     edx, 7FF00000h
		cmp     edx, 7FF00000h
		je      L1                      ; Re-direct if x is NaN or infinity
		mov     edx, ecx
		and     ecx, 7FFFFFFFh
		or      ecx, eax
		jz      L2                      ; Re-direct if x == 0
		sub     edx, 0BFF00000h
		jae     L3                      ; Re-direct if x <= -1.0
		fld     qword ptr [x]           ; Compute the natural log(x + 1.0)
		fadd    qword ptr [_one]
		fldln2
		fld     st(1)
		fyl2x
		fld     st(1)
		fsub    qword ptr [_one]
		fsub    qword ptr [x]
		fdivrp  st(2), st(0)
		fsubr
		ret
	L1:
		xor     ecx, 0FFF00000h
		or      eax, ecx
		jz      L4                      ; Re-direct if x is -infinity
	L2:
		fld     qword ptr [x]
		ret
	L3:
		or      eax, edx
		jz      L5                      ; Re-direct if x == -1.0
	L4:
		set_errno(EDOM)                 ; Set domain error (EDOM)
		fld     qword ptr [_nan]
		ret
	L5:
		set_errno(ERANGE)               ; Set range error (ERANGE)
		fld     qword ptr [_minus_inf]
		ret

		#undef x
	}

	#undef set_errno
}
