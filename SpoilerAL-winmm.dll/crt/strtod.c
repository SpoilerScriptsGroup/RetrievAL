#define NULL ((void *)0)

double __cdecl strtod(const char *nptr, char **endptr)
{
	long double  r;     /* result */
	unsigned int e;     /* exponent */
	long double  d;     /* scale */
	int          sign;  /* +-1.0 */
	int          esign;
	unsigned int i;
	int          flags;

	flags = 0;
	r = 0.0;
	sign = 1;
	e = 0;
	esign = 1;

	if (nptr == NULL)
		return r;

	while (*nptr == ' ' || ((unsigned char)*nptr <= (unsigned char)'\r' && (unsigned char)*nptr >= (unsigned char)'\t'))
		nptr++;

	if (*nptr == '+')
		nptr++;
	else if (*nptr == '-')
	{
		sign = -1;
		nptr++;
	}

	while (*nptr >= '0' && (unsigned char)*nptr <= (unsigned char)'9')
	{
		flags |= 1;
		r *= 10.0;
		r += *nptr - '0';
		nptr++;
	}

	if (*nptr == '.')
	{
		d = 0.1L;
		nptr++;
		while (*nptr >= '0' && (unsigned char)*nptr <= (unsigned char)'9')
		{
			flags |= 2;
			r += d * (*nptr - '0');
			nptr++;
			d *= 0.1L;
		}
	}

	if (flags == 0)
	{
		if (endptr)
			*endptr = (char *)nptr;
		return 0;
	}

	if ((*nptr == 'e') || (*nptr == 'E'))
	{
		nptr++;
		if (*nptr == '+')
			nptr++;
		else if (*nptr == '-')
		{
			nptr++;
			esign = -1;
		}
		if (*nptr < '0' || (unsigned char)*nptr > (unsigned char)'9')
		{
			if (endptr)
				*endptr = (char *)nptr;
			return r;
		}

		while (*nptr >= '0' && (unsigned char)*nptr <= (unsigned char)'9')
		{
			e *= 10;
			e += *nptr - '0';
			nptr++;
		}
	}

	if (esign < 0)
		for (i = 1; i <= e; i++)
			r *= 0.1L;
	else
		for (i = 1; i <= e; i++)
			r *= 10.0;

	if (endptr)
		*endptr = (char *)nptr;
	return r * sign;
}
