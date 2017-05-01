#include <math.h>
#include <float.h>
#include <errno.h>

#pragma function(pow)
#pragma function(log)
#pragma function(exp)

double __cdecl pow(double x, double y)
{
	double dummy;
	int    result_neg;

	if ((x == 0 && y == 0) || (x < 0 && modf(y, &dummy) != 0)) {
		errno = EDOM;
		return 0;
	}
	if (x == 0)
		return x;
	result_neg = 0;
	if (x < 0) {
		if (modf(y / 2.0, &dummy) != 0) {
			result_neg = 1;
		}
		x = -x;
	}
	x = log(x);
	if (x < 0) {
		x = -x;
		y = -y;
	}
	if (x > 1.0 && y > DBL_MAX / x) {
		errno = ERANGE;
		return result_neg ? -HUGE_VAL : HUGE_VAL;
	}
	x = exp(x * y);
	return result_neg ? -x : x;
}
