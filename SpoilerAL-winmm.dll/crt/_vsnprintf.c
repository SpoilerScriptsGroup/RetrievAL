#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <winternl.h>
#include <inttypes.h>
#include <math.h>
#include <float.h>

#ifdef _MSC_VER
#pragma function(log)
#pragma function(pow)
typedef double long_double;
#define logl log
#define powl pow
#define modfl modf
#define _isnanl _isnan
#define _finitel _finite
#endif

#ifndef M_LN10
#define M_LN10 2.30258509299404568401799145468436421
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
#define PRINT_C_INT             9
#define PRINT_C_INT32           10
#define PRINT_C_INT64           11
#define PRINT_C_WCHAR           12

#ifndef CHARTOINT
#define CHARTOINT(ch) (ch - '0')
#endif

#ifndef ISDIGIT
#define ISDIGIT(ch) (ch >= '0' && ch <= '9')
#endif

#define OUTCHAR(str, len, size, ch) \
do {                                \
    if (len + 1 < size)             \
        str[len] = ch;              \
    (len)++;                        \
} while (/* CONSTCOND */ 0)

static void fmtstr(char *, size_t *, size_t, const char *, int, int, int);
static void fmtint(char *, size_t *, size_t, intmax_t, int, int, int, int);
static void fmtflt(char *, size_t *, size_t, long_double, int, int, int, int *);

