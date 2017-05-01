#include <math.h>
#include <float.h>
#include <errno.h>

#pragma function(log)

#ifndef M_SQRT1_2
#define M_SQRT1_2 0.70710678118654752440084436210484904
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

	double znum, zden, z, w;
	int    exponent;

	if (_isnan(x)) {
		errno = EDOM;
		return x;
	}
	if (x < 0) {
		errno = EDOM;
		return -HUGE_VAL;
	}
	else if (x == 0) {
		errno = ERANGE;
		return -HUGE_VAL;
	}
	if (x > DBL_MAX)
		return x;
	x = frexp(x, &exponent);
	if (x > M_SQRT1_2) {
		znum = (x - 0.5) - 0.5;
		zden = x * 0.5 + 0.5;
	}
	else {
		znum = x - 0.5;
		zden = znum * 0.5 + 0.5;
		exponent--;
	}
	z = znum / zden; w = z * z;
	x = z + z * w * (POLYNOM2(w, a) / POLYNOM3(w, b));
	z = exponent;
	x += z * (-2.121944400546905827679e-4);
	return x + z * 0.693359375;
}
