#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <winternl.h>
#if _MSC_VER > 1400
#include <intrin.h>
#endif
#endif

#include <inttypes.h>
#include <math.h>
#include <float.h>

#ifdef _MSC_VER
#define LONGDOUBLE_IS_DOUBLE 1
#pragma function(pow)
#ifdef isnan
#undef isnan
#endif
#define isnan _isnan
#ifdef isfinite
#undef isfinite
#endif
#define isfinite _finite
#ifdef signbit
#undef signbit
#endif
#define signbit(x) (*(__int64 *)&(x) < 0)
#ifdef isleadbyte
#undef isleadbyte
#endif
#define isleadbyte IsDBCSLeadByte
#endif

#if LONGDOUBLE_IS_DOUBLE
typedef double long_double;
#define powl pow
#define modfl modf
#define isnanl isnan
#define isfinitel isfinite
#define signbitl signbit
#else
typedef long double long_double;
#endif

#if !defined(CVTBUFSIZE) && defined(_CVTBUFSIZE)
#define CVTBUFSIZE _CVTBUFSIZE
#endif

#if defined(_MSC_VER) && _MSC_VER > 1400 && (defined(_M_IX86) || defined(_M_X64))
#define uldiv10(value) \
	(unsigned long int)(__emulu(value, 0xCCCCCCCDUL) >> 35)
#else
#define uldiv10(value) \
	(unsigned long int)(((unsigned long long)(unsigned long int)(value) * 0xCCCCCCCDUL) >> 35)
#endif

/*
 * Buffer size to hold the octal string representation of UINT128_MAX without
 * nul-termination ("3777777777777777777777777777777777777777777").
 */
#ifdef INT128_MAX
#define MAX_INTEGER_LENGTH      43
#elif defined(INT64_MAX)
#define MAX_INTEGER_LENGTH      22
#else
#define MAX_INTEGER_LENGTH      11
#endif

/* Format read states. */
#define PRINT_S_DEFAULT         0
#define PRINT_S_FLAGS           1
#define PRINT_S_WIDTH           2
#define PRINT_S_DOT             3
#define PRINT_S_PRECISION       4
#define PRINT_S_MOD             5
#define PRINT_S_CONV            6

/* Format flags. */
#define PRINT_F_MINUS           (1 << 0)
#define PRINT_F_PLUS            (1 << 1)
#define PRINT_F_SPACE           (1 << 2)
#define PRINT_F_NUM             (1 << 3)
#define PRINT_F_ZERO            (1 << 4)
#define PRINT_F_QUOTE           (1 << 5)
#define PRINT_F_UP              (1 << 6)
#define PRINT_F_UNSIGNED        (1 << 7)
#define PRINT_F_TYPE_G          (1 << 8)
#define PRINT_F_TYPE_E          (1 << 9)

/* Conversion flags. */
#define PRINT_C_DEFAULT         0
#define PRINT_C_CHAR            1
#define PRINT_C_SHORT           2
#define PRINT_C_LONG            3
#define PRINT_C_LLONG           4
#define PRINT_C_LDOUBLE         5
#define PRINT_C_SIZE            6
#define PRINT_C_PTRDIFF         7
#define PRINT_C_INTMAX          8
#ifdef _MSC_VER
#define PRINT_C_INT             9
#define PRINT_C_INT32           10
#define PRINT_C_INT64           11
#define PRINT_C_WCHAR           12
#endif

#ifndef CHARTOINT
#define CHARTOINT(ch) ((ch) - '0')
#endif

#ifndef ISDIGIT
#define ISDIGIT(ch) ((ch) >= '0' && (ch) <= '9')
#endif

#define OUTCHAR(dest, end, ch) \
    if (++(dest) < (end))      \
        *(dest) = (ch)

static char *fmtstr(char *, const char *, const char *, size_t, ptrdiff_t, int);
static char *fmtint(char *, const char *, intmax_t, size_t, size_t, ptrdiff_t, int);
static char *fmtflt(char *, const char *, long_double, size_t, ptrdiff_t, int);

