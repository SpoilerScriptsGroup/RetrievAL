#pragma function(exp)

#define SMALL 0

#ifndef _M_IX86
#include <math.h>
#include <errno.h>

double __cdecl exp(double x)
{
	// log2(e)          1.442695040888963407359924681001892137426645954152985934135
	#define L2E_A       1.442687988281250000000000000000000000000000000000000000000	// 0x3FF7154000000000
	#define L2E_B       0.000007052607713407359924681001892137426645954152985934135	// 0x3EDD94AE0BF85DDF

	#define M_LN_MAX_D  709.7827128933839968432456923731728057093121375849013802195	// log(2) * DBL_MAX_EXP
	#define M_LN_MIN_D -708.3964185322641062244112281302564525731611373161808697113	// log(2) * (DBL_MIN_EXP - 1)

	if (!_isnan(x))
	{
		if (x < M_LN_MAX_D)
		{
			if (x > M_LN_MIN_D)
			{
				double f1, f2, i1, i2, n;

				f1 = x * L2E_A;
				f2 = x * L2E_B;
				i1 = round(f1);
				i2 = round(f2);
				n = i1 + i2;
				f1 -= i1;
				f2 -= i2;
				f1 += f2;
				i1 = round(f1);
				n += i1;
				f1 -= i1;
				x = exp2(f1);
				x = ldexp(x, (int)n);
			}
			else
			{
				errno = ERANGE;
				x = 0;
			}
		}
		else
		{
			errno = ERANGE;
			x = HUGE_VAL;
		}
	}
	return x;

	#undef L2E_A
	#undef L2E_B
	#undef M_LN_MAX_D
	#undef M_LN_MIN_D
}
#elif !SMALL
#include <errno.h>

errno_t * __cdecl _errno();

__declspec(naked) double __cdecl exp(double x)
{
	// log2(e) ................ 1.442695040888963407359924681001892137426645954152985934135
	static const double l2e_a = 1.442687988281250000000000000000000000000000000000000000000;	// 0x3FF7154000000000
	static const double l2e_b = 0.000007052607713407359924681001892137426645954152985934135;	// 0x3EDD94AE0BF85DDF

	__asm
	{
		emms
		fld     qword ptr [esp + 4]     ; Load x
		fxam                            ; Examine st
		fstsw   ax                      ; Get the FPU status word
		and     ah, 01000101B           ; Isolate  C0, C2 and C3
		cmp     ah, 01000000B           ; Zero ?
		je      L1                      ; Re-direct if x == 0
		test    ah, 00000001B           ; NaN or infinity ?
		jnz     L2                      ; Re-direct if x is NaN or infinity
		fmul    qword ptr [l2e_a]       ; Multiply:                 f1 = x * l2e_a
		fld     qword ptr [esp + 4]     ; Load real from stack
		fmul    qword ptr [l2e_b]       ; Multiply:                 f2 = x * l2e_b
		fld     st(1)                   ; Duplicate f1
		frndint                         ; Round to integer:         i1 = round(f1)
		fld     st(1)                   ; Duplicate f2
		frndint                         ; Round to integer:         i2 = round(f2)
		fld     st(1)                   ; Duplicate i1
		fadd    st(0), st(1)            ; Add:                      n = i1 + i2
		fxch    st(4)                   ; Exchange st, st(4)
		fsubrp  st(2), st(0)            ; Subtract:                 f1 -= i1
		fsubp   st(2), st(0)            ; Subtract:                 f2 -= i2
		fadd    st(0), st(1)            ; Add:                      f1 += f2
		fst     st(1)                   ; Push f1
		frndint                         ; Round to integer:         i1 = round(f1)
		fadd    st(2), st(0)            ; Add:                      n += i1
		fsubp   st(1), st(0)            ; Subtract:                 f1 -= i1
		f2xm1                           ; Compute 2 to the (x - 1): x *= exp2(f1)
		fld1                            ; Load real number 1
		fadd                            ; 2 to the x
		fscale                          ; Scale by power of 2:      x = ldexp(x, n)
		fstp    st(1)                   ; Set new stack top and pop
		fstp    qword ptr [esp + 4]     ; Save x, 'fxam' is require the load memory
		fld     qword ptr [esp + 4]     ; Load x
		fxam                            ; Examine st
		fstsw   ax                      ; Get the FPU status word
		and     ah, 01000101B           ; Isolate  C0, C2 and C3
		test    ah, 00000001B           ; NaN or infinity ?
		jnz     L3                      ; Re-direct if x is NaN or infinity (overflow)
		cmp     ah, 01000000B           ; Zero ?
		je      L3                      ; Re-direct if x is zero (underflow)
	L1:
		ret
	L2:
		call    _errno                  ; Get C errno variable pointer
		mov     dword ptr [eax], EDOM   ; Set domain error (EDOM)
		ret
	L3:
		call    _errno                  ; Get C errno variable pointer
		mov     dword ptr [eax], ERANGE ; Set range error (ERANGE)
		ret
	}
}
#else
__declspec(naked) double __cdecl exp(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]     ; Load real from stack
		fldl2e                          ; Load log base 2(e)
		fmulp   st(1), st(0)            ; Multiply x * log base 2(e)
		fst     st(1)                   ; Push result
		frndint                         ; Round to integer
		fsub    st(1), st(0)            ; Subtract
		fxch                            ; Exchange st, st(1)
		f2xm1                           ; Compute 2 to the (x - 1)
		fld1                            ; Load real number 1
		fadd                            ; 2 to the x
		fscale                          ; Scale by power of 2
		fstp    st(1)                   ; Set new stack top and pop
		ret
	}
}
#endif
