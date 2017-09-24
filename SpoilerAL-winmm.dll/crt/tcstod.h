#ifdef _tcstod
#include <stdint.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>
#ifdef __GNUC__
#include <sys/param.h>
#endif

double __cdecl _tcstod(const TCHAR *nptr, TCHAR **endptr)
{
#ifndef _UNICODE
	#define SCHAR char
	#define UCHAR unsigned char
#else
	#define SCHAR short
	#define UCHAR wchar_t
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
	#define MSW(x) *((uint32_t *)&(x) + 1)
#else
	#define MSW(x) *((uint32_t *)&(x))
#endif

	double      r;  /* result */
	const UCHAR *p;

	r = 0;
	if (p = (const UCHAR *)nptr)
	{
		#define DBL_EXP_MASK (DBL_MAX_EXP * 2 - 1)      // 0x7FF
		#define MSW_MANT_BIT (DBL_MANT_DIG - 1 - 32)    // 20

		bool sign;

		while (*p == ' ' || (*p <= (UCHAR)'\r' && *p >= (UCHAR)'\t'))
			p++;

		if ((sign = *p == '-') || *p == '+')
			p++;

		if (p[0] != '0' || (p[1] != 'x' && p[1] != 'X'))
		{
			const UCHAR  *decptr;
			unsigned int size, e;

			decptr = p;
			while ((SCHAR)*p >= '0' && *p <= (UCHAR)'9')
				p++;
			size = e = p - decptr;
			if (*p == '.')
			{
				while ((SCHAR)*(++p) >= '0' && *p <= (UCHAR)'9');
				size = p - decptr - 1;
			}

			if (p != decptr)
			{
				const UCHAR *expptr;

				expptr = p;
				if (size > 18)
					size = 18;
				e -= size;

				p = decptr;
				if (size > 9)
				{
					unsigned int i;

					size -= 9;
					i = 0;
					do
					{
						if (*p == '.')
							p++;
						i = 10 * i + *(p++) - '0';
					} while (--size);
					r = 1e9 * i;
					size = 9;
				}
				if (size)
				{
					unsigned int i;

					i = 0;
					do
					{
						if (*p == '.')
							p++;
						i = 10 * i + *(p++) - '0';
					} while (--size);
					r += i;
				}

				p = expptr;
				if (*p == 'e' || *p == 'E')
				{
					bool esign;

					if ((esign = *(++p) == '-') || *p == '+')
						p++;

					if ((SCHAR)*p >= '0' && *p <= (UCHAR)'9')
					{
						unsigned int i;

						i = *(p++) - '0';
						while ((SCHAR)*p >= '0' && *p <= (UCHAR)'9')
							i = i * 10 + *(p++) - '0';
						e += (esign ? -(int)i : i);
					}
					else
					{
						p = expptr;
					}
				}

				if (e && *(uint64_t *)&r)
				{
					bool esign;

					if (esign = (int)e < 0)
						e = -(int)e;

					if (e < 512)
					{
						if (esign)
						{
							if (e & 1) r *= 1e-001; if (e >>= 1) {
							if (e & 1) r *= 1e-002; if (e >>= 1) {
							if (e & 1) r *= 1e-004; if (e >>= 1) {
							if (e & 1) r *= 1e-008; if (e >>= 1) {
							if (e & 1) r *= 1e-016; if (e >>= 1) {
							if (e & 1) r *= 1e-032; if (e >>= 1) {
							if (e & 1) r *= 1e-064; if (e >>= 1) {
							if (e & 1) r *= 1e-128; if (e >>= 1) {
							if (e & 1) r *= 1e-256; } } } } } } } }
							if (!*(uint64_t *)&r)
								errno = ERANGE;
						}
						else
						{
							if (e & 1) r /= 1e-001; if (e >>= 1) {
							if (e & 1) r /= 1e-002; if (e >>= 1) {
							if (e & 1) r /= 1e-004; if (e >>= 1) {
							if (e & 1) r /= 1e-008; if (e >>= 1) {
							if (e & 1) r /= 1e-016; if (e >>= 1) {
							if (e & 1) r /= 1e-032; if (e >>= 1) {
							if (e & 1) r /= 1e-064; if (e >>= 1) {
							if (e & 1) r /= 1e-128; if (e >>= 1) {
							if (e & 1) r /= 1e-256; } } } } } } } }
						}
					}
					else
					{
						r = HUGE_VAL;
					}
				}
			}
			else
			{
				p = (const UCHAR *)nptr;
			}
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
						#define MSW_MANT_MASK ((1U << MSW_MANT_BIT) - 1)    // 0x000FFFFF

						unsigned int e; /* exponent */

						e = *(p++) - '0';
						while ((SCHAR)*p >= '0' && *p <= (UCHAR)'9')
							e = e * 10 + *(p++) - '0';
						if (esign)
							e = -(int)e;
						e += MSW(r) >> MSW_MANT_BIT;
						MSW(r) &= MSW_MANT_MASK;
						if ((int)e > 0)
						{
							if (e < DBL_EXP_MASK)
								MSW(r) |= e << MSW_MANT_BIT;
							else
								r = HUGE_VAL;
						}
						else if (*(uint64_t *)&r)
						{
							if ((int)e > -(DBL_MANT_DIG - 1))
							{
								MSW(r) |= 1 << MSW_MANT_BIT;
								*(uint64_t *)&r >>= -(int)e + 1;
							}
							else
							{
								errno = ERANGE;
								*(uint64_t *)&r = 0;
							}
						}

						#undef MSW_MANT_MASK
					}
				}
			}
			else
			{
				p = (const UCHAR *)nptr;
			}
		}

		if ((MSW(r) >> MSW_MANT_BIT) == DBL_EXP_MASK)
			errno = ERANGE;

		if (sign)
			MSW(r) |= 0x80000000;

		#undef DBL_EXP_MASK
		#undef MSW_MANT_BIT
	}

	if (endptr)
		*endptr = (TCHAR *)p;

	return r;

	#undef SCHAR
	#undef UCHAR
	#undef MSW
}
#endif
