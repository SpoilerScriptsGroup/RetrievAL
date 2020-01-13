#include <math.h>

double __cdecl cbrt(double x)
{
	double y;

	y = 0.0;
	if (x != 0.0)
	{
		y = x;
		if (x < 0.0)
			y = -y;
		y = exp(log(y) / 3.0);
		if (x < 0.0)
			y = -y;
	}
	return y;
}
