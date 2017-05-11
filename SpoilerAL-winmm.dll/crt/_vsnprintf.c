#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>    // using IsDBCSLeadByte
#include <winternl.h>   // using PANSI_STRING, PUNICODE_STRING
#if _MSC_VER > 1400
#include <intrin.h>
#pragma intrinsic(__emulu)
#endif
#endif

#ifndef NEAR
#define NEAR
#endif
#ifndef FAR
#define FAR
#endif

// standard integer type definition
#if !defined(_MSC_VER) || _MSC_VER >= 1600
#include <stdint.h>
#else
typedef __int8           int8_t;
typedef unsigned __int8  uint8_t;
typedef __int16          int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32          int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64          int64_t;
typedef unsigned __int64 uint64_t;
typedef __int64          intmax_t;
typedef unsigned __int64 uintmax_t;
#include <limits.h>
#define INT8_MIN    _I8_MIN
#define INT16_MIN   _I16_MIN
#define INT32_MIN   _I32_MIN
#define INT64_MIN   _I64_MIN
#define INT8_MAX    _I8_MAX
#define INT16_MAX   _I16_MAX
#define INT32_MAX   _I32_MAX
#define INT64_MAX   _I64_MAX
#define UINT8_MAX   _UI8_MAX
#define UINT16_MAX  _UI16_MAX
#define UINT32_MAX  _UI32_MAX
#define UINT64_MAX  _UI64_MAX
#define INTMAX_MAX  _I64_MAX
#define UINTMAX_MAX _UI64_MAX
#endif

#include <math.h>   // using modf
#include <float.h>  // using DBL_MANT_DIG, DBL_MAX_EXP

// byte-order definition
#if defined(_MSC_VER) || defined(__MINGW32__)
#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN    4321
#define __BYTE_ORDER    __LITTLE_ENDIAN
#elif defined(__GNUC__)
#include <sys/param.h>
#endif

#ifdef _DEBUG
#include <assert.h>
#endif

// compiler dependent
#ifdef _MSC_VER
#ifndef _WIN64
#define ARCH32 1    // 32bit application
#define ARCH64 0
#else
#define ARCH32 0
#define ARCH64 1    // 64bit application
#endif
#ifdef isleadbyte
#undef isleadbyte
#endif
#define isleadbyte IsDBCSLeadByte
#endif

#define LONGDOUBLE_IS_DOUBLE (!defined(LDBL_MANT_DIG) || (LDBL_MANT_DIG == DBL_MANT_DIG))

// type definition
#if LONGDOUBLE_IS_DOUBLE
typedef double long_double;
#else
typedef long double long_double;
#endif

// floating-point macro function
#if LONGDOUBLE_IS_DOUBLE
#if ARCH32
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define LSW(x) ((uint32_t *)&(x))[0]
#define MSW(x) ((uint32_t *)&(x))[1]
#else
#define LSW(x) ((uint32_t *)&(x))[1]
#define MSW(x) ((uint32_t *)&(x))[0]
#endif
#endif
#ifdef isinf
#undef isinf
#endif
#if ARCH32
#define isinf(x) ((MSW(x) & 0x7FF00000) == 0x7FF00000)
#else
#define isinf(x) ((*(uint64_t *)&(x) & 0x7FF0000000000000) == 0x7FF0000000000000)
#endif
#ifdef isnan
#undef isnan
#endif
#if ARCH32
#define isnan(x) (isinf(x) && ((MSW(x) & 0x000FFFFF) || LSW(x)))
#else
#define isnan(x) (isinf(x) && (*(uint64_t *)&(x) & 0x000FFFFFFFFFFFFF))
#endif
#ifdef signbit
#undef signbit
#endif
#if ARCH32
#define signbit(x) ((int32_t)MSW(x) < 0)
#else
#define signbit(x) (*(int64_t *)&(x) < 0)
#endif
#endif

#if LONGDOUBLE_IS_DOUBLE
#ifdef isinfl
#undef isinfl
#endif
#define isinfl isinf
#ifdef isnanl
#undef isnanl
#endif
#define isnanl isnan
#ifdef signbitl
#undef signbitl
#endif
#define signbitl signbit
#ifdef modfl
#undef modfl
#endif
#define modfl modf
#endif

// floating-point constant definision
#if LONGDOUBLE_IS_DOUBLE
#ifdef LDBL_MANT_DIG
#undef LDBL_MANT_DIG
#endif
#define LDBL_MANT_DIG DBL_MANT_DIG
#ifdef LDBL_MAX_EXP
#undef LDBL_MAX_EXP
#endif
#define LDBL_MAX_EXP DBL_MAX_EXP
#endif

#define LDBL_BITS      (sizeof(long_double) * 8)
#define LDBL_SIGN_BITS 1
#define LDBL_MANT_BITS (LDBL_MANT_DIG - 1)
#define LDBL_EXP_BITS  (LDBL_BITS - LDBL_SIGN_BITS - LDBL_MANT_BITS)
#define LDBL_SIGN_MASK ((uintmax_t)1 << (LDBL_BITS - 1))
#define LDBL_MANT_MASK (((uintmax_t)1 << LDBL_MANT_BITS) - 1)
#define LDBL_EXP_MASK  ((LDBL_SIGN_MASK - 1) & ~LDBL_MANT_MASK)
#define LDBL_EXP_BIAS  (LDBL_MAX_EXP - 1)

#if !defined(CVTBUFSIZE) && defined(_CVTBUFSIZE)
#define CVTBUFSIZE _CVTBUFSIZE
#endif

