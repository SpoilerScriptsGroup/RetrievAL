#define DBL_EXP_BIAS 1023

#define LO(value) \
	*(unsigned long int *)(value)

#define HI(value) \
	*((unsigned long int *)(value) + 1)

double __cdecl modf(double x, double *intptr)
{
	unsigned long int hi, lo;
	long int          exp;
	unsigned long int u;

	lo = LO(&x);
	hi = HI(&x);
	exp = ((hi >> 20) & 0x7FFU) - DBL_EXP_BIAS;
	if (exp < 20)
	{
		if (exp < 0)
		{
			LO(intptr) = 0;
			HI(intptr) = hi & 0x80000000U;
		}
		else
		{
			u = 0x000FFFFFU >> exp;
			if (!((hi & u) | lo))
			{
				*intptr = x;
				LO(&x) = 0;
				HI(&x) = hi & 0x80000000U;
			}
			else
			{
				LO(intptr) = 0;
				HI(intptr) = hi & ~u;
				x -= *intptr;
			}
		}
	}
	else if (exp > 51)
	{
		const double one = 1.0;

		*intptr = x * one;
		LO(&x) = 0;
		HI(&x) = hi & 0x80000000U;
	}
	else
	{
		u = 0xFFFFFFFFU >> (exp - 20);
		if (!(lo & u))
		{
			*intptr = x;
			LO(&x) = 0;
			HI(&x) = hi & 0x80000000U;
		}
		else
		{
			LO(intptr) = lo & ~u;
			HI(intptr) = hi;
			x -= *intptr;
		}
	}
	return x;
}
