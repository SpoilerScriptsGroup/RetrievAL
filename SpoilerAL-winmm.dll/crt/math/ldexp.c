#ifndef _M_IX86
#include <math.h>
#include <float.h>
#include <errno.h>
#include <stdint.h>

#define DBL_BIT       64
#define DBL_MANT_BIT  (DBL_MANT_DIG - DBL_HAS_SUBNORM)
#define DBL_MANT_MASK ((UINT64_C(1) << DBL_MANT_BIT) - 1)
#define DBL_SIGN_BIT  1
#define DBL_SIGN_MASK ((UINT64_C(1) << (DBL_BIT - DBL_SIGN_BIT))
#define DBL_EXP_BIT   (DBL_BIT - DBL_SIGN_BIT - DBL_MANT_BIT)
#define DBL_EXP_MASK  (((UINT64_C(1) << DBL_EXP_BIT) - 1) << DBL_MANT_BIT)
#define DBL_INF_BIN   DBL_EXP_MASK

#define UI64(x) (*(uint64_t *)&(x))

double __cdecl ldexp(double x, int exp)
{
	uintptr_t sign;

	sign = (UI64(x) >> (DBL_BIT - sizeof(uintptr_t) * 8)) & INTPTR_MIN;
	UI64(x) &= ~DBL_SIGN_MASK;
	if (UI64(x) < DBL_INF_BIN && UI64(x) && exp)
	{
		exp += UI64(x) >> DBL_MANT_BIT;
		if (exp < (int)(DBL_EXP_MASK >> DBL_MANT_BIT))
		{
			if (exp >= 0)
			{
				UI64(x) &= DBL_MANT_MASK;
				UI64(x) |= (uint64_t)(unsigned int)exp << DBL_MANT_BIT;
			}
			else
			{
				UI64(x) = 0;
			}
		}
		else
		{
			UI64(x) = DBL_INF_BIN;
			errno = ERANGE;
		}
	}
	UI64(x) |= (uint64_t)sign << (DBL_BIT - sizeof(uintptr_t) * 8);
	return x;
}
#else
#include <errno.h>

__declspec(naked) double __cdecl ldexp(double x, int exp)
{
#ifdef _DEBUG
	errno_t * __cdecl _errno();
	#define set_errno(x) \
		__asm   sub     esp, 8                  /* Allocate temporary space */ \
		__asm   fstp    qword ptr [esp]         /* Save x */ \
		__asm   call    _errno                  /* Get C errno variable pointer */ \
		__asm   add     esp, 8                  /* Deallocate temporary space */ \
		__asm   mov     dword ptr [eax], x      /* Set error number */ \
		__asm   fld     qword ptr [esp - 8]     /* Load x */
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
#endif
