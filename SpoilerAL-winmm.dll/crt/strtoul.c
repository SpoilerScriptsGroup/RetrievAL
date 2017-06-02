#include <windows.h>
#include <errno.h>

unsigned long __cdecl strtoul(const char *nptr, char **endptr, int base)
{
	const char    *p;
	char          c;
	unsigned long number;
	unsigned      digval;
	unsigned long maxval;
	BOOLEAN       overflow;
	BOOLEAN       neg;

	overflow = FALSE;
	neg = FALSE;
	p = nptr;                   /* p is our scanning pointer */
	number = 0;                 /* start with zero */

	c = *p;                     /* read char */
	while (c == ' ' || ((unsigned char)c <= (unsigned char)'\r' && (unsigned char)c >= (unsigned char)'\t'))
		c = *(++p);             /* skip whitespace */

	if (c == '-')
	{
		neg = TRUE;             /* remember minus sign */
		c = *(++p);
	}
	else if (c == '+')
		c = *(++p);             /* skip sign */

	if (base == 1 || (unsigned int)base > (unsigned int)(10 + 'Z' - 'A' + 1))
	{
		/* bad base! */
		errno = EINVAL;
		if (endptr)
			/* store beginning of string in endptr */
			*endptr = (char *)nptr;
		return 0UL;             /* return 0 */
	}
	else if (base == 0)
	{
		/* determine base free-lance, based on first two chars of
		   string */
		if (c != '0')
			base = 10;
		else if (*(p + 1) == 'x' || *(p + 1) == 'X')
		{
			c = *(p += 2);
			base = 16;
		}
		else
			base = 8;
	}

	/* convert c to value */
	if (c >= '0' && (unsigned char)c <= (unsigned char)'9')
		digval = c - '0';
	else if (c >= 'A' && (unsigned char)c <= (unsigned char)'Z')
		digval = c - 'A' + 10;
	else if (c >= 'a' && (unsigned char)c <= (unsigned char)'z')
		digval = c - 'a' + 10;
	else
	{
		/* no number there; return 0 and point to beginning of
		   string */
		if (endptr)
			/* store beginning of string in endptr later on */
			*endptr = (char *)nptr;
		return 0UL;             /* return 0 */
	}

	/* if our number exceeds this, we will overflow on multiply */
	maxval = ULONG_MAX / base;

	for (; ; )                  /* exit in middle of loop */
	{
		/* we now need to compute number = number * base + digval,
		   but we need to know if overflow occured.  This requires
		   a tricky pre-check. */

		if (number < maxval || (number == maxval && digval <= (unsigned int)(ULONG_MAX % base)))
			/* we won't overflow, go ahead and multiply */
			number = number * base + digval;
		else
			/* we would have overflowed -- set the overflow flag */
			overflow = TRUE;

		c = *(++p);             /* read next digit */

	    /* convert c to value */
		if (c >= '0' && (unsigned char)c <= (unsigned char)'9')
			digval = c - '0';
		else if (c >= 'A' && (unsigned char)c <= (unsigned char)'Z')
			digval = c - 'A' + 10;
		else if (c >= 'a' && (unsigned char)c <= (unsigned char)'z')
			digval = c - 'a' + 10;
		else
			break;
		if (digval >= (unsigned int)base)
			break;              /* exit loop if bad digit found */
	}

	if (endptr != NULL)
		/* store pointer to char that stopped the scan */
		*endptr = (char *)p;

	if (overflow)
	{
		/* overflow or signed overflow occurred */
		errno = ERANGE;
		number = ULONG_MAX;
	}
	else if (neg)
		/* negate result if there was a neg sign */
		number = (unsigned long)(-(long)number);

	return number;              /* done. */
}
