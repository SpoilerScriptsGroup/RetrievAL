#define DBL_EXP_BIAS 1023

double modf(double x, double *intptr)
{
	unsigned long int hi, lo;
	long int          exp;
	unsigned long int u;

	lo = * (int *)&x     ;
	hi = *((int *)&x + 1);
	exp = ((hi >> 20) & 0x7FFU) - DBL_EXP_BIAS;
	if (exp < 20)
	{
		if (exp < 0)
		{
			* (int *)intptr      = 0               ;
			*((int *)intptr + 1) = hi & 0x80000000U;
			return x;
		}
		else
		{
			u = 0x000FFFFFU >> exp;
			if (!((hi & u) | lo))
			{
				*intptr = x;
				* (int *)&x      = 0               ;
				*((int *)&x + 1) = hi & 0x80000000U;
				return x;
			}
			else
			{
				* (int *)intptr      = 0      ;
				*((int *)intptr + 1) = hi & ~u;
				return x - *intptr;
			}
		}
	}
	else if (exp > 51)
	{
		const double one = 1.0;

		*intptr = x * one;
		* (int *)&x      = 0               ;
		*((int *)&x + 1) = hi & 0x80000000U;
		return x;
	}
	else
	{
		u = 0xFFFFFFFFU >> (exp - 20);
		if (!(lo & u))
		{
			*intptr = x;
			* (int *)&x      = 0               ;
			*((int *)&x + 1) = hi & 0x80000000U;
			return x;
		}
		else
		{
			* (int *)intptr      = lo & ~u;
			*((int *)intptr + 1) = hi     ;
			return x - *intptr;
		}
	}
}