// floating-point structure definision
typedef union _UNION_LONGDOUBLE {
	struct {
#if __BYTE_ORDER == __LITTLE_ENDIAN
		uintmax_t mantissa : LDBL_MANT_BITS;
		uintmax_t exponent : LDBL_EXP_BITS;
		uintmax_t sign     : LDBL_SIGN_BITS;
#else
		uintmax_t sign     : LDBL_SIGN_BITS;
		uintmax_t exponent : LDBL_EXP_BITS;
		uintmax_t mantissa : LDBL_MANT_BITS;
#endif
	};
	long_double value;
} UNION_LONGDOUBLE, NEAR *PUNION_LONGDOUBLE, FAR *LPUNION_LONGDOUBLE;

// integer macro function
#if defined(_MSC_VER) && _MSC_VER > 1400 && (defined(_M_IX86) || defined(_M_X64))
#define uldiv10(value) \
	(uint32_t)(__emulu(value, 0xCCCCCCCDUL) >> 35)
#else
#define uldiv10(value) \
	(uint32_t)(((uint64_t)(uint32_t)(value) * 0xCCCCCCCDUL) >> 35)
#endif

/*
 * Buffer size to hold the octal string representation of UINTMAX_MAX without
 * nul-termination.
 * if UINTMAX_MAX is UINT128_MAX then 43 ("3777777777777777777777777777777777777777777").
 */
#define UINTMAX_LENGTH ((sizeof(uintmax_t) * 8) / 3 + !!((sizeof(uintmax_t) * 8) % 3))

// Get number of characters from integer (0 <= max_value <= UINT64_MAX)
#define DECIMAL_LENGTH(max_value) ( \
	(max_value) < 0 ? 0 : \
	(max_value) < 10 ? 1 : \
	(max_value) < 100 ? 2 : \
	(max_value) < 1000 ? 3 : \
	(max_value) < 10000 ? 4 : \
	(max_value) < 100000 ? 5 : \
	(max_value) < 1000000 ? 6 : \
	(max_value) < 10000000 ? 7 : \
	(max_value) < 100000000 ? 8 : \
	(max_value) < 1000000000 ? 9 : \
	(max_value) < 10000000000 ? 10 : \
	(max_value) < 100000000000 ? 11 : \
	(max_value) < 1000000000000 ? 12 : \
	(max_value) < 10000000000000 ? 13 : \
	(max_value) < 100000000000000 ? 14 : \
	(max_value) < 1000000000000000 ? 15 : \
	(max_value) < 10000000000000000 ? 16 : \
	(max_value) < 100000000000000000 ? 17 : \
	(max_value) < 1000000000000000000 ? 18 : 19)

// Number of characters of the mantissa of floating-point number.
// strlen("fffffffffffff")
#define MANTISSA_HEX_LENGTH ((LDBL_MANT_BITS + (4 - 1)) / 4)

// Number of characters of the exponent of floating-point number.
// strlen("1024")
#define EXPONENT_LENGTH DECIMAL_LENGTH(LDBL_MAX_EXP)

// strlen("e-") + EXPONENT_LENGTH
#define EXPBUFSIZE  (2 + EXPONENT_LENGTH)

/* Format flags. */
#define FL_SIGN         0x0001  /* put plus or minus in front */
#define FL_SIGNSP       0x0002  /* put space or minus in front */
#define FL_LEFT         0x0004  /* left justify */
#define FL_LEADZERO     0x0008  /* pad with leading zeros */
#define FL_ALTERNATE    0x0010  /* alternate form requested */
#define FL_QUOTE        0x0020
#define FL_UP           0x0040
#define FL_UNSIGNED     0x0080
#define FL_TYPE_E       0x0100
#define FL_TYPE_G       0x0200
#define FL_TYPE_A       0x0400

/* Conversion flags. */
enum {
	C_DEFAULT,
	C_CHAR,
	C_SHORT,
	C_LONG,
	C_LLONG,
	C_LDOUBLE,
	C_SIZE,
	C_PTRDIFF,
	C_INTMAX,
#ifdef _MSC_VER
	C_INT,
	C_INT32,
	C_INT64,
	C_WCHAR,
#endif
};

#ifndef ALIGN
#define ALIGN(x, a) (((x) + (a) - 1) & ~((a) - 1))
#endif

#ifndef CHARTOINT
#define CHARTOINT(c) ((c) - '0')
#endif

#ifndef ISDIGIT
#define ISDIGIT(c) ((c) >= '0' && (c) <= '9')
#endif

#define OUTCHAR(dest, end, c) \
    if (++(dest) < (end))     \
        *(dest) = (c)