int __cdecl _vsnprintf(char *str, size_t size, const char *format, va_list args)
{
	long_double     fvalue;
	intmax_t        value;
	unsigned char   cvalue;
	long int        ivalue;
	wchar_t         wvalue;
	char            buffer[2];
	char            *strvalue;
	wchar_t         *wstrvalue;
	intmax_t        *intmaxptr;
	ptrdiff_t       *ptrdiffptr;
	size_t          *sizeptr;
	long long int   *llongptr;
	long int        *longptr;
	int             *intptr;
	short int       *shortptr;
	signed char     *charptr;
	PANSI_STRING    as;
	PUNICODE_STRING us;
	HANDLE          handle;
	size_t          len;
	int             overflow;
	int             base;
	int             cflags;
	int             flags;
	int             width;
	int             precision;
	int             state;
	char            ch;

	len = 0;
	overflow = 0;
	base = 0;
	cflags = 0;
	flags = 0;
	width = 0;
	precision = -1;
	state = PRINT_S_DEFAULT;
	ch = *format++;

	/*
	 * C99 says: "If `n' is zero, nothing is written, and `s' may be a null
	 * pointer." (7.19.6.5, 2)  We're forgiving and allow a NULL pointer
	 * even if a size larger than zero was specified.  At least NetBSD's
	 * snprintf(3) does the same, as well as other versions of this file.
	 * (Though some of these versions will write to a non-NULL buffer even
	 * if a size of zero was specified, which violates the standard.)
	 */
	if (str == NULL)
		size = 0;

	while (ch != '\0')
		switch (state) {
		case PRINT_S_DEFAULT:
			if (ch == '%')
				state = PRINT_S_FLAGS;
			else {
				if (IsDBCSLeadByte(ch)) {
					OUTCHAR(str, len, size, ch);
					if ((ch = *format++) == '\0')
						goto out;
				}
				OUTCHAR(str, len, size, ch);
			}
			ch = *format++;
			break;
		case PRINT_S_FLAGS:
			switch (ch) {
			case '-':
				flags |= PRINT_F_MINUS;
				ch = *format++;
				break;
			case '+':
				flags |= PRINT_F_PLUS;
				ch = *format++;
				break;
			case ' ':
				flags |= PRINT_F_SPACE;
				ch = *format++;
				break;
			case '#':
				flags |= PRINT_F_NUM;
				ch = *format++;
				break;
			case '0':
				flags |= PRINT_F_ZERO;
				ch = *format++;
				break;
			case '\'':	/* SUSv2 flag (not in C99). */
				flags |= PRINT_F_QUOTE;
				ch = *format++;
				break;
			default:
				state = PRINT_S_WIDTH;
				break;
			}
			break;
		case PRINT_S_WIDTH:
			if (ISDIGIT(ch)) {
				ch = CHARTOINT(ch);
				if (width > (INT_MAX - ch) / 10) {
					overflow = 1;
					goto out;
				}
				width = 10 * width + ch;
				ch = *format++;
			}
			else if (ch == '*') {
				/*
				 * C99 says: "A negative field width argument is
				 * taken as a `-' flag followed by a positive
				 * field width." (7.19.6.1, 5)
				 */
				if ((width = va_arg(args, int)) < 0) {
					flags |= PRINT_F_MINUS;
					width = -width;
				}
				ch = *format++;
				state = PRINT_S_DOT;
			}
			else
				state = PRINT_S_DOT;
			break;
		case PRINT_S_DOT:
			if (ch == '.') {
				state = PRINT_S_PRECISION;
				ch = *format++;
			}
			else
				state = PRINT_S_MOD;
			break;
		case PRINT_S_PRECISION:
			if (precision == -1)
				precision = 0;
			if (ISDIGIT(ch)) {
				ch = CHARTOINT(ch);
				if (precision > (INT_MAX - ch) / 10) {
					overflow = 1;
					goto out;
				}
				precision = 10 * precision + ch;
				ch = *format++;
			}
			else if (ch == '*') {
				/*
				 * C99 says: "A negative precision argument is
				 * taken as if the precision were omitted."
				 * (7.19.6.1, 5)
				 */
				if ((precision = va_arg(args, int)) < 0)
					precision = -1;
				ch = *format++;
				state = PRINT_S_MOD;
			}
			else
				state = PRINT_S_MOD;
			break;
		case PRINT_S_MOD:
			switch (ch) {
			case 'h':
				ch = *format++;
				if (ch == 'h') {	/* It's a char. */
					ch = *format++;
					cflags = PRINT_C_CHAR;
				}
				else
					cflags = PRINT_C_SHORT;
				break;
			case 'l':
				ch = *format++;
				if (ch == 'l') {	/* It's a long long. */
					ch = *format++;
					cflags = PRINT_C_LLONG;
				}
				else
					cflags = PRINT_C_LONG;
				break;
			case 'I':
				ch = *format++;
				if (ch == '6' && format[1] == '4') {
					ch = *(format += 2);
					cflags = PRINT_C_INT64;
				}
				else if (ch == '3' && format[1] == '2') {
					ch = *(format += 2);
					cflags = PRINT_C_INT32;
				}
				else
					cflags = PRINT_C_INT;
				break;
			case 'L':
				cflags = PRINT_C_LDOUBLE;
				ch = *format++;
				break;
			case 'j':
				cflags = PRINT_C_INTMAX;
				ch = *format++;
				break;
			case 't':
				cflags = PRINT_C_PTRDIFF;
				ch = *format++;
				break;
			case 'z':
				cflags = PRINT_C_SIZE;
				ch = *format++;
				break;
			case 'w':
				cflags = PRINT_C_WCHAR;
				ch = *format++;
				break;
			}
			state = PRINT_S_CONV;
			break;
		case PRINT_S_CONV:
			switch (ch) {
			case 'd':
				/* FALLTHROUGH */
			case 'i':
				switch (cflags) {
				case PRINT_C_CHAR:
					value = (signed char)va_arg(args, int);
					break;
				case PRINT_C_SHORT:
					value = (short int)va_arg(args, int);
					break;
				case PRINT_C_LONG:
				case PRINT_C_INT32:
					value = va_arg(args, long int);
					break;
				case PRINT_C_LLONG:
				case PRINT_C_INT64:
					value = va_arg(args, long long int);
					break;
				case PRINT_C_SIZE:
				case PRINT_C_INT:
					value = va_arg(args, size_t);
					break;
				case PRINT_C_INTMAX:
					value = va_arg(args, intmax_t);
					break;
				case PRINT_C_PTRDIFF:
					value = va_arg(args, ptrdiff_t);
					break;
				default:
					value = va_arg(args, int);
					break;
				}
				fmtint(str, &len, size, value, 10, width,
					precision, flags);
				break;
			case 'X':
				flags |= PRINT_F_UP;
				/* FALLTHROUGH */
			case 'x':
				base = 16;
				/* FALLTHROUGH */
			case 'o':
				if (base == 0)
					base = 8;
				/* FALLTHROUGH */
			case 'u':
				if (base == 0)
					base = 10;
				flags |= PRINT_F_UNSIGNED;
				switch (cflags) {
				case PRINT_C_CHAR:
					value = (unsigned char)va_arg(args,
						unsigned int);
					break;
				case PRINT_C_SHORT:
					value = (unsigned short int)va_arg(args,
						unsigned int);
					break;
				case PRINT_C_LONG:
				case PRINT_C_INT32:
					value = va_arg(args, unsigned long int);
					break;
				case PRINT_C_LLONG:
				case PRINT_C_INT64:
					value = va_arg(args, unsigned long long int);
					break;
				case PRINT_C_SIZE:
				case PRINT_C_INT:
					value = va_arg(args, size_t);
					break;
				case PRINT_C_INTMAX:
					value = va_arg(args, uintmax_t);
					break;
				case PRINT_C_PTRDIFF:
					value = va_arg(args, ptrdiff_t);
					break;
				default:
					value = va_arg(args, unsigned int);
					break;
				}
				fmtint(str, &len, size, value, base, width,
					precision, flags);
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
				if (cflags == PRINT_C_LDOUBLE)
					fvalue = va_arg(args, long_double);
				else
					fvalue = va_arg(args, double);
				fmtflt(str, &len, size, fvalue, width,
					precision, flags, &overflow);
				if (overflow)
					goto out;
				break;
			case 'E':
				flags |= PRINT_F_UP;
				/* FALLTHROUGH */
			case 'e':
				flags |= PRINT_F_TYPE_E;
				if (cflags == PRINT_C_LDOUBLE)
					fvalue = va_arg(args, long_double);
				else
					fvalue = va_arg(args, double);
				fmtflt(str, &len, size, fvalue, width,
					precision, flags, &overflow);
				if (overflow)
					goto out;
				break;
			case 'G':
				flags |= PRINT_F_UP;
				/* FALLTHROUGH */
			case 'g':
				flags |= PRINT_F_TYPE_G;
				if (cflags == PRINT_C_LDOUBLE)
					fvalue = va_arg(args, long_double);
				else
					fvalue = va_arg(args, double);
				/*
				 * If the precision is zero, it is treated as
				 * one (cf. C99: 7.19.6.1, 8).
				 */
				if (precision == 0)
					precision = 1;
				fmtflt(str, &len, size, fvalue, width,
					precision, flags, &overflow);
				if (overflow)
					goto out;
				break;
			case 'c':
				if (cflags == PRINT_C_LONG || cflags == PRINT_C_WCHAR)
					goto PRINT_WCHAR;
			PRINT_CHAR:
				cvalue = va_arg(args, int);
				OUTCHAR(str, len, size, cvalue);
				break;
			case 'C':
				if (cflags == PRINT_C_SHORT || cflags == PRINT_C_CHAR)
					goto PRINT_CHAR;
			PRINT_WCHAR:
				wvalue = va_arg(args, int);
				ivalue = WideCharToMultiByte(CP_ACP, 0, &wvalue, 1, buffer, 2, NULL, NULL);
				if (ivalue <= 0)
					break;
				OUTCHAR(str, len, size, buffer[0]);
				if (!--ivalue)
					break;
				OUTCHAR(str, len, size, buffer[1]);
				break;
			case 's':
				if (cflags == PRINT_C_LONG || cflags == PRINT_C_WCHAR)
					goto PRINT_WSTR;
			PRINT_STR:
				strvalue = va_arg(args, char *);
				fmtstr(str, &len, size, strvalue, width,
					precision, flags);
				break;
			case 'S':
				if (cflags == PRINT_C_SHORT || cflags == PRINT_C_CHAR)
					goto PRINT_STR;
			PRINT_WSTR:
				strvalue = NULL;
				ivalue = 0;
				wstrvalue = va_arg(args, wchar_t *);
				if (wstrvalue)
					if (ivalue = WideCharToMultiByte(CP_ACP, 0, wstrvalue, -1, NULL, 0, NULL, NULL))
						if (strvalue = (char *)HeapAlloc(handle = GetProcessHeap(), 0, ++ivalue * sizeof(char)))
							ivalue = WideCharToMultiByte(CP_ACP, 0, wstrvalue, -1, strvalue, ivalue, NULL, NULL);
				fmtstr(str, &len, size, ivalue ? strvalue : NULL, width, precision, flags);
				if (strvalue)
					HeapFree(handle, 0, strvalue);
				break;
			case 'p':
				/*
				 * C99 says: "The value of the pointer is
				 * converted to a sequence of printing
				 * characters, in an implementation-defined
				 * manner." (C99: 7.19.6.1, 8)
				 */
				if ((strvalue = va_arg(args, void *)) == NULL)
					/*
					 * We use the glibc format.  BSD prints
					 * "0x0", SysV "0".
					 */
					fmtstr(str, &len, size, "(nil)", width,
						-1, flags);
				else {
					/*
					 * We use the BSD/glibc format.  SysV
					 * omits the "0x" prefix (which we emit
					 * using the PRINT_F_NUM flag).
					 */
					flags |= PRINT_F_UNSIGNED;
					flags |= PRINT_F_UP;
					fmtint(str, &len, size,
						(uintptr_t)strvalue, 16, width,
						sizeof(void *) * 2, flags);
				}
				break;
			case 'n':
				switch (cflags) {
				case PRINT_C_CHAR:
					charptr = va_arg(args, signed char *);
					*charptr = (char)len;
					break;
				case PRINT_C_SHORT:
					shortptr = va_arg(args, short int *);
					*shortptr = (short)len;
					break;
				case PRINT_C_LONG:
				case PRINT_C_INT32:
					longptr = va_arg(args, long int *);
					*longptr = len;
					break;
				case PRINT_C_LLONG:
				case PRINT_C_INT64:
					llongptr = va_arg(args, long long int *);
					*llongptr = len;
					break;
				case PRINT_C_SIZE:
				case PRINT_C_INT:
					/*
					 * C99 says that with the "z" length
					 * modifier, "a following `n' conversion
					 * specifier applies to a pointer to a
					 * signed integer type corresponding to
					 * size_t argument." (7.19.6.1, 7)
					 */
					sizeptr = va_arg(args, size_t *);
					*sizeptr = len;
					break;
				case PRINT_C_INTMAX:
					intmaxptr = va_arg(args, intmax_t *);
					*intmaxptr = len;
					break;
				case PRINT_C_PTRDIFF:
					ptrdiffptr = va_arg(args, ptrdiff_t *);
					*ptrdiffptr = len;
					break;
				default:
					intptr = va_arg(args, int *);
					*intptr = len;
					break;
				}
				break;
			case 'Z':
				switch (cflags) {
				case PRINT_C_LONG:
				case PRINT_C_WCHAR:
					strvalue = NULL;
					ivalue = 0;
					us = va_arg(args, PUNICODE_STRING);
					if (us && us->Buffer)
						if (ivalue = WideCharToMultiByte(CP_ACP, 0, us->Buffer, -1, NULL, 0, NULL, NULL))
							if (strvalue = (char *)HeapAlloc(handle = GetProcessHeap(), 0, ++ivalue * sizeof(char)))
								ivalue = WideCharToMultiByte(CP_ACP, 0, us->Buffer, -1, strvalue, ivalue, NULL, NULL);
					fmtstr(str, &len, size, ivalue ? strvalue : NULL, width, precision, flags);
					if (strvalue)
						HeapFree(handle, 0, strvalue);
					break;
				default:
					as = va_arg(args, PANSI_STRING);
					fmtstr(str, &len, size, as ? as->Buffer : NULL, width, precision, flags);
					break;
				}
				break;
			case '%':	/* Print a "%" character verbatim. */
				OUTCHAR(str, len, size, ch);
				break;
			default:	/* Skip other characters. */
				if (IsDBCSLeadByte(ch))
					if (*format++ == '\0')
						goto out;
				break;
			}
			ch = *format++;
			state = PRINT_S_DEFAULT;
			base = cflags = flags = width = 0;
			precision = -1;
			break;
		}
out:
	if (len < size)
		str[len] = '\0';
	else if (size > 0)
		str[size - 1] = '\0';

	if (overflow || len >= INT_MAX) {
		errno = overflow ? EOVERFLOW : ERANGE;
		return -1;
	}
	return (int)len;
}

