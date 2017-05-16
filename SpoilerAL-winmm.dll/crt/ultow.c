#if defined(_MSC_VER) && _MSC_VER > 1400
#include <intrin.h>
#endif

#if defined(_MSC_VER)
#if _MSC_VER > 1400 && (defined(_M_IX86) || defined(_M_X64))
#define div10_32bit(value) \
	(unsigned long)(__emulu(value, 0xCCCCCCCDUL) >> 35)
#define div10_30bit(value) \
	(unsigned long)(__emulu(value, 0x1999999AUL) >> 32)
#else
#define div10_32bit(value) \
	(unsigned long)(((unsigned __int64)(unsigned long)(value) * 0xCCCCCCCDUL) >> 35)
#define div10_30bit(value) \
	(unsigned long)(((unsigned __int64)(unsigned long)(value) * 0x1999999AUL) >> 32)
#endif
#else
#define div10_32bit(value) \
	(unsigned long)(((unsigned long long)(unsigned long)(value) * 0xCCCCCCCDUL) >> 35)
#define div10_30bit(value) \
	(unsigned long)(((unsigned long long)(unsigned long)(value) * 0x1999999AUL) >> 32)
#endif

__inline unsigned long mul10(unsigned long value)
{
	value += value * 4;
	value += value;
	return value;
}

wchar_t * __cdecl _ultow(unsigned long value, wchar_t *str, int radix)
{
	wchar_t *p1, *p2;

	p2 = p1 = str;

	/* check radix */
	if (radix == 10)
	{
		unsigned long quot;

		/* digit */
		quot = div10_32bit(value);
		*(p1++) = (char)(value - quot * 10 + L'0');
		while (value = quot)
		{
			quot = div10_30bit(value);
			*(p1++) = (char)(value - quot * 10 + L'0');
		}
	}
	else if (radix == 16)
	{
		/* letter or digit */
		do
		{
			unsigned long rem;

			/* convert to ascii and store */
			rem = value & 15;
			*(p1++) = (wchar_t)rem + (rem < 10 ? L'0' : L'A' - 10);
		}
		while (value >>= 4);
	}
	else if (radix == 8)
	{
		/* digit */
		do
		{
			/* convert to ascii and store */
			*(p1++) = ((wchar_t)value & 7) + L'0';
		}
		while (value >>= 3);
	}
	else if (radix == 4)
	{
		/* digit */
		do
		{
			/* convert to ascii and store */
			*(p1++) = ((wchar_t)value & 3) + L'0';
		}
		while (value >>= 2);
	}
	else if (radix == 2)
	{
		/* digit */
		do
		{
			/* convert to ascii and store */
			*(p1++) = ((wchar_t)value & 1) + L'0';
		}
		while (value >>= 1);
	}
	else if (radix > 1)
	{
		if (radix < 10)
		{
			do
			{
				*(p1++) = (wchar_t)(value % (unsigned long)radix) + L'0';
				value /= (unsigned long)radix;
			} while (value);
		}
		else if (radix <= 10 + L'Z' - L'A' + 1)
		{
			do
			{
				unsigned long rem;

				rem = value % (unsigned long)radix;
				value /= (unsigned long)radix;
				*(p1++) = (wchar_t)rem + (rem < 10 ? L'0' : L'A' - 10);
			} while (value);
		}
	}

	/* We now have the digit of the number in the buffer, but in reverse */
	/* order.  Thus we reverse them now.                                 */

	*(p1--) = L'\0';    /* terminate string; p points to last digit */
	while (p1 > p2)     /* repeat until halfway */
	{
		wchar_t c1, c2;

		c1 = *p1;       /* swap *p1 and *p2 */
		c2 = *p2;
		*(p1--) = c2;
		*(p2++) = c1;   /* advance to next two digits */
	}

	return str;
}