int __cdecl _vsnprintf(char *buffer, size_t count, const char *format, va_list argptr)
{
	char            *dest;
	const char      *end;
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
	int             overflow;
	size_t          base;
	int             cflags;
	int             flags;
	size_t          width;
	ptrdiff_t       precision;
	int             state;
	char            ch;

	overflow = 0;
	base = 0;
	cflags = 0;
	flags = 0;
	width = 0;
	precision = -1;
	state = PRINT_S_DEFAULT;
	ch = *(format++);

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
	while (ch)
	{
		switch (state)
		{
		case PRINT_S_DEFAULT:
			if (ch == '%')
			{
				state = PRINT_S_FLAGS;
			}
			else
			{
				if (isleadbyte(ch))
				{
					OUTCHAR(dest, end, ch);
					if (!(ch = *(format++)))
						goto NESTED_BREAK;
				}
				OUTCHAR(dest, end, ch);
			}
			ch = *(format++);
			break;
		case PRINT_S_FLAGS:
			switch (ch)
			{
			case '-':
				flags |= PRINT_F_MINUS;
				ch = *(format++);
				break;
			case '+':
				flags |= PRINT_F_PLUS;
				ch = *(format++);
				break;
			case ' ':
				flags |= PRINT_F_SPACE;
				ch = *(format++);
				break;
			case '#':
				flags |= PRINT_F_NUM;
				ch = *(format++);
				break;
			case '0':
				flags |= PRINT_F_ZERO;
				ch = *(format++);
				break;
			case '\'':
				/* SUSv2 flag (not in C99). */
				flags |= PRINT_F_QUOTE;
				ch = *(format++);
				break;
			default:
				state = PRINT_S_WIDTH;
				break;
			}
			break;
		case PRINT_S_WIDTH:
			if (ISDIGIT(ch))
			{
				ch = CHARTOINT(ch);
				if (width >= (size_t)(INT_MAX / 10) && (width > (size_t)(INT_MAX / 10) || (unsigned char)ch >= (unsigned char)(INT_MAX - (INT_MAX / 10) * 10)))
				{
					overflow = 1;
					goto NESTED_BREAK;
				}
				width = 10 * width + ch;
				ch = *(format++);
			}
			else if (ch == '*')
			{
				/*
				 * C99 says: "A negative field width argument is
				 * taken as a `-' flag followed by a positive
				 * field width." (7.19.6.1, 5)
				 */
				if ((ptrdiff_t)(width = (ptrdiff_t)va_arg(argptr, int)) < 0)
				{
					flags |= PRINT_F_MINUS;
					width = -(ptrdiff_t)width;
				}
				ch = *(format++);
				state = PRINT_S_DOT;
			}
			else
				state = PRINT_S_DOT;
			break;
		case PRINT_S_DOT:
			if (ch == '.')
			{
				state = PRINT_S_PRECISION;
				ch = *(format++);
			}
			else
			{
				state = PRINT_S_MOD;
			}
			break;
		case PRINT_S_PRECISION:
			if (precision < 0)
				precision = 0;
			if (ISDIGIT(ch))
			{
				ch = CHARTOINT(ch);
				if ((size_t)precision >= (size_t)(INT_MAX / 10) && ((size_t)precision > (size_t)(INT_MAX / 10) || (unsigned char)ch >= (unsigned char)(INT_MAX - (INT_MAX / 10) * 10)))
				{
					overflow = 1;
					goto NESTED_BREAK;
				}
				precision = 10 * precision + ch;
				ch = *(format++);
			}
			else if (ch == '*')
			{
				/*
				 * C99 says: "A negative precision argument is
				 * taken as if the precision were omitted."
				 * (7.19.6.1, 5)
				 */
				if ((precision = va_arg(argptr, int)) < 0)
					precision = -1;
				ch = *(format++);
				state = PRINT_S_MOD;
			}
			else
				state = PRINT_S_MOD;
			break;
		case PRINT_S_MOD:
			switch (ch)
			{
			case 'h':
				ch = *(format++);
				if (ch == 'h')
				{
					/* It's a char. */
					ch = *(format++);
					cflags = PRINT_C_CHAR;
				}
				else
					cflags = PRINT_C_SHORT;
				break;
			case 'l':
				ch = *(format++);
				if (ch == 'l')
				{
					/* It's a long long. */
					ch = *(format++);
					cflags = PRINT_C_LLONG;
				}
				else
					cflags = PRINT_C_LONG;
				break;
#ifdef _MSC_VER
			case 'I':
				ch = *(format++);
				if (ch == '6')
				{
					if (*format == '4')
					{
						format++;
						ch = *(format++);
						cflags = PRINT_C_INT64;
						break;
					}
				}
				else if (ch == '3')
				{
					if (*format == '2')
					{
						format++;
						ch = *(format++);
						cflags = PRINT_C_INT32;
						break;
					}
				}
				cflags = PRINT_C_INT;
				break;
#endif
			case 'L':
				cflags = PRINT_C_LDOUBLE;
				ch = *(format++);
				break;
			case 'j':
				cflags = PRINT_C_INTMAX;
				ch = *(format++);
				break;
			case 't':
				cflags = PRINT_C_PTRDIFF;
				ch = *(format++);
				break;
			case 'z':
				cflags = PRINT_C_SIZE;
				ch = *(format++);
				break;
#ifdef _MSC_VER
			case 'w':
				cflags = PRINT_C_WCHAR;
				ch = *(format++);
				break;
#endif
			}
			state = PRINT_S_CONV;
			break;
		case PRINT_S_CONV:
			switch (ch)
			{
			case 'd':
				/* FALLTHROUGH */
			case 'i':
				switch (cflags)
				{
				case PRINT_C_CHAR:
					value = (char)va_arg(argptr, int);
					break;
				case PRINT_C_SHORT:
					value = (short int)va_arg(argptr, int);
					break;
				case PRINT_C_LONG:
#ifdef _MSC_VER
				case PRINT_C_INT32:
#endif
					value = va_arg(argptr, long int);
					break;
				case PRINT_C_LLONG:
#ifdef _MSC_VER
				case PRINT_C_INT64:
#endif
					value = va_arg(argptr, long long int);
					break;
				case PRINT_C_SIZE:
#ifdef _MSC_VER
				case PRINT_C_INT:
#endif
					value = va_arg(argptr, size_t);
					break;
				case PRINT_C_INTMAX:
					value = va_arg(argptr, intmax_t);
					break;
				case PRINT_C_PTRDIFF:
					value = va_arg(argptr, ptrdiff_t);
					break;
				default:
					value = va_arg(argptr, int);
					break;
				}
				dest = fmtint(dest, end, value, 10, width, precision, flags);
				break;
			case 'X':
				flags |= PRINT_F_UP;
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
				flags |= PRINT_F_UNSIGNED;
				switch (cflags)
				{
				case PRINT_C_CHAR:
					value = (unsigned char)va_arg(argptr, unsigned int);
					break;
				case PRINT_C_SHORT:
					value = (unsigned short int)va_arg(argptr, unsigned int);
					break;
				case PRINT_C_LONG:
#ifdef _MSC_VER
				case PRINT_C_INT32:
#endif
					value = va_arg(argptr, unsigned long int);
					break;
				case PRINT_C_LLONG:
#ifdef _MSC_VER
				case PRINT_C_INT64:
#endif
					value = va_arg(argptr, unsigned long long int);
					break;
				case PRINT_C_SIZE:
#ifdef _MSC_VER
				case PRINT_C_INT:
#endif
					value = va_arg(argptr, size_t);
					break;
				case PRINT_C_INTMAX:
					value = va_arg(argptr, uintmax_t);
					break;
				case PRINT_C_PTRDIFF:
					value = va_arg(argptr, ptrdiff_t);
					break;
				default:
					value = va_arg(argptr, unsigned int);
					break;
				}
				dest = fmtint(dest, end, value, base, width, precision, flags);
				break;
			case 'A':
				/* Not yet supported, we'll use "%F". */
				/* FALLTHROUGH */
			case 'F':
				flags |= PRINT_F_UP;
			case 'a':
				/* Not yet supported, we'll use "%f". */
				/* FALLTHROUGH */
			case 'f':
#if !LONGDOUBLE_IS_DOUBLE
				if (cflags == PRINT_C_LDOUBLE)
					fvalue = va_arg(argptr, long_double);
				else
#endif
					fvalue = va_arg(argptr, double);
				dest = fmtflt(dest, end, fvalue, width, precision, flags);
				if (overflow)
					goto NESTED_BREAK;
				break;
			case 'E':
				flags |= PRINT_F_UP;
				/* FALLTHROUGH */
			case 'e':
				flags |= PRINT_F_TYPE_E;
#if !LONGDOUBLE_IS_DOUBLE
				if (cflags == PRINT_C_LDOUBLE)
					fvalue = va_arg(argptr, long_double);
				else
#endif
					fvalue = va_arg(argptr, double);
				dest = fmtflt(dest, end, fvalue, width, precision, flags);
				break;
			case 'G':
				flags |= PRINT_F_UP;
				/* FALLTHROUGH */
			case 'g':
				flags |= PRINT_F_TYPE_G;
#if !LONGDOUBLE_IS_DOUBLE
				if (cflags == PRINT_C_LDOUBLE)
					fvalue = va_arg(argptr, long_double);
				else
#endif
					fvalue = va_arg(argptr, double);
				/*
				 * If the precision is zero, it is treated as
				 * one (cf. C99: 7.19.6.1, 8).
				 */
				if (precision == 0)
					precision = 1;
				dest = fmtflt(dest, end, fvalue, width, precision, flags);
				break;
			case 'c':
#ifdef _MSC_VER
				if (cflags == PRINT_C_LONG || cflags == PRINT_C_WCHAR)
					goto PRINT_WCHAR;
			PRINT_CHAR:
#endif
				cvalue = va_arg(argptr, int);
				OUTCHAR(dest, end, cvalue);
				break;
#ifdef _MSC_VER
			case 'C':
				if (cflags == PRINT_C_SHORT || cflags == PRINT_C_CHAR)
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
				if (cflags == PRINT_C_LONG || cflags == PRINT_C_WCHAR)
					goto PRINT_WSTR;
			PRINT_STR:
#endif
				strvalue = va_arg(argptr, char *);
				dest = fmtstr(dest, end, strvalue, width, precision, flags);
				break;
#ifdef _MSC_VER
			case 'S':
				if (cflags == PRINT_C_SHORT || cflags == PRINT_C_CHAR)
					goto PRINT_STR;
			PRINT_WSTR:
				strvalue = NULL;
				ivalue = 0;
				wstrvalue = va_arg(argptr, wchar_t *);
				if (wstrvalue)
					if (ivalue = WideCharToMultiByte(CP_ACP, 0, wstrvalue, -1, NULL, 0, NULL, NULL))
						if (strvalue = (char *)HeapAlloc(handle = GetProcessHeap(), 0, ++ivalue * sizeof(char)))
							ivalue = WideCharToMultiByte(CP_ACP, 0, wstrvalue, -1, strvalue, ivalue, NULL, NULL);
				dest = fmtstr(dest, end, ivalue ? strvalue : NULL, width, precision, flags);
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
					dest = fmtstr(dest, end, "(nil)", width, -1, flags);
				}
				else
				{
					/*
					 * We use the BSD/glibc format.  SysV
					 * omits the "0x" prefix (which we emit
					 * using the PRINT_F_NUM flag).
					 */
					flags |= PRINT_F_UNSIGNED | PRINT_F_UP;
					dest = fmtint(dest, end, (uintptr_t)strvalue, 16, width, sizeof(void *) * 2, flags);
				}
				break;
			case 'n':
				switch (cflags)
				{
				case PRINT_C_CHAR:
					charptr = va_arg(argptr, char *);
					*charptr = (char)(dest + 1 - buffer);
					break;
				case PRINT_C_SHORT:
					shortptr = va_arg(argptr, short int *);
					*shortptr = (short int)(dest + 1 - buffer);
					break;
				case PRINT_C_LONG:
#ifdef _MSC_VER
				case PRINT_C_INT32:
#endif
					longptr = va_arg(argptr, long int *);
					*longptr = (long int)(dest + 1 - buffer);
					break;
				case PRINT_C_LLONG:
#ifdef _MSC_VER
				case PRINT_C_INT64:
#endif
					llongptr = va_arg(argptr, long long int *);
					*llongptr = (long long int)(dest + 1 - buffer);
					break;
				case PRINT_C_SIZE:
#ifdef _MSC_VER
				case PRINT_C_INT:
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
				case PRINT_C_INTMAX:
					intmaxptr = va_arg(argptr, intmax_t *);
					*intmaxptr = dest + 1 - buffer;
					break;
				case PRINT_C_PTRDIFF:
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
				case PRINT_C_LONG:
				case PRINT_C_WCHAR:
					strvalue = NULL;
					ivalue = 0;
					us = va_arg(argptr, PUNICODE_STRING);
					if (us && us->Buffer)
						if (ivalue = WideCharToMultiByte(CP_ACP, 0, us->Buffer, -1, NULL, 0, NULL, NULL))
							if (strvalue = (char *)HeapAlloc(handle = GetProcessHeap(), 0, ++ivalue * sizeof(char)))
								ivalue = WideCharToMultiByte(CP_ACP, 0, us->Buffer, -1, strvalue, ivalue, NULL, NULL);
					dest = fmtstr(dest, end, ivalue ? strvalue : NULL, width, precision, flags);
					if (strvalue)
						HeapFree(handle, 0, strvalue);
					break;
				default:
					as = va_arg(argptr, PANSI_STRING);
					dest = fmtstr(dest, end, as ? as->Buffer : NULL, width, precision, flags);
					break;
				}
				break;
#endif
			case '%':
				/* Print a "%" character verbatim. */
				OUTCHAR(dest, end, ch);
				break;
			default:
				/* Skip other characters. */
				if (isleadbyte(ch))
					if (!*(format++))
						goto NESTED_BREAK;
				break;
			}
			ch = *format++;
			state = PRINT_S_DEFAULT;
			base = 0;
			cflags = 0;
			flags = 0;
			width = 0;
			precision = -1;
			break;
		}
	}
