#include <float.h>
#include <math.h>

double __cdecl ldexp10(double x, int exp);

double __cdecl frexp10(double x, int *expptr)
{
	if (_finite(x))
		x = ldexp10(x, -(*expptr = (int)floor(log10(x))));
	else
		*expptr = -1;
	return x;
}
