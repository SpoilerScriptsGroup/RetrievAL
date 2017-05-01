#include <math.h>
#include <float.h>
#include <errno.h>

double __cdecl ldexp(double x, int exp)
{
	int sign;
	int currexp;

	if (_isnan(x)) {
		errno = EDOM;
		return x;
	}
	if (x == 0.0)
		return 0.0;
	sign = 1;
	if (x < 0) {
		x = -x;
		sign = -1;
	}
	if (x > DBL_MAX) {
		errno = ERANGE;
		return sign * x;
	}
	x = frexp(x, &currexp);
	exp += currexp;
	if (exp > 0) {
		if (exp > DBL_MAX_EXP) {
			errno = ERANGE;
			return sign * HUGE_VAL;
		}
		while (exp>30) {
			x *= (double)(1L << 30);
			exp -= 30;
		}
		x *= (double)(1L << exp);
	}
	else {
		if (exp < DBL_MIN_EXP - DBL_MANT_DIG) {
			return 0.0;
		}
		while (exp < -30) {
			x /= (double)(1L << 30);
			exp += 30;
		}
		x /= (double)(1L << -exp);
	}
	return sign * x;
}