NESTED_BREAK:
	if (++dest < end)
		*dest = '\0';
	else if (count)
		*(char *)(end - 1) = '\0';

	if (overflow || (int)(dest - buffer) >= INT_MAX)
	{
		errno = overflow ? EOVERFLOW : ERANGE;
		return -1;
	}
	return dest - buffer;
}

#define PRINTSEP(dest, end) \
	OUTCHAR(dest, end, ',')

#define GETNUMSEP(digits) \
	((digits % 3 ? digits : digits - 1) / 3)

static size_t icvt(uintmax_t value, char *buffer, size_t count, size_t base, int caps)
{
	char *dest, *end;

	dest = buffer;
	end = buffer + count;

	/* We return an unterminated buffer with the digits in reverse order. */
	if (base == 10)
	{
		unsigned long int ul, quot;

		if ((ul = (unsigned long int)value) == value)
		{
			do
			{
				quot = uldiv10(ul);
				*(dest++) = (char)(ul - quot * 10 + '0');
			} while ((ul = quot) && dest < end);
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
		const char *digitsLarge = "0123456789ABCDEF";
		const char *digitsSmall = "0123456789abcdef";
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

static char *fmtstr(char *dest, const char *end, const char *value, size_t width, ptrdiff_t precision, int flags)
{
	ptrdiff_t     padlen;	/* Amount to pad. */
	size_t        strln;
	unsigned char noprecision;

	if (value == NULL)
		/* We're forgiving. */
		value = "(null)";

	/* If a precision was specified, don't read the string past it. */
	noprecision = (precision < 0);
	for (strln = 0; value[strln] && (noprecision || (ptrdiff_t)strln < precision); strln++);

	if ((padlen = width - strln) < 0)
		padlen = 0;
	if (flags & PRINT_F_MINUS)
		/* Left justify. */
		padlen = -padlen;

	while (padlen > 0)
	{
		/* Leading spaces. */
		OUTCHAR(dest, end, ' ');
		padlen--;
	}
	while (*value && (noprecision || precision-- > 0))
	{
		OUTCHAR(dest, end, *value);
		value++;
	}
	while (padlen < 0)
	{
		/* Trailing spaces. */
		OUTCHAR(dest, end, ' ');
		padlen++;
	}
	return dest;
}

static char *fmtint(char *dest, const char *end, intmax_t value, size_t base, size_t width, ptrdiff_t precision, int flags)
{
	uintmax_t     uvalue;
	char          icvtbuf[MAX_INTEGER_LENGTH];
	char          sign;
	char          hexprefix;
	ptrdiff_t     spadlen;	/* Amount to space pad. */
	ptrdiff_t     zpadlen;	/* Amount to zero pad. */
	size_t        pos;
	size_t        separators;
	unsigned char noprecision;

	sign = '\0';
	if (flags & PRINT_F_UNSIGNED)
	{
		uvalue = value;
	}
	else
	{
		uvalue = (value >= 0) ? value : -value;
		if (value < 0)
			sign = '-';
		else if (flags & PRINT_F_PLUS)
			/* Do a sign. */
			sign = '+';
		else if (flags & PRINT_F_SPACE)
			sign = ' ';
	}

	pos = icvt(uvalue, icvtbuf, _countof(icvtbuf), base, flags & PRINT_F_UP);

	hexprefix = '\0';
	noprecision = (precision < 0);
	if (flags & PRINT_F_NUM && uvalue != 0)
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
			hexprefix = (flags & PRINT_F_UP) ? 'X' : 'x';
			break;
		}
	}

	/* Get the number of group separators we'll print. */
	separators = (flags & PRINT_F_QUOTE) ? GETNUMSEP(pos) : 0;

	zpadlen = precision - pos - separators;
	spadlen = width                         /* Minimum field width. */
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
	if (flags & PRINT_F_MINUS)
	{
		/* Left justify. */
		spadlen = -spadlen;
	}
	else if (flags & PRINT_F_ZERO && noprecision)
	{
		zpadlen += spadlen;
		spadlen = 0;
	}
	while (spadlen > 0)
	{
		/* Leading spaces. */
		OUTCHAR(dest, end, ' ');
		spadlen--;
	}
	if (sign != 0)
	{
		/* Sign. */
		OUTCHAR(dest, end, sign);
	}
	if (hexprefix != 0)
	{
		/* A "0x" or "0X" prefix. */
		OUTCHAR(dest, end, '0');
		OUTCHAR(dest, end, hexprefix);
	}
	while (zpadlen > 0)
	{
		/* Leading zeros. */
		OUTCHAR(dest, end, '0');
		zpadlen--;
	}
	while (pos)
	{
		/* The actual digits. */
		pos--;
		OUTCHAR(dest, end, icvtbuf[pos]);
		if (separators > 0 && pos > 0 && pos % 3 == 0)
			PRINTSEP(dest, end);
	}
	while (spadlen < 0)
	{
		/* Trailing spaces. */
		OUTCHAR(dest, end, ' ');
		spadlen++;
	}
	return dest;
}

static char *fmtflt(char *dest, const char *end, long_double fvalue, size_t width, ptrdiff_t precision, int flags)
{
	long_double intpart;
	long_double fracpart;
	const char  *infnan;
	char        cvtbuf[CVTBUFSIZE + (CVTBUFSIZE - 1) / 3];
	char        *fcvtbuf;
	char        ecvtbuf[4];	/* "e-12" (without nul-termination). */
	char        sign;
	size_t      tailfraczeros;
	ptrdiff_t   exponent;
	size_t      emitpoint;
	ptrdiff_t   padlen;
	size_t      epos;
	size_t      fpos;
	size_t      ipos;
	size_t      separators;
	int         estyle;

	/*
	 * AIX' man page says the default is 0, but C99 and at least Solaris'
	 * and NetBSD's man pages say the default is 6, and sprintf(3) on AIX
	 * defaults to 6.
	 */
	if (precision < 0)
		precision = 6;
	else if (!precision && (flags & PRINT_F_TYPE_G))
		precision = 1;
	else if (precision > _countof(cvtbuf) - 1)
		precision = _countof(cvtbuf) - 1;

	if (signbitl(fvalue))
	{
		sign = '-';
		fvalue = -fvalue;
	}
	else if (flags & (PRINT_F_PLUS | PRINT_F_SPACE))
		sign = (flags & PRINT_F_PLUS) ? '+' : ' ';
	else
		sign = '\0';

	if (isnanl(fvalue))
#ifndef _MSC_VER
		infnan = (flags & PRINT_F_UP) ? "NAN" : "nan";
#else
		infnan = fvalue == NAN ?
			(flags & PRINT_F_UP) ? "NAN" : "nan" :
			(flags & PRINT_F_UP) ? "NAN(IND)" : "nan(ind)";
#endif
	else if (!isfinitel(fvalue))
		infnan = (flags & PRINT_F_UP) ? "INF" : "inf";
	else
		infnan = NULL;

	if (infnan)
	{
		char *p;

		p = cvtbuf;
		if (sign)
			*(p++) = sign;
		while (*infnan)
			*(p++) = *infnan++;
		return fmtstr(dest, end, cvtbuf, width, p - cvtbuf, flags);
	}

	/* "%e" (or "%E") or "%g" (or "%G") conversion. */
	exponent = 0;
	if (estyle = flags & (PRINT_F_TYPE_E | PRINT_F_TYPE_G))
	{
		if (flags & PRINT_F_TYPE_G)
		{
			/*
			 * For "%g" (and "%G") conversions, the precision
			 * specifies the number of significant digits, which
			 * includes the digits in the integer part.  The
			 * conversion will or will not be using "e-style" (like
			 * "%e" or "%E" conversions) depending on the precision
			 * and on the exponent.  However, the exponent can be
			 * affected by rounding the converted value, so we'll
			 * leave this decision for later.  Until then, we'll
			 * assume that we're going to do an "e-style" conversion
			 * (in order to get the exponent calculated).  For
			 * "e-style", the precision must be decremented by one.
			 */
			precision--;
		}
		if (fvalue)
		{
			long_double x;

			/*
			 * We check for 99 > exponent > -99 in order to work around possible
			 * endless loops which could happen (at least) in the second loop (at
			 * least) if we're called with an infinite value.  However, we checked
			 * for infinity before calling this function using our ISINF() macro, so
			 * this might be somewhat paranoid.
			 */
			x = fvalue;
			while (x < 1 && x > 0 && --exponent > -99)
				x *= 10;
			while (x >= 10 && ++exponent < 99)
				x /= 10;
			x = powl(10, precision - exponent);
			modfl(x * fvalue + .5, &fvalue);
			fvalue /= x;
			x = fvalue;
			exponent = 0;
			while (x < 1 && x > 0 && --exponent > -99)
				x *= 10;
			while (x >= 10 && ++exponent < 99)
				x /= 10;
		}
	}
	fvalue += .5 * powl(.1, precision - exponent);

	if (estyle && exponent)
		/* We want exactly one integer digit. */
		fvalue /= powl(10, exponent);

	fracpart = modfl(fvalue, &intpart);

	if ((flags & PRINT_F_TYPE_G) && estyle && precision + 1 > exponent && exponent >= -4)
	{
		/*
		 * Now that we know the real exponent, we can check whether or not to
		 * use "e-style" for "%g" (and "%G") conversions.  If we don't need
		 * "e-style", the precision must be adjusted and the integer and
		 * fractional parts must be recalculated from the original value.
		 *
		 * C99 says: "Let P equal the precision if nonzero, 6 if the precision
		 * is omitted, or 1 if the precision is zero.  Then, if a conversion
		 * with style `E' would have an exponent of X:
		 *
		 * - if P > X >= -4, the conversion is with style `f' (or `F') and
		 *   precision P - (X + 1).
		 *
		 * - otherwise, the conversion is with style `e' (or `E') and precision
		 *   P - 1." (7.19.6.1, 8)
		 *
		 * Note that we had decremented the precision by one.
		 */
		precision -= exponent;
		fracpart = modfl(fvalue * powl(10, exponent), &intpart);
		estyle = 0;
	}

	epos = 0;
	if (estyle)
	{
		char esign;

		if (exponent < 0)
		{
			exponent = -exponent;
			esign = '-';
		}
		else
		{
			esign = '+';
		}

		/*
		 * Convert the exponent.  The _countof(ecvtbuf) is 4.  So, the
		 * ecvtbuf buffer can hold e.g. "e+99" and "e-99".  We don't
		 * support an exponent which contains more than two digits.
		 * Therefore, the following stores are safe.
		 */
		epos = icvt(exponent, ecvtbuf, 2, 10, 0);

		/*
		 * C99 says: "The exponent always contains at least two digits,
		 * and only as many more digits as necessary to represent the
		 * exponent." (7.19.6.1, 8)
		 */
		if (epos == 1)
			ecvtbuf[epos++] = '0';
		ecvtbuf[epos++] = esign;
		ecvtbuf[epos++] = (flags & PRINT_F_UP) ? 'E' : 'e';
	}

	/* Convert the integer part and the fractional part. */
	ipos = 0;
	if (intpart)
	{
		char *p, *end;

		p = end = cvtbuf + _countof(cvtbuf);
		do
		{
			*(--p) = (char)((modfl(intpart / 10, &intpart) + .03) * 10) + '0';
		} while (p != cvtbuf && intpart);
		if (p != cvtbuf)
		{
			fcvtbuf = cvtbuf;
			do
			{
				*(fcvtbuf++) = *(p++);
			} while (p != end);
		}
		else
		{
			fcvtbuf = end;
		}
		ipos = fcvtbuf - cvtbuf;
	}
	else
	{
		*cvtbuf = '0';
		ipos = 1;
		fcvtbuf = cvtbuf + 1;
	}

	/* fractional part */
	fpos = 0;
	if (precision && fracpart)
	{
		char *p, *end;

		p = fcvtbuf;
		end = (char *)min((size_t)(fcvtbuf + precision), (size_t)(cvtbuf + _countof(cvtbuf)));
		if (p != end)
		{
			do
			{
				fracpart = modfl(fracpart * 10, &intpart);
				*p = (char)intpart + '0';
			} while (++p != end && fracpart);
			while (*(--p) == '0' && p != fcvtbuf);
			p++;
		}
		fpos = p - fcvtbuf;
	}

	tailfraczeros = precision - fpos;

	/*
	 * Print a decimal point if either the fractional part is non-zero
	 * and/or the "#" flag was specified.
	 */
	emitpoint = (precision > 0 || flags & PRINT_F_NUM);

	/* Get the number of group separators we'll print. */
	separators = (flags & PRINT_F_QUOTE) ? GETNUMSEP(ipos) : 0;

	padlen = width          /* Minimum field width. */
	    - ipos              /* Number of integer digits. */
	    - epos              /* Number of exponent characters. */
	    - precision         /* Number of fractional digits. */
	    - separators        /* Number of group separators. */
	    - emitpoint         /* Will we print a decimal point? */
	    - (sign ? 1 : 0);   /* Will we print a sign character? */

	if (padlen < 0)
		padlen = 0;

	/*
	 * C99 says: "If the `0' and `-' flags both appear, the `0' flag is
	 * ignored." (7.19.6.1, 6)
	 */
	if (flags & PRINT_F_MINUS)
	{
		/* Left justifty. */
		padlen = -padlen;
	}
	else if ((flags & PRINT_F_ZERO) && padlen)
	{
		if (sign)
		{
			/* Sign. */
			OUTCHAR(dest, end, sign);
			sign = '\0';
		}
		while (padlen)
		{
			/* Leading zeros. */
			OUTCHAR(dest, end, '0');
			padlen--;
		}
	}
	while (padlen > 0)
	{
		/* Leading spaces. */
		OUTCHAR(dest, end, ' ');
		padlen--;
	}
	if (sign)
	{
		/* Sign. */
		OUTCHAR(dest, end, sign);
	}
	if (ipos)
	{
		/* Integer part. */
		OUTCHAR(dest, end, *cvtbuf);
		for (size_t i = 1; i < ipos; i++)
		{
			if (separators && (ipos - i) % 3 == 0)
				PRINTSEP(dest, end);
			OUTCHAR(dest, end, cvtbuf[i]);
		}
	}
	if (emitpoint)
	{
		/* Decimal point. */
		OUTCHAR(dest, end, '.');
	}
	for (size_t i = 0; i < fpos; i++)
	{
		/* The remaining fractional part. */
		OUTCHAR(dest, end, fcvtbuf[i]);
	}
	while (tailfraczeros)
	{
		/* Leading fractional part zeros. */
		OUTCHAR(dest, end, '0');
		tailfraczeros--;
	}
	while (epos > 0)
	{
		/* Exponent. */
		epos--;
		OUTCHAR(dest, end, ecvtbuf[epos]);
	}
	while (padlen < 0)
	{
		/* Trailing spaces. */
		OUTCHAR(dest, end, ' ');
		padlen++;
	}
	return dest;
}

