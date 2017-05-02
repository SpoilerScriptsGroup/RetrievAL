#include <math.h>
#include <float.h>
#include <errno.h>

double __cdecl ldexp(double x, int exp)
{
	if (!_isnan(x))
	{
		if (x)
		{
			unsigned char sign;

			sign = x < 0;
			if (sign)
			{
				x = -x;
			}
			if (x <= DBL_MAX)
			{
				int currexp;

				x = frexp(x, &currexp);
				exp += currexp;
				if (exp > 0)
				{
					if (exp <= DBL_MAX_EXP)
					{
						while (exp > 30)
						{
							x *= (double)(1L << 30);
							exp -= 30;
						}
						x *= (double)(1L << exp);
					}
					else
					{
						errno = ERANGE;
						x = HUGE_VAL;
					}
				}
				else
				{
					if (exp >= DBL_MIN_EXP - DBL_MANT_DIG)
					{
						while (exp < -30)
						{
							x /= (double)(1L << 30);
							exp += 30;
						}
						x /= (double)(1L << -exp);
					}
					else
					{
						x = 0;
						sign = 0;
					}
				}
			}
			else
			{
				errno = ERANGE;
			}
			if (sign)
			{
				x = -x;
			}
		}
		else
		{
			x = 0;
		}
	}
	else
	{
		errno = EDOM;
	}
	return x;
}
