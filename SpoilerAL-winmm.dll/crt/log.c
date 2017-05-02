#include <math.h>
#include <float.h>
#include <errno.h>

#pragma function(log)

#ifndef M_SQRT1_2
#define M_SQRT1_2 0.707106781186547524400844362104849039
#endif

#ifndef POLYNOM3
#define POLYNOM1(x, a) ((a)[1] * (x) + (a)[0])
#define POLYNOM2(x, a) (POLYNOM1((x), (a) + 1) * (x) + (a)[0])
#define POLYNOM3(x, a) (POLYNOM2((x), (a) + 1) * (x) + (a)[0])
#endif

double __cdecl log(double x)
{
	const double a[] = {
		-0.64124943423745581147e2,
		 0.16383943563021534222e2,
		-0.78956112887491257267e0
	};
	const double b[] = {
		-0.76949932108494879777e3,
		 0.31203222091924532844e3,
		-0.35667977739034646171e2,
		 1.0
	};

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
				x = z + z * w * (POLYNOM2(w, a) / POLYNOM3(w, b));
				x += exponent * (-2.121944400546905827679e-4 + 0.693359375);
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
