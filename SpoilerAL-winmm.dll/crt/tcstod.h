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

		if ((sign = *p == '-') || *p == '+')
			p++;

		if (p[0] != '0' || (p[1] != 'x' && p[1] != 'X'))
		{
			#define MAX_EXPONENT 511

			const UCHAR  *decptr;
			unsigned int size, exp;

			decptr = p;
			while ((SCHAR)*p >= '0' && *p <= (UCHAR)'9')
				p++;
			size = exp = p - decptr;

			if (*p == '.')
			{
				while ((SCHAR)*(++p) >= '0' && *p <= (UCHAR)'9');
				size = p - decptr - 1;
			}

			if (p != decptr)
			{
				unsigned int i;
				const UCHAR  *expptr;

				p = decptr;

				if (size > 18)
					size = 18;

				exp -= size;

				i = 0;
				for (; size > 9; size--)
				{
					if (*p == '.')
						p++;
					i = 10 * i + (*(p++) - '0');
				}
				r = 1e9 * i;
				if (size)
				{
					i = 0;
					do
					{
						if (*p == '.')
							p++;
						i = 10 * i + (*(p++) - '0');
					} while (--size);
					r += i;
				}

				expptr = p;
				if (*p == 'e' || *p == 'E')
				{
					bool esign;

					if ((esign = *(++p) == '-') || *p == '+')
						p++;
					if ((SCHAR)*p >= '0' && *p <= (UCHAR)'9')
					{
						i = 0;
						do
						{
							i = i * 10 + (*(p++) - '0');
						} while ((SCHAR)*p >= '0' && *p <= (UCHAR)'9');
						exp += (esign ? -(int)i : i);
					}
					else
					{
						p = expptr;
						goto DONE;
					}
				}

				if (exp)
				{
					bool esign;

					if (esign = (int)exp < 0)
						exp = -(int)exp;

					if (exp <= MAX_EXPONENT)
					{
						static const double powers[] = { 1e1, 1e2, 1e4, 1e8, 1e16, 1e32, 1e64, 1e128, 1e256 };

						double       d;
						const double *pwr;

						d = 1;
						pwr = powers;
						do
						{
							if (exp & 1)
								d *= *pwr;
							pwr++;
						} while (exp >>= 1);
						if (esign)
							r /= d;
						else
							r *= d;
					}
					else
					{
						errno = ERANGE;
						r = HUGE_VAL;
					}
				}
			}
			else
			{
				p = (const UCHAR *)nptr;
			}

			#undef MAX_EXPONENT
		}
		else
		{
			const UCHAR *decptr;

			decptr = p += 2;

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

			if (p != decptr)
			{
				if (*p == 'p' || *p == 'P')
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
			else
			{
				p = (const UCHAR *)nptr;
			}
		}

	DONE:
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
