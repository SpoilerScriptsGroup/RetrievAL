#ifndef _M_IX86
#include <math.h>
#include <float.h>
#include <errno.h>

double __cdecl ldexp(double x, int exp)
{
	if (!_isnan(x))
	{
		if (x)
		{
			unsigned char sign;

			sign = x < 0;
			if (sign)
			{
				x = -x;
			}
			if (x <= DBL_MAX)
			{
				int currexp;

				x = frexp(x, &currexp);
				exp += currexp;
				if (exp > 0)
				{
					if (exp <= DBL_MAX_EXP)
					{
						while (exp > 30)
						{
							x *= (double)(1L << 30);
							exp -= 30;
						}
						x *= (double)(1L << exp);
					}
					else
					{
						errno = ERANGE;
						x = HUGE_VAL;
					}
				}
				else
				{
					if (exp >= DBL_MIN_EXP - DBL_MANT_DIG)
					{
						while (exp < -30)
						{
							x /= (double)(1L << 30);
							exp += 30;
						}
						x /= (double)(1L << -exp);
					}
					else
					{
						x = 0;
						sign = 0;
					}
				}
			}
			else
			{
				errno = ERANGE;
			}
			if (sign)
			{
				x = -x;
			}
		}
		else
		{
			x = 0;
		}
	}
	else
	{
		errno = EDOM;
	}
	return x;
}
#else
#include <errno.h>

__declspec(naked) double __cdecl ldexp(double x, int exp)
{
#ifdef _DEBUG
	errno_t * __cdecl _errno();
	#define set_errno(x) \
		__asm   fstp    qword ptr [esp + 4]     /* Save x */ \
		__asm   call    _errno                  /* Get C errno variable pointer */ \
		__asm   mov     dword ptr [eax], x      /* Set error number */ \
		__asm   fld     qword ptr [esp + 4]     /* Load x */
#else
	extern errno_t _terrno;
	#define set_errno(x) \
		__asm   mov     dword ptr [_terrno], x  /* Set error number */
#endif

	__asm
	{
		fild    dword ptr [esp + 12]        ; Load exp as integer
		fld     qword ptr [esp + 4]         ; Load real from stack
		fxam                                ; Examine st
		fstsw   ax                          ; Get the FPU status word
		test    ah, 00000001B               ; NaN or infinity ?
		jnz     L1                          ; Re-direct if x is NaN or infinity
		fscale                              ; Compute 2 to the x
		fstp    st(1)                       ; Set new stack top and pop
		fst     qword ptr [esp + 4]         ; Save x, cast to qword
		fld     qword ptr [esp + 4]         ; Load x
		fxam                                ; Examine st
		fstsw   ax                          ; Get the FPU status word
		and     ah, 01000101B               ; Isolate C0, C2 and C3
		cmp     ah, 00000101B               ; Infinity ?
		je      L2                          ; Re-direct if x is infinity
		fstp    st(0)                       ; Set new top of stack
		ret

		align   16
	L1:
		and     ah, 01000101B               ; Isolate C0, C2 and C3
		cmp     ah, 00000101B               ; Infinity ?
		je      L2                          ; Re-direct if x is infinity
		fstp    st(1)                       ; Set new stack top and pop
		set_errno(EDOM)                     ; Set domain error (EDOM)
		ret

		align   16
	L2:
		fstp    st(1)                       ; Set new stack top and pop
		set_errno(ERANGE)                   ; Set range error (ERANGE)
		ret
	}

	#undef set_errno
}
#endif