static const char digitsLarge[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
static const char digitsSmall[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
static const char *lpcszNull    = "(null)";
static const char *lpcszNil     = "(nil)";
static const char *lpcszNan     = "nan";
static const char *lpcszNanInd  = "nan(ind)";
static const char *lpcszInf     = "inf";

static char *strfmt(char *, const char *, const char *, size_t, ptrdiff_t, int);
static char *intfmt(char *, const char *, intmax_t, unsigned char, size_t, ptrdiff_t, int);
static char *fltfmt(char *, const char *, long_double, size_t, ptrdiff_t, int);

int __cdecl _vsnprintf(char *buffer, size_t count, const char *format, va_list argptr)
{
	char          *dest;
	const char    *end;
	unsigned char overflow;
	char          c;

	/*
	 * C99 says: "If `n' is zero, nothing is written, and `s' may be a null
	 * pointer." (7.19.6.5, 2)  We're forgiving and allow a NULL pointer
	 * even if a count larger than zero was specified.  At least NetBSD's
	 * snprintf(3) does the same, as well as other versions of this file.
	 * (Though some of these versions will write to a non-NULL buffer even
	 * if a count of zero was specified, which violates the standard.)
	 */
	if (buffer == NULL)
		count = 0;

	dest = buffer - 1;
	end = buffer + count;

	overflow = 0;
	c = *(format++);
	while (c)
	{
		int             flags;
		size_t          width;
		ptrdiff_t       precision;
		int             cflags;
		unsigned char   base;
		long_double     fvalue;
		intmax_t        value;
		unsigned char   cvalue;
#ifdef _MSC_VER
		long int        ivalue;
		wchar_t         wvalue;
		char            wbuf[2];
#endif
		char            *strvalue;
#ifdef _MSC_VER
		wchar_t         *wstrvalue;
#endif
		intmax_t        *intmaxptr;
		ptrdiff_t       *ptrdiffptr;
		size_t          *sizeptr;
		long long int   *llongptr;
		long int        *longptr;
		int             *intptr;
		short int       *shortptr;
		char            *charptr;
#ifdef _MSC_VER
		PANSI_STRING    as;
		PUNICODE_STRING us;
		HANDLE          handle;
#endif

		if (c != '%')
		{
			if (isleadbyte(c))
			{
				OUTCHAR(dest, end, c);
				if (!(c = *(format++)))
					break;
			}
			OUTCHAR(dest, end, c);
			c = *(format++);
			continue;
		}
		c = *(format++);

		// Process flags
		flags = 0;
		for (; ; )
		{
			switch (c)
			{
			case '-':
				flags |= FL_LEFT;
				c = *(format++);
				continue;
			case '+':
				flags |= FL_SIGN;
				c = *(format++);
				continue;
			case ' ':
				flags |= FL_SIGNSP;
				c = *(format++);
				continue;
			case '#':
				flags |= FL_ALTERNATE;
				c = *(format++);
				continue;
			case '0':
				flags |= FL_LEADZERO;
				c = *(format++);
				continue;
			case '\'':
				/* SUSv2 flag (not in C99). */
				flags |= FL_QUOTE;
				c = *(format++);
				continue;
			}
			break;
		}

		// Get field width
		width = 0;
		for (; ; )
		{
			if (ISDIGIT(c))
			{
				c = CHARTOINT(c);
				if (width >= (size_t)(INT_MAX / 10) && (
					width > (size_t)(INT_MAX / 10) ||
					(unsigned char)c > (unsigned char)(INT_MAX % 10)))
				{
					overflow = 1;
					goto NESTED_BREAK;
				}
				width = width * 10 + c;
				c = *(format++);
				continue;
			}
			if (c == '*')
			{
				/*
				 * C99 says: "A negative field width argument is
				 * taken as a `-' flag followed by a positive
				 * field width." (7.19.6.1, 5)
				 */
				if ((ptrdiff_t)(width = (ptrdiff_t)va_arg(argptr, int)) < 0)
				{
					flags |= FL_LEFT;
					width = -(ptrdiff_t)width;
				}
				c = *(format++);
			}
			break;
		}

		// Get precision
		precision = -1;
		if (c == '.')
		{
			c = *(format++);
			for (; ; )
			{
				if (precision < 0)
					precision = 0;
				if (ISDIGIT(c))
				{
					c = CHARTOINT(c);
					if ((size_t)precision >= (size_t)(INT_MAX / 10) && (
						(size_t)precision >(size_t)(INT_MAX / 10) ||
						(unsigned char)c > (unsigned char)(INT_MAX % 10)))
					{
						overflow = 1;
						goto NESTED_BREAK;
					}
					precision = (size_t)precision * 10 + c;
					c = *(format++);
					continue;
				}
				if (c == '*')
				{
					/*
					 * C99 says: "A negative precision argument is
					 * taken as if the precision were omitted."
					 * (7.19.6.1, 5)
					 */
					if ((precision = va_arg(argptr, int)) < 0)
						precision = -1;
					c = *(format++);
				}
				break;
			}
		}

		// Get the conversion qualifier
		cflags = C_DEFAULT;
		for (; ; )
		{
			switch (c)
			{
			case 'h':
				c = *(format++);
				if (c == 'h')
				{
					/* It's a char. */
					c = *(format++);
					cflags = C_CHAR;
				}
				else
					cflags = C_SHORT;
				continue;
			case 'l':
				c = *(format++);
				if (c == 'l')
				{
					/* It's a long long. */
					c = *(format++);
					cflags = C_LLONG;
				}
				else
					cflags = C_LONG;
				continue;
#ifdef _MSC_VER
			case 'I':
				c = *(format++);
				if (c == '6')
				{
					if (*format == '4')
					{
						format++;
						c = *(format++);
						cflags = C_INT64;
						continue;
					}
				}
				else if (c == '3')
				{
					if (*format == '2')
					{
						format++;
						c = *(format++);
						cflags = C_INT32;
						continue;
					}
				}
				cflags = C_INT;
				continue;
#endif
			case 'L':
				cflags = C_LDOUBLE;
				c = *(format++);
				continue;
			case 'j':
				cflags = C_INTMAX;
				c = *(format++);
				continue;
			case 't':
				cflags = C_PTRDIFF;
				c = *(format++);
				continue;
			case 'z':
				cflags = C_SIZE;
				c = *(format++);
				continue;
#ifdef _MSC_VER
			case 'w':
				cflags = C_WCHAR;
				c = *(format++);
				continue;
#endif
			}
			break;
		}

		base = 0;
		switch (c)
		{
		case 'd':
			/* FALLTHROUGH */
		case 'i':
			switch (cflags)
			{
			case C_CHAR:
				value = (char)va_arg(argptr, int);
				break;
			case C_SHORT:
				value = (short int)va_arg(argptr, int);
				break;
			case C_LONG:
#ifdef _MSC_VER
			case C_INT32:
#endif
				value = va_arg(argptr, long int);
				break;
			case C_LLONG:
#ifdef _MSC_VER
			case C_INT64:
#endif
				value = va_arg(argptr, long long int);
				break;
			case C_SIZE:
#ifdef _MSC_VER
			case C_INT:
#endif
				value = va_arg(argptr, size_t);
				break;
			case C_INTMAX:
				value = va_arg(argptr, intmax_t);
				break;
			case C_PTRDIFF:
				value = va_arg(argptr, ptrdiff_t);
				break;
			default:
				value = va_arg(argptr, int);
				break;
			}
			dest = intfmt(dest, end, value, 10, width, precision, flags);
			break;
		case 'X':
			flags |= FL_UP;
			/* FALLTHROUGH */
		case 'x':
			base = 16;
			/* FALLTHROUGH */
		case 'o':
			if (!base)
				base = 8;
			/* FALLTHROUGH */
		case 'u':
			if (!base)
				base = 10;
			flags |= FL_UNSIGNED;
			switch (cflags)
			{
			case C_CHAR:
				value = (unsigned char)va_arg(argptr, unsigned int);
				break;
			case C_SHORT:
				value = (unsigned short int)va_arg(argptr, unsigned int);
				break;
			case C_LONG:
#ifdef _MSC_VER
			case C_INT32:
#endif
				value = va_arg(argptr, unsigned long int);
				break;
			case C_LLONG:
#ifdef _MSC_VER
			case C_INT64:
#endif
				value = va_arg(argptr, unsigned long long int);
				break;
			case C_SIZE:
#ifdef _MSC_VER
			case C_INT:
#endif
				value = va_arg(argptr, size_t);
				break;
			case C_INTMAX:
				value = va_arg(argptr, uintmax_t);
				break;
			case C_PTRDIFF:
				value = va_arg(argptr, ptrdiff_t);
				break;
			default:
				value = va_arg(argptr, unsigned int);
				break;
			}
			dest = intfmt(dest, end, value, base, width, precision, flags);
			break;
		case 'F':
			flags |= FL_UP;
		case 'f':
#if !LONGDOUBLE_IS_DOUBLE
			if (cflags == C_LDOUBLE)
				fvalue = va_arg(argptr, long_double);
			else
#endif
				fvalue = va_arg(argptr, double);
			dest = fltfmt(dest, end, fvalue, width, precision, flags);
			if (overflow)
				goto NESTED_BREAK;
			break;
		case 'E':
			flags |= FL_UP;
			/* FALLTHROUGH */
		case 'e':
			flags |= FL_TYPE_E;
#if !LONGDOUBLE_IS_DOUBLE
			if (cflags == C_LDOUBLE)
				fvalue = va_arg(argptr, long_double);
			else
#endif
				fvalue = va_arg(argptr, double);
			dest = fltfmt(dest, end, fvalue, width, precision, flags);
			break;
		case 'G':
			flags |= FL_UP;
			/* FALLTHROUGH */
		case 'g':
			flags |= FL_TYPE_G;
#if !LONGDOUBLE_IS_DOUBLE
			if (cflags == C_LDOUBLE)
				fvalue = va_arg(argptr, long_double);
			else
#endif
				fvalue = va_arg(argptr, double);
			dest = fltfmt(dest, end, fvalue, width, precision, flags);
			break;
		case 'A':
			flags |= FL_UP;
		case 'a':
			flags |= FL_TYPE_A;
#if !LONGDOUBLE_IS_DOUBLE
			if (cflags == C_LDOUBLE)
				fvalue = va_arg(argptr, long_double);
			else
#endif
				fvalue = va_arg(argptr, double);
			dest = fltfmt(dest, end, fvalue, width, precision, flags);
			break;
		case 'c':
#ifdef _MSC_VER
			if (cflags == C_LONG || cflags == C_WCHAR)
				goto PRINT_WCHAR;
		PRINT_CHAR:
#endif
			cvalue = va_arg(argptr, int);
			OUTCHAR(dest, end, cvalue);
			break;
#ifdef _MSC_VER
		case 'C':
			if (cflags == C_SHORT || cflags == C_CHAR)
				goto PRINT_CHAR;
		PRINT_WCHAR:
			wvalue = va_arg(argptr, int);
			ivalue = WideCharToMultiByte(CP_ACP, 0, &wvalue, 1, wbuf, 2, NULL, NULL);
			if (ivalue <= 0)
				break;
			OUTCHAR(dest, end, wbuf[0]);
			if (!--ivalue)
				break;
			OUTCHAR(dest, end, wbuf[1]);
			break;
#endif
		case 's':
#ifdef _MSC_VER
			if (cflags == C_LONG || cflags == C_WCHAR)
				goto PRINT_WSTR;
		PRINT_STR:
#endif
			strvalue = va_arg(argptr, char *);
			dest = strfmt(dest, end, strvalue, width, precision, flags);
			break;
#ifdef _MSC_VER
		case 'S':
			if (cflags == C_SHORT || cflags == C_CHAR)
				goto PRINT_STR;
		PRINT_WSTR:
			strvalue = NULL;
			ivalue = 0;
			wstrvalue = va_arg(argptr, wchar_t *);
			if (wstrvalue)
				if (ivalue = WideCharToMultiByte(CP_ACP, 0, wstrvalue, -1, NULL, 0, NULL, NULL))
					if (strvalue = (char *)HeapAlloc(handle = GetProcessHeap(), 0, ++ivalue * sizeof(char)))
						ivalue = WideCharToMultiByte(CP_ACP, 0, wstrvalue, -1, strvalue, ivalue, NULL, NULL);
			dest = strfmt(dest, end, ivalue ? strvalue : NULL, width, precision, flags);
			if (strvalue)
				HeapFree(handle, 0, strvalue);
			break;
#endif
		case 'p':
			/*
			 * C99 says: "The value of the pointer is
			 * converted to a sequence of printing
			 * characters, in an implementation-defined
			 * manner." (C99: 7.19.6.1, 8)
			 */
			if ((strvalue = va_arg(argptr, void *)) == NULL)
			{
				/*
				 * We use the glibc format.  BSD prints
				 * "0x0", SysV "0".
				 */
				dest = strfmt(dest, end, lpcszNil, width, -1, flags);
			}
			else
			{
				/*
				 * We use the BSD/glibc format.  SysV
				 * omits the "0x" prefix (which we emit
				 * using the FL_ALTERNATE flag).
				 */
#ifndef _MSC_VER
				flags |= FL_UNSIGNED | FL_UP | FL_ALTERNATE;
#else
				flags |= FL_UNSIGNED | FL_UP;
#endif
				dest = intfmt(dest, end, (uintptr_t)strvalue, 16, width, sizeof(void *) * 2, flags);
			}
			break;
		case 'n':
			switch (cflags)
			{
			case C_CHAR:
				charptr = va_arg(argptr, char *);
				*charptr = (char)(dest + 1 - buffer);
				break;
			case C_SHORT:
				shortptr = va_arg(argptr, short int *);
				*shortptr = (short int)(dest + 1 - buffer);
				break;
			case C_LONG:
#ifdef _MSC_VER
			case C_INT32:
#endif
				longptr = va_arg(argptr, long int *);
				*longptr = (long int)(dest + 1 - buffer);
				break;
			case C_LLONG:
#ifdef _MSC_VER
			case C_INT64:
#endif
				llongptr = va_arg(argptr, long long int *);
				*llongptr = (long long int)(dest + 1 - buffer);
				break;
			case C_SIZE:
#ifdef _MSC_VER
			case C_INT:
#endif
				/*
				 * C99 says that with the "z" length
				 * modifier, "a following `n' conversion
				 * specifier applies to a pointer to a
				 * signed integer type corresponding to
				 * size_t argument." (7.19.6.1, 7)
				 */
				sizeptr = va_arg(argptr, size_t *);
				*sizeptr = dest + 1 - buffer;
				break;
			case C_INTMAX:
				intmaxptr = va_arg(argptr, intmax_t *);
				*intmaxptr = dest + 1 - buffer;
				break;
			case C_PTRDIFF:
				ptrdiffptr = va_arg(argptr, ptrdiff_t *);
				*ptrdiffptr = dest + 1 - buffer;
				break;
			default:
				intptr = va_arg(argptr, int *);
				*intptr = (int)(dest + 1 - buffer);
				break;
			}
			break;
#ifdef _MSC_VER
		case 'Z':
			switch (cflags)
			{
			case C_LONG:
			case C_WCHAR:
				strvalue = NULL;
				ivalue = 0;
				us = va_arg(argptr, PUNICODE_STRING);
				if (us && us->Buffer)
					if (ivalue = WideCharToMultiByte(CP_ACP, 0, us->Buffer, -1, NULL, 0, NULL, NULL))
						if (strvalue = (char *)HeapAlloc(handle = GetProcessHeap(), 0, ++ivalue * sizeof(char)))
							ivalue = WideCharToMultiByte(CP_ACP, 0, us->Buffer, -1, strvalue, ivalue, NULL, NULL);
				dest = strfmt(dest, end, ivalue ? strvalue : NULL, width, precision, flags);
				if (strvalue)
					HeapFree(handle, 0, strvalue);
				break;
			default:
				as = va_arg(argptr, PANSI_STRING);
				dest = strfmt(dest, end, as ? as->Buffer : NULL, width, precision, flags);
				break;
			}
			break;
#endif
		case '%':
			/* Print a "%" character verbatim. */
			OUTCHAR(dest, end, c);
			break;
		default:
			/* Skip other characters. */
			if (isleadbyte(c))
				if (!*(format++))
					goto NESTED_BREAK;
			break;
		}
		c = *format++;
	}
NESTED_BREAK:
	if (++dest < end)
		*dest = '\0';
	else if (count)
		*(char *)(end - 1) = '\0';

	if (overflow || (size_t)(dest - buffer) >= INT_MAX)
	{
		errno = overflow ? EOVERFLOW : ERANGE;
		return -1;
	}
	return dest - buffer;
}

static char *strfmt(char *dest, const char *end, const char *value, size_t width, ptrdiff_t precision, int flags)
{
	ptrdiff_t padlen;	/* Amount to pad. */
	size_t    strln;

	/* We're forgiving. */
	if (value == NULL)
		value = lpcszNull;

	/* If a precision was specified, don't read the string past it. */
	for (strln = 0; value[strln] && (precision < 0 || strln < (size_t)precision); strln++);

	if ((padlen = width - strln) < 0)
		padlen = 0;

	/* Left justify. */
	if (flags & FL_LEFT)
		padlen = -padlen;

	/* Leading spaces. */
	if (padlen > 0)
	{
		do
		{
			OUTCHAR(dest, end, ' ');
		} while (--padlen);
	}

	while (*value && (precision < 0 || precision-- > 0))
	{
		OUTCHAR(dest, end, *value);
		value++;
	}

	/* Trailing spaces. */
	if (padlen < 0)
	{
		do
		{
			OUTCHAR(dest, end, ' ');
		} while (++padlen);
	}

	return dest;
}

inline size_t intcvt(uintmax_t value, char *buffer, size_t count, unsigned char base, int caps)
{
	char *dest, *end;

	dest = buffer;
	end = buffer + count;

	/* We return an unterminated buffer with the digits in reverse order. */
	if (base == 10)
	{
		uint32_t ui32, quot;

		if ((ui32 = (uint32_t)value) == value)
		{
			do
			{
				quot = uldiv10(ui32);
				*(dest++) = (char)(ui32 - quot * 10) + '0';
			} while ((ui32 = quot) && dest < end);
		}
		else
		{
			do
			{
				*(dest++) = (char)(value % 10) + '0';
			} while ((value /= 10) && dest < end);
		}
	}
	else if (base == 16)
	{
		const char *digits;

		digits = caps ? digitsLarge : digitsSmall;
		do
		{
			*(dest++) = digits[value & (16 - 1)];
		} while ((value >>= 4) && dest < end);
	}
	else //if (base == 8)
	{
		do
		{
			*(dest++) = ((char)value & (8 - 1)) + '0';
		} while ((value >>= 3) && dest < end);
	}
	return dest - buffer;
}

#define PRINTSEP(dest, end) \
	OUTCHAR(dest, end, ',')

#define GETNUMSEP(digits) \
	((digits % 3 ? digits : digits - 1) / 3)

static char *intfmt(char *dest, const char *end, intmax_t value, unsigned char base, size_t width, ptrdiff_t precision, int flags)
{
	uintmax_t     uvalue;
	char          icvtbuf[UINTMAX_LENGTH];
	char          sign;
	char          hexprefix;
	ptrdiff_t     spadlen;	/* Amount to space pad. */
	ptrdiff_t     zpadlen;	/* Amount to zero pad. */
	size_t        pos;
	size_t        separators;
	unsigned char noprecision;

	sign = '\0';
	if (flags & FL_UNSIGNED)
	{
		uvalue = value;
	}
	else
	{
		uvalue = (value >= 0) ? value : -value;
		if (value < 0)
			sign = '-';
		else if (flags & FL_SIGN)
			/* Do a sign. */
			sign = '+';
		else if (flags & FL_SIGNSP)
			sign = ' ';
	}

	pos = intcvt(uvalue, icvtbuf, _countof(icvtbuf), base, flags & FL_UP);

	hexprefix = '\0';
	noprecision = (precision < 0);
	if (flags & FL_ALTERNATE && uvalue)
	{
		/*
		 * C99 says: "The result is converted to an `alternative form'.
		 * For `o' conversion, it increases the precision, if and only
		 * if necessary, to force the first digit of the result to be a
		 * zero (if the value and precision are both 0, a single 0 is
		 * printed).  For `x' (or `X') conversion, a nonzero result has
		 * `0x' (or `0X') prefixed to it." (7.19.6.1, 6)
		 */
		switch (base)
		{
		case 8:
			if (precision <= (ptrdiff_t)pos)
				precision = pos + 1;
			break;
		case 16:
			hexprefix = (flags & FL_UP) ? 'X' : 'x';
			break;
		}
	}

	/* Get the number of group separators we'll print. */
	separators = (flags & FL_QUOTE) ? GETNUMSEP(pos) : 0;

	zpadlen = precision - pos - separators;
	spadlen =
		width                               /* Minimum field width. */
		- separators                        /* Number of separators. */
		- max(precision, (ptrdiff_t)pos)    /* Number of integer digits. */
		- (sign ? 1 : 0)                    /* Will we print a sign? */
		- (hexprefix ? 2 : 0);              /* Will we print a prefix? */

	if (zpadlen < 0)
		zpadlen = 0;
	if (spadlen < 0)
		spadlen = 0;

	/*
	 * C99 says: "If the `0' and `-' flags both appear, the `0' flag is
	 * ignored.  For `d', `i', `o', `u', `x', and `X' conversions, if a
	 * precision is specified, the `0' flag is ignored." (7.19.6.1, 6)
	 */
	if (flags & FL_LEFT)
	{
		/* Left justify. */
		spadlen = -spadlen;
	}
	else if ((flags & FL_LEADZERO) && noprecision)
	{
		zpadlen += spadlen;
		spadlen = 0;
	}

	/* Leading spaces. */
	if (spadlen > 0)
	{
		do
		{
			OUTCHAR(dest, end, ' ');
		} while (--spadlen);
	}

	/* Sign. */
	if (sign)
		OUTCHAR(dest, end, sign);

	/* A "0x" or "0X" prefix. */
	if (hexprefix)
	{
		OUTCHAR(dest, end, '0');
		OUTCHAR(dest, end, hexprefix);
	}

	/* Leading zeros. */
	if (zpadlen)
	{
		do
		{
			OUTCHAR(dest, end, '0');
		} while (--zpadlen);
	}

	/* The actual digits. */
	if (pos)
	{
		pos--;
		OUTCHAR(dest, end, icvtbuf[pos]);
		while (pos)
		{
			if (separators && !(pos % 3))
				PRINTSEP(dest, end);
			pos--;
			OUTCHAR(dest, end, icvtbuf[pos]);
		}
	}

	/* Trailing spaces. */
	if (spadlen < 0)
	{
		do
		{
			OUTCHAR(dest, end, ' ');
		} while (++spadlen);
	}

	return dest;
}

#define ECVTBUF(value, ndigits, decpt, cvtbuf) \
	fltcvt(value, ndigits, decpt, cvtbuf, 1)

#define FCVTBUF(value, ndigits, decpt, cvtbuf) \
	fltcvt(value, ndigits, decpt, cvtbuf, 0)

static size_t fltcvt(long_double value, size_t ndigits, ptrdiff_t *decpt, char *cvtbuf, ptrdiff_t eflag)
{
	ptrdiff_t   r2;
	long_double intpart, fracpart;
	char        *p1, *p2;

#ifdef _DEBUG
	assert(!signbitl(value));
	assert(!isnanl(value));
	assert(!isinfl(value));
	assert((ptrdiff_t)ndigits >= 0);
#endif

	value = modfl(value, &intpart);
	r2 = 0;
	p1 = cvtbuf;
	if (intpart != 0)
	{
		char c1, c2;

		while (intpart)
		{
			fracpart = modfl(intpart / 10, &intpart);
			*(p1++) = (char)((fracpart + .03) * 10) + '0';
		}
		r2 = (p1--) - cvtbuf;
		p2 = cvtbuf;
		while (p1 > p2)
		{
			c1 = *p1;
			c2 = *p2;
			*(p1--) = c2;
			*(p2++) = c1;
		}
		p1 = cvtbuf + r2;
	}
	else if (value)
	{
		while ((fracpart = value * 10) < 1)
		{
			value = fracpart;
			r2--;
		}
	}
	p2 = cvtbuf + ndigits;
	if (!eflag)
		p2 += r2;
	if (p2 >= cvtbuf)
	{
		char *p3;

		p3 = min(p2, cvtbuf + CVTBUFSIZE - 1);
		while (p1 <= p3)
		{
			value *= 10;
			value = modfl(value, &fracpart);
			*(p1++) = (char)fracpart + '0';
		}
		if (p2 < cvtbuf + CVTBUFSIZE)
		{
			p1 = p2;
			*p2 += 5;
			while (*p2 > '9')
			{
				*p2 = '0';
				if (p2 > cvtbuf)
				{
					++(*(--p2));
				}
				else
				{
					*p2 = '1';
					r2++;
					if (!eflag)
					{
						if (p1 > cvtbuf)
							*p1 = '0';
						p1++;
					}
				}
			}
		}
		else
		{
			p1 = cvtbuf + (r2 = CVTBUFSIZE - 1);
		}
	}
	else
	{
		r2 = 0;
		p1 = cvtbuf;
	}
	*decpt = r2;
	return p1 - cvtbuf;
}

inline size_t fltacvt(long_double value, size_t precision, char *cvtbuf, size_t *elen, char *ecvtbuf, int flags)
{
	uintmax_t     mantissa;
	int32_t       exponent;
	uint32_t      quotient;
	size_t        i;
	char          *p1, *p2, *p3;
	char          c1, c2;
	unsigned char diff;

#ifdef _DEBUG
	assert(!signbitl(value));
	assert(!isnanl(value));
	assert(!isinfl(value));
	assert((ptrdiff_t)precision >= 0);
#endif

	mantissa = ((UNION_LONGDOUBLE *)&value)->mantissa;
	exponent = (int32_t)((UNION_LONGDOUBLE *)&value)->exponent - LDBL_EXP_BIAS;
	p1 = cvtbuf + 1;
	if (precision)
	{
		if (precision > MANTISSA_HEX_LENGTH)
			precision = MANTISSA_HEX_LENGTH;
		*(p1++) = '.';
	}
	if (i = MANTISSA_HEX_LENGTH - precision)
	{
		mantissa >>= i * 4 - 4;
		mantissa += 7;
		mantissa >>= 4;
	}
	diff = (flags & FL_UP) ? 0 : 'a' - 'A';
	p2 = p1 + precision;
	do
	{
		*(--p2) = (unsigned char)mantissa & 0x0F;
		*p2 += (unsigned char)*p2 < 0x0A ? '0' : 'A' - 0x0A + diff;
		mantissa >>= 4;
	} while (p2 != p1);
	*cvtbuf = ((unsigned char)mantissa + 1) & 0x0F;
	*cvtbuf += (unsigned char)*cvtbuf < 0x0A ? '0' : 'A' - 0x0A + diff;
	p1 += precision;
	p2 = ecvtbuf;
	*(p2++) = 'P' + diff;
	if (exponent >= 0)
	{
		*(p2++) = '+';
	}
	else
	{
		exponent = -exponent;
		*(p2++) = '-';
	}
	p3 = p2;
	do
	{
		quotient = uldiv10(exponent);
		*(p3++) = (char)(exponent - quotient * 10) + '0';
	} while (exponent = quotient);
	*elen = (p3--) - ecvtbuf;
	while (p2 < p3)
	{
		c1 = *p2;
		c2 = *p3;
		*(p2++) = c2;
		*(p3--) = c1;
	}
	return p1 - cvtbuf;
}

static char *fltfmt(char *dest, const char *end, long_double value, size_t width, ptrdiff_t precision, int flags)
{
	char       cvtbuf[ALIGN(CVTBUFSIZE, 16)];
	char       ecvtbuf[ALIGN(EXPBUFSIZE, 16)];	/* "e-12" (without nul-termination). */
	char       sign;
	char       hexprefix;
	size_t     cvtlen;
	ptrdiff_t  decpt;
	size_t     ilen;
	size_t     flen;
	size_t     elen;
	size_t     tailfraczeros;
	size_t     separators;
	size_t     emitpoint;
	ptrdiff_t  padlen;
	char       *p;
	const char *infnan;
	char       c;

	// Left align means no zero padding
	if (flags & FL_LEFT)
		flags &= ~FL_LEADZERO;

	// Determine padding and sign char
	if (signbitl(value))
	{
		value = -value;
		sign = '-';
	}
	else if (flags & (FL_SIGN | FL_SIGNSP))
	{
		sign = (flags & FL_SIGN) ? '+' : ' ';
	}
	else
	{
		sign = '\0';
	}

	if (isnanl(value))
		goto NaN;
	if (isinfl(value))
		goto INF;

	// Compute the precision value
	if (precision < 0)
		// Default precision: 6
		precision = !(flags & FL_TYPE_A) ? 6 : MANTISSA_HEX_LENGTH;
	else if (!precision && (flags & FL_TYPE_G))
		// ANSI specified
		precision = 1;

	// Convert floating point number to text
	if (!(flags & FL_TYPE_A))
	{
		if (flags & FL_TYPE_G)
		{
			ECVTBUF(value, precision, &decpt, cvtbuf);
			if (decpt <= -4 || decpt > precision)
			{
				flags |= FL_TYPE_E;
				precision -= 1;
			}
			else
			{
				precision -= decpt;
			}
		}
		elen = 0;
		if (flags & FL_TYPE_E)
		{
			int32_t  exponent;
			uint32_t quotient;
			char     *p1, *p2;
			char     c1, c2;

			cvtlen = ECVTBUF(value, precision + 1, &decpt, cvtbuf);

			exponent = !decpt ? !value ? 0 : -1 : decpt - 1;
			decpt = 1;

			ecvtbuf[elen++] = (flags & FL_UP) ? 'E' : 'e';
			if (exponent >= 0)
			{
				ecvtbuf[elen++] = '+';
			}
			else
			{
				exponent = -exponent;
				ecvtbuf[elen++] = '-';
			}
			do
			{
				quotient = uldiv10(exponent);
				ecvtbuf[elen++] = (char)(exponent - quotient * 10) + '0';
			} while (exponent = quotient);

			/*
			 * C99 says: "The exponent always contains at least two digits,
			 * and only as many more digits as necessary to represent the
			 * exponent." (7.19.6.1, 8)
			 */
			if (elen == 3)
				ecvtbuf[elen++] = '0';

			p1 = ecvtbuf + 2;
			p2 = ecvtbuf + elen - 1;
			while (p1 < p2)
			{
				c1 = *p1;
				c2 = *p2;
				*(p1++) = c2;
				*(p2--) = c1;
			}
		}
		else
		{
			cvtlen = FCVTBUF(value, precision, &decpt, cvtbuf);
		}
		if ((flags & FL_TYPE_G) && !(flags & FL_ALTERNATE))
		{
			char *end;

			p = end = cvtbuf + cvtlen;
			while (*(--p) == '0' && p != cvtbuf);
			if (++p != end)
			{
				ptrdiff_t diff;

				diff = end - p;
				precision -= min(diff, precision);
				cvtlen -= diff;
			}
		}
		ilen = max(decpt, 1);
		flen = cvtlen - min(decpt, (ptrdiff_t)cvtlen);
		tailfraczeros = max(precision - (ptrdiff_t)flen, 0);
		hexprefix = '\0';
	}
	else
	{
		cvtlen = fltacvt(value, precision, cvtbuf, &elen, ecvtbuf, flags);
		ilen = decpt = cvtlen;
		tailfraczeros = (size_t)precision + 2 <= cvtlen ? 0 : precision + 2 - cvtlen;
		flen = precision = 0;
		hexprefix = (flags & FL_UP) ? 'X' : 'x';
	}

	/*
	 * Print a decimal point if either the fractional part is non-zero
	 * and/or the "#" flag was specified.
	 */
	emitpoint = !(flags & FL_TYPE_A) && (precision || (flags & (FL_ALTERNATE)));

	/* Get the number of group separators we'll print. */
	separators = !(flags & FL_TYPE_A) && (flags & FL_QUOTE) ? GETNUMSEP(ilen) : 0;

	padlen =
		width                   /* Minimum field width. */
		- ilen                  /* Number of integer digits. */
		- elen                  /* Number of exponent characters. */
		- precision             /* Number of fractional digits. */
		- separators            /* Number of group separators. */
		- emitpoint             /* Will we print a decimal point? */
		- (sign ? 1 : 0)        /* Will we print a sign character? */
		- (hexprefix ? 2 : 0);  /* Will we print a prefix? */

	if (padlen < 0)
		padlen = 0;

	/*
	 * C99 says: "If the `0' and `-' flags both appear, the `0' flag is
	 * ignored." (7.19.6.1, 6)
	 */
	if (flags & FL_LEFT)
	{
		/* Left justifty. */
		padlen = -padlen;
	}
	else if ((flags & FL_LEADZERO) && padlen)
	{
		/* Sign. */
		if (sign)
		{
			OUTCHAR(dest, end, sign);
			sign = '\0';
		}

		/* A "0x" or "0X" prefix. */
		if (hexprefix)
		{
			OUTCHAR(dest, end, '0');
			OUTCHAR(dest, end, hexprefix);
			hexprefix = '\0';
		}

		/* Leading zeros. */
		do
		{
			OUTCHAR(dest, end, '0');
		} while (--padlen);
	}

	/* Leading spaces. */
	if (padlen > 0)
	{
		do
		{
			OUTCHAR(dest, end, ' ');
		} while (--padlen);
	}

	/* Sign. */
	if (sign)
		OUTCHAR(dest, end, sign);

	/* A "0x" or "0X" prefix. */
	if (hexprefix)
	{
		OUTCHAR(dest, end, '0');
		OUTCHAR(dest, end, hexprefix);
	}

	/* Integer part. */
	if (decpt > 0)
	{
		OUTCHAR(dest, end, *cvtbuf);
		for (size_t i = 1; i < (size_t)decpt; i++)
		{
			if (separators && !(((size_t)decpt - i) % 3))
				PRINTSEP(dest, end);
			OUTCHAR(dest, end, cvtbuf[i]);
		}
	}
	else
	{
		OUTCHAR(dest, end, '0');
	}

	/* Decimal point. */
	if (emitpoint)
		OUTCHAR(dest, end, '.');

	/* The remaining fractional part. */
	if (decpt < 0)
	{
		do
		{
			OUTCHAR(dest, end, '0');
		} while (++decpt);
	}
	for (size_t i = decpt; i < cvtlen; i++)
		OUTCHAR(dest, end, cvtbuf[i]);

	/* Following fractional part zeros. */
	if (tailfraczeros)
	{
		do
		{
			OUTCHAR(dest, end, '0');
		} while (--tailfraczeros);
	}

	/* Exponent. */
	for (size_t i = 0; i < elen; i++)
		OUTCHAR(dest, end, ecvtbuf[i]);

	/* Trailing spaces. */
	if (padlen < 0)
	{
		do
		{
			OUTCHAR(dest, end, ' ');
		} while (++padlen);
	}

	return dest;

NaN:
#ifndef _MSC_VER
	infnan = lpcszNan;
#else
	infnan = value == NAN ? lpcszNan : lpcszNanInd;
#endif
	goto INF_NaN;

INF:
	infnan = lpcszInf;

INF_NaN:
	p = cvtbuf;
	if (sign)
		*(p++) = sign;
	if (flags & FL_UP)
		while (c = *(infnan++))
			*(p++) = (c >= 'a' && c <= 'z') ? c - ('a' - 'A') : c;
	else
		while (c = *(infnan++))
			*(p++) = c;
	return strfmt(dest, end, cvtbuf, width, p - cvtbuf, flags);
}