#define PRINTSEP(str, len, size) \
	OUTCHAR(str, len, size, ',')

#define GETNUMSEP(digits) \
	((digits % 3 ? digits : digits - 1) / 3)

static const char *digitsLarge = "0123456789ABCDEF";
static const char *digitsSmall = "0123456789abcdef";

static int icvt(uintmax_t value, char *buf, size_t size, int base, int caps)
{
	const char *digits;
	size_t     pos;

	digits = caps ? digitsLarge : digitsSmall;
	pos = 0;

	/* We return an unterminated buffer with the digits in reverse order. */
	do {
		buf[pos++] = digits[value % base];
		value /= base;
	} while (value != 0 && pos < size);

	return (int)pos;
}

static void fmtstr(char *str, size_t *len, size_t size, const char *value, int width, int precision, int flags)
{
	int padlen, strln;	/* Amount to pad. */
	int noprecision;

	noprecision = (precision == -1);

	if (value == NULL)	/* We're forgiving. */
		value = "(null)";

	/* If a precision was specified, don't read the string past it. */
	for (strln = 0; value[strln] != '\0' &&
	    (noprecision || strln < precision); strln++)
		continue;

	if ((padlen = width - strln) < 0)
		padlen = 0;
	if (flags & PRINT_F_MINUS)	/* Left justify. */
		padlen = -padlen;

	while (padlen > 0) {	/* Leading spaces. */
		OUTCHAR(str, *len, size, ' ');
		padlen--;
	}
	while (*value != '\0' && (noprecision || precision-- > 0)) {
		OUTCHAR(str, *len, size, *value);
		value++;
	}
	while (padlen < 0) {	/* Trailing spaces. */
		OUTCHAR(str, *len, size, ' ');
		padlen++;
	}
}

