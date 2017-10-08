#pragma function(exp)

#ifndef _M_IX86
#include <math.h>
#include <float.h>
#include <errno.h>

#ifndef M_LN2
#define M_LN2 0.693147180559945309417232121458176568
#endif
#ifndef M_LOG2E
#define M_LOG2E 1.44269504088896340735992468100189214
#endif
#ifndef M_LN_MAX_D
#define M_LN_MAX_D (M_LN2 * DBL_MAX_EXP)
#endif
#ifndef M_LN_MIN_D
#define M_LN_MIN_D (M_LN2 * (DBL_MIN_EXP - 1))
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

double __cdecl exp(double x)
{
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
				{
					x = -x;
				}
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
}
#else
__declspec(naked) double __cdecl exp(double x)
{
	fld     qword [esp + 4]         ; Load real from stack
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
#endif
