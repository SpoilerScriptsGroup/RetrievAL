#ifdef __GNUC__
#include <sys/param.h>
#endif
#include <stdint.h>
#include <float.h>
#include <math.h>
#include <errno.h>
#include <tchar.h>
#include "atoitbl.h"

#if !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)
#if '\4\3\2\1' == 0x01020304 || defined(_MSC_VER)
#define __LITTLE_ENDIAN__   1
#elif '\4\3\2\1' == 0x04030201
#define __BIG_ENDIAN__      1
#else
#error Current byte order is not supported.
#endif
#endif

#if defined(_MSC_VER) && defined(_M_IX86)
double __cdecl ldexp10(double x, int e);
#endif

double __cdecl _tcstod(const TCHAR *nptr, TCHAR **endptr)
{
#ifdef _UNICODE
	typedef wchar_t       uchar_t;
	typedef short         schar_t;
#else
	typedef unsigned char uchar_t;
	typedef char          schar_t;
#endif

	#define DBL_MANT_BIT  (DBL_MANT_DIG - 1)                    // 52
	#define DBL_EXP_MASK  (DBL_MAX_EXP * 2 - 1)                 // 0x7FF
	#define DBL_MANT_MASK ((UINT64_C(1) << DBL_MANT_BIT) - 1)   // 0x000FFFFFFFFFFFFF
	#define DBL_SIGN      UINT64_C(0x8000000000000000)          // 0x8000000000000000

#ifdef _WIN64
	#define MSW(x)        *((uint64_t *)&(x))
	#define MSW_MANT_BIT  DBL_MANT_BIT
	#define MSW_MANT_MASK DBL_MANT_MASK
	#define MSW_SIGN      DBL_SIGN
#else
#if defined(__LITTLE_ENDIAN__)
	#define MSW(x)        *((uint32_t *)&(x) + 1)
#else
	#define MSW(x)        *((uint32_t *)&(x))
#endif
	#define MSW_MANT_BIT  (DBL_MANT_BIT - 32)                   // 20
	#define MSW_MANT_MASK ((UINT32_C(1) << MSW_MANT_BIT) - 1)   // 0x000FFFFF
	#define MSW_SIGN      UINT32_C(0x80000000)                  // 0x80000000
#endif

	double        r;  /* result */
	const uchar_t *p;
	uchar_t       sign;

	r = 0;
	if (!(p = nptr))
		goto L_SET_ENDPTR;

	while (*p == ' ' || (uchar_t)(*p - '\t') < '\r' - '\t' + 1)
		p++;

	if ((sign = *p) == '-' || sign == '+')
		p++;

	if (p[0] == 'I' || p[0] == 'i')
		goto L_INF;

	if (p[0] == 'N' || p[0] == 'n')
		goto L_NAN;

	if (p[0] != '0' || (p[1] != 'x' && p[1] != 'X'))
	{
		const uchar_t *first, *mantptr, *expptr;
		size_t        width;
		uint64_t      x;
		int32_t       e;

		first = p;
		while (*p == '0')
			p++;
		mantptr = p;
		while ((uchar_t)(*p - '0') < '9' - '0' + 1)
			p++;
		e = (int32_t)(width = p - mantptr);
		if (*p == '.')
		{
			while ((uchar_t)(*(++p) - '0') < '9' - '0' + 1);
			width = p - mantptr - 1;
		}

		if (!width && mantptr == first)
			goto L_INVALIDATE;

		expptr = p;
		if (width > DBL_DECIMAL_DIG + 1)
			width = DBL_DECIMAL_DIG + 1;
		e -= width;

		x = 0;
		p = mantptr;
		if (width > 9)
		{
			uint32_t i;

			width -= 9;
			i = 0;
			do
			{
				if (*p == '.')
					p++;
				i = i * 10 + *(p++) - '0';
			} while (--width);
			x = (uint64_t)1e9 * i;
			width = 9;
		}
		if (width)
		{
			uint32_t i;

			i = 0;
			do
			{
				if (*p == '.')
					p++;
				i = i * 10 + *(p++) - '0';
			} while (--width);
			x += i;
		}

		p = expptr;
		if (*p == 'e' || *p == 'E')
		{
			uchar_t esign, c;

			if ((esign = c = *(++p)) == '-' || c == '+')
				c = *(++p);

			if ((uchar_t)(c -= '0') < '9' - '0' + 1)
			{
				uint32_t i;

				i = c;
				while ((uchar_t)(c = *(++p) - '0') < '9' - '0' + 1)
					i = i * 10 + c;
				if (esign == '-')
					i = -(int32_t)i;
				e += i;
			}
			else
			{
				p = expptr;
			}
		}

		r = (double)x;
		if (e && *(uint64_t *)&r)
		{
#if !defined(_MSC_VER) || !defined(_M_IX86)
			if (e >= 0)
			{
				if ((uint32_t)e < 512)
				{
					// multiply "1e+???" has error of floting-point number.

					if (e & 1) r /= 1e-001; if (e = (uint32_t)e >> 1) {
					if (e & 1) r /= 1e-002; if (e = (uint32_t)e >> 1) {
					if (e & 1) r /= 1e-004; if (e = (uint32_t)e >> 1) {
					if (e & 1) r /= 1e-008; if (e = (uint32_t)e >> 1) {
					if (e & 1) r /= 1e-016; if (e = (uint32_t)e >> 1) {
					if (e & 1) r /= 1e-032; if (e = (uint32_t)e >> 1) {
					if (e & 1) r /= 1e-064; if (e = (uint32_t)e >> 1) {
					if (e & 1) r /= 1e-128; if (e = (uint32_t)e >> 1) {
					if (e & 1) r /= 1e-256; } } } } } } } }
				}
				else
				{
					goto L_OVERFLOW;
				}
			}
			else
			{
				if ((uint32_t)(e = -e) < 512)
				{
					if (e & 1) r *= 1e-001; if (e = (uint32_t)e >> 1) {
					if (e & 1) r *= 1e-002; if (e = (uint32_t)e >> 1) {
					if (e & 1) r *= 1e-004; if (e = (uint32_t)e >> 1) {
					if (e & 1) r *= 1e-008; if (e = (uint32_t)e >> 1) {
					if (e & 1) r *= 1e-016; if (e = (uint32_t)e >> 1) {
					if (e & 1) r *= 1e-032; if (e = (uint32_t)e >> 1) {
					if (e & 1) r *= 1e-064; if (e = (uint32_t)e >> 1) {
					if (e & 1) r *= 1e-128; if (e = (uint32_t)e >> 1) {
					if (e & 1) r *= 1e-256; } } } } } } } }
					if (!*(uint64_t *)&r)
						goto L_ERANGE;
				}
				else
				{
					goto L_UNDERFLOW;
				}
			}
#else
			r = ldexp10(r, e);
			if (!*(uint64_t *)&r)
				goto L_ERANGE;
#endif
		}
	}
	else
	{
		const uchar_t *first, *expptr;
		size_t        width;
		uchar_t       c;

		c = *(first = p += 2);
		while (c == '0')
			c = *(++p);
		while (CTOI(&c, 'f', 16))
		{
			r = r * 0x10 + c;
			if (r != HUGE_VAL)
			{
				c = *(++p);
			}
			else
			{
				do
					c = *(++p);
				while (CHECK_CTOI(c, 'f', 16));
				break;
			}
		}
		width = p - first;

		if (*p == '.')
		{
			double d;

			d = 1;
			c = *(++p);
			while (CTOI(&c, 'f', 16))
			{
				if (d *= 1.0 / 0x10)
				{
					r += d * c;
					c = *(++p);
				}
				else
				{
					*(uint64_t *)&d = c >> 2;
					r += d;
					do
						c = *(++p);
					while (CHECK_CTOI(c, 'f', 16));
					break;
				}
			}
			width = p - first - 1;
		}

		if (!width)
			goto L_INVALIDATE;

		expptr = p;
		if (*p == 'p' || *p == 'P')
		{
			uchar_t esign;

			if ((esign = c = *(++p)) == '-' || c == '+')
				c = *(++p);

			if ((uchar_t)(c -= '0') < '9' - '0' + 1)
			{
				uint32_t e;

				e = c;
				while ((uchar_t)(c = *(++p) - '0') < '9' - '0' + 1)
					e = e * 10 + c;
				if (esign == '-')
					e = -(int32_t)e;
				e += MSW(r) >> MSW_MANT_BIT;
				MSW(r) &= MSW_MANT_MASK;
				if (e > 0)
				{
					if ((uint32_t)e < DBL_EXP_MASK)
					{
						MSW(r) |= (uintptr_t)e << MSW_MANT_BIT;
					}
					else
					{
#if !defined(_MSC_VER) || !defined(_M_IX86)
L_OVERFLOW:
#endif
						r = HUGE_VAL;
						goto L_ERANGE;
					}
				}
				else if (*(uint64_t *)&r)
				{
					if (e > -DBL_MANT_BIT)
					{
						MSW(r) |= (uintptr_t)1 << MSW_MANT_BIT;
						*(uint64_t *)&r >>= -(int32_t)e + 1;
					}
					else
					{
#if !defined(_MSC_VER) || !defined(_M_IX86)
L_UNDERFLOW:
#endif
						r = 0;
						goto L_ERANGE;
					}
				}
			}
			else
			{
				p = expptr;
			}
		}
	}

	if ((MSW(r) >> MSW_MANT_BIT) == DBL_EXP_MASK)
L_ERANGE:
		errno = ERANGE;
	goto L_SET_SIGN;

L_INF:
	if ((p[1] != 'N' && p[1] != 'n') || (p[2] != 'F' && p[2] != 'f'))
		goto L_INVALIDATE;
	p += 3;
	if ((p[0] == 'I' || p[0] == 'i') && (p[1] == 'N' || p[1] == 'n') && (p[2] == 'I' || p[2] == 'i') && (p[3] == 'T' || p[3] == 't') && (p[4] == 'Y' || p[4] == 'y'))
		p += 5;
	r = HUGE_VAL;
	goto L_SET_SIGN;

L_NAN:
	if ((p[1] != 'A' && p[1] != 'a') || (p[2] != 'N' && p[2] != 'n'))
		goto L_INVALIDATE;
	p += 3;
	*(uint64_t *)&r = INT64_MAX;
	goto L_SET_SIGN;

L_INVALIDATE:
	p = nptr;
	goto L_SET_ENDPTR;

L_SET_SIGN:
	if (sign == '-')
#ifdef _WIN64
		*(uint64_t *)&r |= DBL_SIGN;
#else
		MSW(r) |= MSW_SIGN;
#endif

L_SET_ENDPTR:
	if (endptr)
		*endptr = (TCHAR *)p;

	return r;

	#undef DBL_MANT_BIT
	#undef DBL_EXP_MASK
	#undef DBL_MANT_MASK
	#undef DBL_SIGN
	#undef MSW
	#undef MSW_MANT_BIT
	#undef MSW_MANT_MASK
	#undef MSW_SIGN
}