static void fmtint(char *str, size_t *len, size_t size, intmax_t value, int base, int width, int precision, int flags)
{
	uintmax_t uvalue;
	char      cvtbuf[MAX_INTEGER_LENGTH];
	char      sign;
	char      hexprefix;
	int       spadlen;	/* Amount to space pad. */
	int       zpadlen;	/* Amount to zero pad. */
	int       pos;
	int       separators;
	int       noprecision;

	sign = 0;
	hexprefix = 0;
	spadlen = 0;
	zpadlen = 0;
	separators = (flags & PRINT_F_QUOTE);
	noprecision = (precision == -1);

	if (flags & PRINT_F_UNSIGNED)
		uvalue = value;
	else {
		uvalue = (value >= 0) ? value : -value;
		if (value < 0)
			sign = '-';
		else if (flags & PRINT_F_PLUS)	/* Do a sign. */
			sign = '+';
		else if (flags & PRINT_F_SPACE)
			sign = ' ';
	}

	pos = icvt(uvalue, cvtbuf, sizeof(cvtbuf), base,
	    flags & PRINT_F_UP);

	if (flags & PRINT_F_NUM && uvalue != 0) {
		/*
		 * C99 says: "The result is converted to an `alternative form'.
		 * For `o' conversion, it increases the precision, if and only
		 * if necessary, to force the first digit of the result to be a
		 * zero (if the value and precision are both 0, a single 0 is
		 * printed).  For `x' (or `X') conversion, a nonzero result has
		 * `0x' (or `0X') prefixed to it." (7.19.6.1, 6)
		 */
		switch (base) {
		case 8:
			if (precision <= pos)
				precision = pos + 1;
			break;
		case 16:
			hexprefix = (flags & PRINT_F_UP) ? 'X' : 'x';
			break;
		}
	}

	if (separators)	/* Get the number of group separators we'll print. */
		separators = GETNUMSEP(pos);

	zpadlen = precision - pos - separators;
	spadlen = width                         /* Minimum field width. */
	    - separators                        /* Number of separators. */
	    - max(precision, pos)               /* Number of integer digits. */
	    - ((sign != 0) ? 1 : 0)             /* Will we print a sign? */
	    - ((hexprefix != 0) ? 2 : 0);       /* Will we print a prefix? */

	if (zpadlen < 0)
		zpadlen = 0;
	if (spadlen < 0)
		spadlen = 0;

	/*
	 * C99 says: "If the `0' and `-' flags both appear, the `0' flag is
	 * ignored.  For `d', `i', `o', `u', `x', and `X' conversions, if a
	 * precision is specified, the `0' flag is ignored." (7.19.6.1, 6)
	 */
	if (flags & PRINT_F_MINUS)	/* Left justify. */
		spadlen = -spadlen;
	else if (flags & PRINT_F_ZERO && noprecision) {
		zpadlen += spadlen;
		spadlen = 0;
	}
	while (spadlen > 0) {	/* Leading spaces. */
		OUTCHAR(str, *len, size, ' ');
		spadlen--;
	}
	if (sign != 0)	/* Sign. */
		OUTCHAR(str, *len, size, sign);
	if (hexprefix != 0) {	/* A "0x" or "0X" prefix. */
		OUTCHAR(str, *len, size, '0');
		OUTCHAR(str, *len, size, hexprefix);
	}
	while (zpadlen > 0) {	/* Leading zeros. */
		OUTCHAR(str, *len, size, '0');
		zpadlen--;
	}
	while (pos > 0) {	/* The actual digits. */
		pos--;
		OUTCHAR(str, *len, size, cvtbuf[pos]);
		if (separators > 0 && pos > 0 && pos % 3 == 0)
			PRINTSEP(str, *len, size);
	}
	while (spadlen < 0) {	/* Trailing spaces. */
		OUTCHAR(str, *len, size, ' ');
		spadlen++;
	}
}

