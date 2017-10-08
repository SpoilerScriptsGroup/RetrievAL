#pragma function(log)

#ifndef _M_IX86
#include <math.h>
#include <float.h>
#include <errno.h>

#ifndef M_SQRT1_2
#define M_SQRT1_2 0.707106781186547524400844362104849039
#endif
#ifndef M_LN2
#define M_LN2 0.693147180559945309417232121458176568
#endif

#define A0   -0.64124943423745581147e2
#define A1    0.16383943563021534222e2
#define A2   -0.78956112887491257267e0
#define A(x) ((A2 * (x) + A1) * (x) + A0)

#define B0   -0.76949932108494879777e3
#define B1    0.31203222091924532844e3
#define B2   -0.35667977739034646171e2
#define B(x) ((((x) + B2) * (x) + B1) * (x) + B0)

double __cdecl log(double x)
{
	if (!_isnan(x))
	{
		if (x > 0)
		{
			if (x <= DBL_MAX)
			{
				double znum, zden, z, w;
				int    exponent;

				x = frexp(x, &exponent);
				if (x > M_SQRT1_2)
				{
					znum = (x - 0.5) - 0.5;
					zden = x * 0.5 + 0.5;
				}
				else
				{
					znum = x - 0.5;
					zden = znum * 0.5 + 0.5;
					exponent--;
				}
				z = znum / zden;
				w = z * z;
				x = z + z * w * (A(w) / B(w));
				x += exponent * M_LN2;
			}
		}
		else
		{
			errno = x ? EDOM : ERANGE;
			x = -HUGE_VAL;
		}
	}
	else
	{
		errno = EDOM;
	}
	return x;
}
#else
__declspec(naked) double __cdecl log(double x)
{
	__asm
	{
		fldln2                          ; Load log base e of 2
		fld     qword ptr [esp + 4]     ; Load real from stack
		fyl2x                           ; Compute the natural log(x)
		ret
	}
}
#endif
