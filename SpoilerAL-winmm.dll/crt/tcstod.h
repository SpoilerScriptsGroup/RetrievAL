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
	#define MSW_SIGN_MASK 0x80000000

	double      r;  /* result */
	const UCHAR *p;
	bool        sign;

	r = 0;
	if (!(p = (const UCHAR *)nptr))
		goto SET_ENDPTR;

	while (*p == ' ' || (*p <= (UCHAR)'\r' && *p >= (UCHAR)'\t'))
		p++;

	if ((sign = *p == '-') || *p == '+')
		p++;

	if (p[0] == 'I' || p[0] == 'i')
		goto INF;

	if (p[0] == 'N' || p[0] == 'n')
		goto NaN;

	if (p[0] != '0' || (p[1] != 'x' && p[1] != 'X'))
	{
		const UCHAR  *mant, *expptr;
		unsigned int width, e;

		while (*p == '0')
			p++;
		mant = p;
		while ((SCHAR)*p >= '0' && *p <= (UCHAR)'9')
			p++;
		width = e = p - mant;
		if (*p == '.')
		{
			while ((SCHAR)*(++p) >= '0' && *p <= (UCHAR)'9');
			width = p - mant - 1;
		}

		if (!width)
			goto INVALIDATE;

		expptr = p;
		if (width > 18)
			width = 18;
		e -= width;

		p = mant;
		if (width > 9)
		{
			unsigned int i;

			width -= 9;
			i = 0;
			do
			{
				if (*p == '.')
					p++;
				i = i * 10 + *(p++) - '0';
			} while (--width);
			r = 1e9 * i;
			width = 9;
		}
		if (width)
		{
			unsigned int i;

			i = 0;
			do
			{
				if (*p == '.')
					p++;
				i = i * 10 + *(p++) - '0';
			} while (--width);
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
		goto CHECK_OVERFLOW;
	}
	else
	{
		#define DBL_EXP_MASK (DBL_MAX_EXP * 2 - 1)      // 0x7FF
		#define MSW_MANT_BIT (DBL_MANT_DIG - 1 - 32)    // 20

		const UCHAR  *mant;
		unsigned int width;

		p += 2;
		while (*p == '0')
			p++;
		mant = p;
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
		width = p - mant;

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
				r += (d *= 1.0 / 0x10) * c;
			}
			width = p - mant - 1;
		}

		if (!width)
			goto INVALIDATE;

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
						r = 0;
					}
				}

				#undef MSW_MANT_MASK
			}
		}

	CHECK_OVERFLOW:
		if ((MSW(r) >> MSW_MANT_BIT) == DBL_EXP_MASK)
			errno = ERANGE;
		goto SET_SIGN;

		#undef DBL_EXP_MASK
		#undef MSW_MANT_BIT
	}

INF:
	if ((p[1] != 'N' && p[1] != 'n') || (p[2] != 'F' && p[2] != 'f'))
		goto INVALIDATE;
	if ((p[3] != 'I' && p[3] != 'i') || (p[4] != 'N' && p[4] != 'n') || (p[5] != 'I' && p[5] != 'i') || (p[6] != 'T' && p[6] != 't') || (p[7] != 'Y' && p[7] != 'y'))
		p += 3;
	else
		p += 8;
	*(uint64_t *)&r = 0x7FF0000000000000;
	goto SET_SIGN;

NaN:
	if ((p[1] != 'A' && p[1] != 'a') || (p[2] != 'N' && p[2] != 'n'))
		goto INVALIDATE;
	p += 3;
	*(uint64_t *)&r = 0x7FFFFFFFFFFFFFFF;
	goto SET_SIGN;

INVALIDATE:
	p = (const UCHAR *)nptr;
	goto SET_ENDPTR;

SET_SIGN:
	if (sign)
		MSW(r) |= MSW_SIGN_MASK;

SET_ENDPTR:
	if (endptr)
		*endptr = (TCHAR *)p;

	return r;

	#undef SCHAR
	#undef UCHAR
	#undef MSW
	#undef MSW_SIGN_MASK
}
#endif