static void fmtflt(char *str, size_t *len, size_t size, long_double fvalue, int width, int precision, int flags, int *overflow)
{
	long_double intpart;
	long_double fracpart;
	const char  *infnan;
	char        cvtbuf[_CVTBUFSIZE];
	char        *fcvtbuf;
	char        ecvtbuf[4];	/* "e-12" (without nul-termination). */
	char        esign;
	char        sign;
	int         leadfraczeros;
	int         exponent;
	int         emitpoint;
	int         omitzeros;
	int         omitcount;
	int         padlen;
	int         epos;
	int         fpos;
	int         ipos;
	int         separators;
	int         estyle;

	infnan = NULL;
	esign = 0;
	sign = 0;
	leadfraczeros = 0;
	exponent = 0;
	emitpoint = 0;
	omitzeros = 0;
	omitcount = 0;
	padlen = 0;
	epos = 0;
	fpos = 0;
	ipos = 0;
	separators = (flags & PRINT_F_QUOTE);
	estyle = (flags & PRINT_F_TYPE_E);

	/*
	 * AIX' man page says the default is 0, but C99 and at least Solaris'
	 * and NetBSD's man pages say the default is 6, and sprintf(3) on AIX
	 * defaults to 6.
	 */
	if (precision == -1)
		precision = 6;

	if (fvalue < 0)
		sign = '-';
	else if (flags & PRINT_F_PLUS)	/* Do a sign. */
		sign = '+';
	else if (flags & PRINT_F_SPACE)
		sign = ' ';

	if (_isnanl(fvalue))
		infnan = (flags & PRINT_F_UP) ? "NAN" : "nan";
	else if (!_finitel(fvalue))
		infnan = (flags & PRINT_F_UP) ? "INF" : "inf";

	if (infnan != NULL) {
		if (sign != 0)
			cvtbuf[ipos++] = sign;
		while (*infnan != '\0')
			cvtbuf[ipos++] = *infnan++;
		fmtstr(str, len, size, cvtbuf, width, ipos, flags);
		return;
	}

	/* "%e" (or "%E") or "%g" (or "%G") conversion. */
	if (flags & PRINT_F_TYPE_E || flags & PRINT_F_TYPE_G) {
		if (flags & PRINT_F_TYPE_G) {
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
			/*
			 * For "%g" (and "%G") conversions, trailing zeros are
			 * removed from the fractional portion of the result
			 * unless the "#" flag was specified.
			 */
			if (!(flags & PRINT_F_NUM))
				omitzeros = 1;
		}
		exponent = fvalue ? (int)(logl(fvalue) / M_LN10) : 0;
		estyle = 1;
	}

again:

	if (fvalue < 0)
		fvalue = -fvalue;
	if (estyle)	/* We want exactly one integer digit. */
		fvalue /= powl(10, exponent);

	fracpart = modfl(fvalue, &intpart);

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
	if (flags & PRINT_F_TYPE_G && estyle && precision + 1 > exponent && exponent >= -4) {
		precision -= exponent;
		estyle = 0;
		goto again;
	}

	if (estyle) {
		if (exponent < 0) {
			exponent = -exponent;
			esign = '-';
		} else
			esign = '+';

		/*
		 * Convert the exponent.  The sizeof(ecvtbuf) is 4.  So, the
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
	if (intpart) {
		char        *p1, *p2;
		long_double fracpart;

		p1 = cvtbuf + sizeof(cvtbuf);
		do {
			fracpart = modfl(intpart / 10, &intpart);
			*(--p1) = (int)((fracpart + .03) * 10) + '0';
		} while (p1 != cvtbuf && intpart);
		p2 = cvtbuf;
		do {
			*(p2++) = *(p1++);
		} while (p1 < cvtbuf + sizeof(cvtbuf));
		ipos = p2 - cvtbuf;
	}
	else {
		*cvtbuf = '0';
		ipos = 1;
	}
	fcvtbuf = cvtbuf + ipos;
	if (precision && fracpart) {	/* fractional part */
		char *p, *end;

		p = fcvtbuf;
		end = min(fcvtbuf + precision, cvtbuf + sizeof(cvtbuf));
		do {
			fracpart *= 10;
			fracpart = modfl(fracpart, &intpart);
			*p = (int)intpart + '0';
		} while (++p < end && fracpart);
		fpos = p - fcvtbuf;
	}

	leadfraczeros = precision - fpos;

	if (omitzeros) {
		if (fpos > 0)	/* Omit trailing fractional part zeros. */
			while (omitcount < fpos && fcvtbuf[omitcount] == '0')
				omitcount++;
		else {	/* The fractional part is zero, omit it completely. */
			omitcount = precision;
			leadfraczeros = 0;
		}
		precision -= omitcount;
	}

	/*
	 * Print a decimal point if either the fractional part is non-zero
	 * and/or the "#" flag was specified.
	 */
	if (precision > 0 || flags & PRINT_F_NUM)
		emitpoint = 1;
	if (separators)	/* Get the number of group separators we'll print. */
		separators = GETNUMSEP(ipos);

	padlen = width                  /* Minimum field width. */
	    - ipos                      /* Number of integer digits. */
	    - epos                      /* Number of exponent characters. */
	    - precision                 /* Number of fractional digits. */
	    - separators                /* Number of group separators. */
	    - (emitpoint ? 1 : 0)       /* Will we print a decimal point? */
	    - ((sign != 0) ? 1 : 0);    /* Will we print a sign character? */

	if (padlen < 0)
		padlen = 0;

	/*
	 * C99 says: "If the `0' and `-' flags both appear, the `0' flag is
	 * ignored." (7.19.6.1, 6)
	 */
	if (flags & PRINT_F_MINUS)	/* Left justifty. */
		padlen = -padlen;
	else if (flags & PRINT_F_ZERO && padlen > 0) {
		if (sign != 0) {	/* Sign. */
			OUTCHAR(str, *len, size, sign);
			sign = 0;
		}
		while (padlen > 0) {	/* Leading zeros. */
			OUTCHAR(str, *len, size, '0');
			padlen--;
		}
	}
	while (padlen > 0) {	/* Leading spaces. */
		OUTCHAR(str, *len, size, ' ');
		padlen--;
	}
	if (sign != 0)	/* Sign. */
		OUTCHAR(str, *len, size, sign);
	if (ipos) {	/* Integer part. */
		OUTCHAR(str, *len, size, *cvtbuf);
		for (int i = 1; i < ipos; i++) {
			if (separators > 0 && (ipos - i) % 3 == 0)
				PRINTSEP(str, *len, size);
			OUTCHAR(str, *len, size, cvtbuf[i]);
		}
	}
	if (emitpoint)	/* Decimal point. */
		OUTCHAR(str, *len, size, '.');
	for (int i = 0; i < fpos - omitcount; i++)	/* The remaining fractional part. */
		OUTCHAR(str, *len, size, fcvtbuf[i]);
	while (leadfraczeros > 0) {	/* Leading fractional part zeros. */
		OUTCHAR(str, *len, size, '0');
		leadfraczeros--;
	}
	while (epos > 0) {	/* Exponent. */
		epos--;
		OUTCHAR(str, *len, size, ecvtbuf[epos]);
	}
	while (padlen < 0) {	/* Trailing spaces. */
		OUTCHAR(str, *len, size, ' ');
		padlen++;
	}
}

