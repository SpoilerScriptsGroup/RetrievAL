#pragma function(exp)

#define SMALL 0

#ifndef _M_IX86
#include <math.h>
#include <errno.h>

double __cdecl exp(double x)
{
	#ifndef M_LN2
	#define M_LN2      0.693147180559945309417232121458176568075500134360255254120	// log(2)
	#endif
	#ifndef M_LOG2E
	#define M_LOG2E    1.442695040888963407359924681001892137426645954152985934135	// log2(e)
	#endif
	#ifndef M_LN_MAX_D
	#define M_LN_MAX_D 709.7827128933839968432456923731728057093121375849013802195	// log(2) * DBL_MAX_EXP
	#endif
	#ifndef M_LN_MIN_D
	#define M_LN_MIN_D -708.3964185322641062244112281302564525731611373161808697113	// log(2) * (DBL_MIN_EXP - 1)
	#endif

	#define P0   0.25000000000000000000e+0
	#define P1   0.75753180159422776666e-2
	#define P2   0.31555192765684646356e-4
	#define P(x) ((P2 * (x) + P1) * (x) + P0)

	#define Q0   0.50000000000000000000e+0
	#define Q1   0.56817302698551221787e-1
	#define Q2   0.63121894374398503557e-3
	#define Q3   0.75104028399870046114e-6
	#define Q(x) (((Q3 * (x) + Q2) * (x) + Q1) * (x) + Q0)

	if (!_isnan(x))
	{
		if (x >= M_LN_MIN_D)
		{
			if (x <= M_LN_MAX_D)
			{
				double        xn, g, x1, x2;
				int           n;
				unsigned char negative;

				negative = x < 0;
				if (negative)
					x = -x;
				n = (int)(x * M_LOG2E + 0.5);
				xn = n;
				x2 = modf(x, &x1);
				g = x1 + x2 - xn * M_LN2;
				if (negative)
				{
					g = -g;
					n = -n;
				}
				xn = g * g;
				x = g * P(xn);
				n++;
				x = ldexp(0.5 + x / (Q(xn) - x), n);
			}
			else
			{
				errno = ERANGE;
				x = HUGE_VAL;
			}
		}
		else
		{
			errno = ERANGE;
			x = 0;
		}
	}
	else
	{
		errno = EDOM;
	}
	return x;

	#undef P0
	#undef P1
	#undef P2
	#undef P
	#undef Q0
	#undef Q1
	#undef Q2
	#undef Q3
	#undef Q
}
#elif SMALL
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
#else
/*** algorithm ***
#include <math.h>
#include <errno.h>

__declspec(naked) static double __cdecl f2xm1(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]
		f2xm1
		ret
	}
}

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
				x = f2xm1(f1) + 1;
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
*/
#include <errno.h>

errno_t * __cdecl _errno();

__declspec(naked) double __cdecl exp(double x)
{
	// log2(e) ................ 1.442695040888963407359924681001892137426645954152985934135
	static const double l2e_a = 1.442687988281250000000000000000000000000000000000000000000;	// 0x3FF7154000000000
	static const double l2e_b = 0.000007052607713407359924681001892137426645954152985934135;	// 0x3EDD94AE0BF85DDF

	__asm
	{
		fld     qword ptr [esp + 4]     ; Load x
		fxam                            ; Examine st
		fstsw   ax                      ; Get the FPU status word
		and     ah, 01000101B           ; Isolate  C0, C2 and C3
		cmp     ah, 01000000B           ; Zero ?
		je      L1                      ; Re-direct if x == 0
		cmp     ah, 00000001B           ; NaN ?
		je      L2                      ; Re-direct if x is NaN
		fmul    qword ptr [l2e_a]       ; Multiply:                 f1 = x * l2e_a
		fld     qword ptr [esp + 4]     ; Load real from stack
		fmul    qword ptr [l2e_b]       ; Multiply:                 f2 = x * l2e_b
		fld     st(1)                   ; Duplicate f1
		frndint                         ; Round to integer:         i1 = trunc(f1)
		fld     st(1)                   ; Duplicate f2
		frndint                         ; Round to integer:         i2 = trunc(f2)
		fld     st(1)                   ; Duplicate i1
		fadd    st(0), st(1)            ; Add:                      n = i1 + i2
		fxch    st(4)                   ; Exchange st, st(4)
		fsubrp  st(2), st(0)            ; Subtract:                 f1 -= i1
		fsubp   st(2), st(0)            ; Subtract:                 f2 -= i2
		fadd    st(0), st(1)            ; Add:                      f1 += f2
		fst     st(1)                   ; Push f1
		frndint                         ; Round to integer:         i1 = trunc(f1)
		fadd    st(2), st(0)            ; Add:                      n += i1
		fsubp   st(1), st(0)            ; Subtract:                 f1 -= i1
		f2xm1                           ; Compute 2 to the (x - 1): x = f2xm1(f1) + 1
		fld1                            ; Load real number 1
		fadd                            ; 2 to the x
		fscale                          ; Scale by power of 2:      x = ldexp(x, n);
		fstp    st(1)                   ; Set new stack top and pop
		fstp    qword ptr [esp + 4]     ; Save x, 'fxam' is require the load memory
		fld     qword ptr [esp + 4]     ; Load x
		fxam                            ; Examine st
		fstsw   ax                      ; Get the FPU status word
		and     ah, 01000101B           ; Isolate  C0, C2 and C3
		cmp     ah, 00000101B           ; Infinity ?
		je      L3                      ; Re-direct if x is infinity (overflow)
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
#endif
