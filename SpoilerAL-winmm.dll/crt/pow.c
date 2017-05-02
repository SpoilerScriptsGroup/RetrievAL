#include <math.h>
#include <float.h>
#include <errno.h>

#pragma function(pow)
#pragma function(log)
#pragma function(exp)

double __cdecl pow(double x, double y)
{
	double dummy;

	if (x)
	{
		if (x >= 0 || !modf(y, &dummy))
		{
			unsigned char sign;

			sign = 0;
			if (x < 0)
			{
				if (modf(y / 2, &dummy))
				{
					sign = 1;
				}
				x = -x;
			}
			x = log(x);
			if (x < 0)
			{
				x = -x;
				y = -y;
			}
			if (x <= 1 || y <= DBL_MAX / x)
			{
				x = exp(x * y);
			}
			else
			{
				errno = ERANGE;
				x = HUGE_VAL;
			}
			if (sign)
			{
				x = -x;
			}
		}
	}
	else if (!y || (x < 0 && modf(y, &dummy)))
	{
		errno = EDOM;
		x = 0;
	}
	return x;
}
