#ifdef _tcstod
#include <stdbool.h>
#include <float.h>
#include <math.h>

double __cdecl _tcstod(const TCHAR *nptr, TCHAR **endptr)
{
#ifndef _UNICODE
	#define SCHAR char
	#define UCHAR unsigned char
#else
	#define SCHAR short
	#define UCHAR wchar_t
#endif
	#define LSW(x) *((unsigned long int *)&(x))
	#define MSW(x) *((unsigned long int *)&(x) + 1)

	double      r;  /* result */
	const UCHAR *p;

	r = 0;
	if (p = (const UCHAR *)nptr)
	{
		bool sign;

		while (*p == ' ' || (*p <= (UCHAR)'\r' && *p >= (UCHAR)'\t'))
			p++;

		if ((sign = (*p == '-')) || *p == '+')
			p++;

		nptr = (const TCHAR *)p;

		if (p[0] != '0' || (p[1] != 'x' && p[1] != 'X'))
		{
			while ((SCHAR)*p >= '0' && *p <= (UCHAR)'9')
				r = r * 10 + (*(p++) - '0');

			if (*p == '.')
			{
				double d;   /* scale */

				d = 1;
				while ((SCHAR)*(++p) >= '0' && *p <= (UCHAR)'9')
					r += (d *= 0.1) * (*p - '0');
			}

			if (p != (const UCHAR *)nptr && (*p == 'e' || *p == 'E'))
			{
				bool esign;

				if ((esign = (*(++p) == '-')) || *p == '+')
					p++;

				if ((SCHAR)*p >= '0' && *p <= (UCHAR)'9')
				{
					unsigned int e; /* exponent */

					e = *(p++) - '0';
					while ((SCHAR)*p >= '0' && *p <= (UCHAR)'9')
						e = e * 10 + (*(p++) - '0');

					if (esign)
						while (e--)
							r *= 0.1;
					else
						while (e--)
							r *= 10;
				}
			}

			if (MSW(r) == 0x7FEFFFFF && LSW(r) > 0xFFFFFFF8)
				r = DBL_MAX;
		}
		else
		{
			p += 2;

			for (; ; )
			{
				UCHAR c;

				if ((SCHAR)*p < '0')
					break;
				if (*p <= (UCHAR)'9')
					c = *p - '0';
				else if (*p >= (UCHAR)'A' && *p <= (UCHAR)'F')
					c = *p - ('A' - 0x0A);
				else if (*p >= (UCHAR)'a' && *p <= (UCHAR)'f')
					c = *p - ('a' - 0x0A);
				else
					break;
				r = r * 0x10 + c;
				p++;
			}

			if (*p == '.')
			{
				double d;   /* scale */

				d = 1;
				for (; ; )
				{
					UCHAR c;

					if ((SCHAR)*(++p) < '0')
						break;
					if (*p <= (UCHAR)'9')
						c = *p - '0';
					else if (*p >= (UCHAR)'A' && *p <= (UCHAR)'F')
						c = *p - ('A' - 0x0A);
					else if (*p >= (UCHAR)'a' && *p <= (UCHAR)'f')
						c = *p - ('a' - 0x0A);
					else
						break;
					r += (d *= 0.0625/* (1.0 / 0x10) */) * c;
				}
			}

			if (p != (const UCHAR *)nptr && (*p == 'p' || *p == 'P'))
			{
				bool esign;

				if ((esign = (*(++p) == '-')) || *p == '+')
					p++;

				if ((SCHAR)*p >= '0' && *p <= (UCHAR)'9')
				{
					unsigned int e; /* exponent */

					e = *(p++) - '0';
					while ((SCHAR)*p >= '0' && *p <= (UCHAR)'9')
						e = e * 10 + (*(p++) - '0');
					if (esign)
						e = -(int)e;
					e += MSW(r) >> 20;
					MSW(r) &= 0x000FFFFF;
					if ((int)e > 0)
					{
						if (e < 0x7FF)
							MSW(r) |= e << 20;
						else
							r = HUGE_VAL;
					}
					else
					{
						MSW(r) |= 0x00100000;
						e = -(int)e + 1;
						if (e <= 52)
							*(unsigned __int64 *)&r >>= e;
						else
							*(unsigned __int64 *)&r = 0;
					}
				}
			}
		}

		if ((MSW(r) >> 20) == 0x7FF)
			errno = ERANGE;

		if (sign)
			r = -r;
	}

	if (endptr)
		*endptr = (TCHAR *)p;

	return r;

	#undef SCHAR
	#undef UCHAR
	#undef LSW
	#undef MSW
}
#endif
