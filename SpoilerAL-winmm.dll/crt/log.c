#include <math.h>
#include <float.h>
#include <errno.h>

#pragma function(log)

#ifndef _M_IX86
#ifndef M_SQRT1_2
#define M_SQRT1_2 0.70710678118654752440084436210484903928483593768847403658833986899536623923105351942519376716382078636750692311545614851246241802792536860632206074854996791570661133296375279637789997525057639103028573505477998580298513726729843100736425870932044459930477616461524215435716072541988130181399762570399484362670
#endif
#ifndef M_LN2
#define M_LN2 0.69314718055994530941723212145817656807550013436025525412068000949339362196969471560586332699641868754200148102057068573368552023575813055703267075163507596193072757082837143519030703862389167347112335011536449795523912047517268157493206515552473413952588295045300709532636664265410423915781495204374043038550
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
		fld     qword ptr [esp + 4]     ; Load real from stack
		fldln2                          ; Load log base e of 2
		fxch    st(1)                   ; Exchange st0, st1
		fyl2x                           ; Compute the natural log(x)
		ret
	}
}
#endif
