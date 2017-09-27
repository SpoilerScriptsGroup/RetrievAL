#ifdef _tcstod
#ifdef __GNUC__
#include <sys/param.h>
#endif
#include <stdint.h>
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

	#define DBL_MANT_BIT  (DBL_MANT_DIG - 1)            // 52
	#define DBL_EXP_MASK  (DBL_MAX_EXP * 2 - 1)         // 0x7FF

#if __BYTE_ORDER == __LITTLE_ENDIAN
	#define MSW(x)        *((uint32_t *)&(x) + 1)
#else
	#define MSW(x)        *((uint32_t *)&(x))
#endif
	#define MSW_MANT_BIT  (DBL_MANT_BIT - 32)           // 20
	#define MSW_MANT_MASK ((1U << MSW_MANT_BIT) - 1)    // 0x000FFFFF
	#define MSW_SIGN      0x80000000

	double      r;  /* result */
	const UCHAR *p;
	bool        sign;

	r = 0;
	if (!(p = (const UCHAR *)nptr))
		goto L_SET_ENDPTR;

	while (*p == ' ' || (*p <= (UCHAR)'\r' && *p >= (UCHAR)'\t'))
		p++;

	if ((sign = *p == '-') || *p == '+')
		p++;

	if (p[0] == 'I' || p[0] == 'i')
		goto L_INF;

	if (p[0] == 'N' || p[0] == 'n')
		goto L_NAN;

	if (p[0] != '0' || (p[1] != 'x' && p[1] != 'X'))
	{
		const UCHAR  *mantptr, *expptr;
		size_t       width;
		int32_t      e;

		while (*p == '0')
			p++;
		mantptr = p;
		while ((SCHAR)*p >= '0' && *p <= (UCHAR)'9')
			p++;
		e = (int32_t)(width = p - mantptr);
		if (*p == '.')
		{
			while ((SCHAR)*(++p) >= '0' && *p <= (UCHAR)'9');
			width = p - mantptr - 1;
		}

		if (!width)
			goto L_INVALIDATE;

		expptr = p;
		if (width > DBL_DECIMAL_DIG + 1)
			width = DBL_DECIMAL_DIG + 1;
		e -= width;

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
			r = 1e9 * i;
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
				uint32_t i;

				i = *(p++) - '0';
				while ((SCHAR)*p >= '0' && *p <= (UCHAR)'9')
					i = i * 10 + *(p++) - '0';
				e += (esign ? -(int32_t)i : i);
			}
			else
			{
				p = expptr;
			}
		}

		if (e && *(uint64_t *)&r)
		{
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
						goto L_SET_ERRNO;
				}
				else
				{
					goto L_UNDERFLOW;
				}
			}
		}
	}
	else
	{
		const UCHAR *mantptr, *expptr;
		size_t      width;

		p += 2;
		while (*p == '0')
			p++;
		mantptr = p;
		for (; ; )
		{
			UCHAR c;

			if ((SCHAR)(c = *p) <= '9')
			{
				if ((SCHAR)(c -= '0') < 0)
					break;
			}
			else
			{
				if (c <= (UCHAR)'F' ? (SCHAR)(c -= 'A') < 0 : c > (UCHAR)'f' || (SCHAR)(c -= 'a') < 0)
					break;
				c += 0x0A;
			}
			r = r * 0x10 + c;
			if (r != HUGE_VAL)
			{
				p++;
			}
			else
			{
				while ((SCHAR)(c = *(++p)) <= '9' ? (SCHAR)c >= '0' :
					c <= (UCHAR)'F' ? c >= (UCHAR)'A' :
					c <= (UCHAR)'f' && c >= (UCHAR)'a');
				break;
			}
		}
		width = p - mantptr;

		if (*p == '.')
		{
			double d;

			d = 1;
			for (; ; )
			{
				UCHAR c;

				if ((SCHAR)(c = *(++p)) <= '9')
				{
					if ((SCHAR)(c -= '0') < 0)
						break;
				}
				else
				{
					if (c <= (UCHAR)'F' ? (SCHAR)(c -= 'A') < 0 : c > (UCHAR)'f' || (SCHAR)(c -= 'a') < 0)
						break;
					c += 0x0A;
				}
				if (d *= 1.0 / 0x10)
				{
					r += d * c;
				}
				else
				{
					*(uint64_t *)&d = c >> 2;
					r += d;
					while ((SCHAR)(c = *(++p)) <= '9' ? (SCHAR)c >= '0' :
						c <= (UCHAR)'F' ? c >= (UCHAR)'A' :
						c <= (UCHAR)'f' && c >= (UCHAR)'a');
					break;
				}
			}
			width = p - mantptr - 1;
		}

		if (!width)
			goto L_INVALIDATE;

		expptr = p;
		if (*p == 'p' || *p == 'P')
		{
			bool esign;

			if ((esign = (*(++p) == '-')) || *p == '+')
				p++;

			if ((SCHAR)*p >= '0' && *p <= (UCHAR)'9')
			{
				int32_t e;

				e = *(p++) - '0';
				while ((SCHAR)*p >= '0' && *p <= (UCHAR)'9')
					e = (uint32_t)e * 10 + *(p++) - '0';
				if (esign)
					e = -e;
				e += MSW(r) >> MSW_MANT_BIT;
				MSW(r) &= MSW_MANT_MASK;
				if (e > 0)
				{
					if ((uint32_t)e < DBL_EXP_MASK)
					{
						MSW(r) |= e << MSW_MANT_BIT;
					}
					else
					{
L_OVERFLOW:
						r = HUGE_VAL;
						goto L_SET_ERRNO;
					}
				}
				else if (*(uint64_t *)&r)
				{
					if (e > -DBL_MANT_BIT)
					{
						MSW(r) |= 1 << MSW_MANT_BIT;
						*(uint64_t *)&r >>= -e + 1;
					}
					else
					{
L_UNDERFLOW:
						r = 0;
						goto L_SET_ERRNO;
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
L_SET_ERRNO:
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
	p = (const UCHAR *)nptr;
	goto L_SET_ENDPTR;

L_SET_SIGN:
	if (sign)
		MSW(r) |= MSW_SIGN;

L_SET_ENDPTR:
	if (endptr)
		*endptr = (TCHAR *)p;

	return r;

	#undef SCHAR
	#undef UCHAR
	#undef DBL_MANT_BIT
	#undef DBL_EXP_MASK
	#undef MSW
	#undef MSW_MANT_BIT
	#undef MSW_MANT_MASK
	#undef MSW_SIGN
}
#endif
