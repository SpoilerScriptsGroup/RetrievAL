#include "corecrt_internal.h"
#include "corecrt_internal_fltintrn.h"
#include "corecrt_internal_securecrt.h"
#include "corecrt_stdio_config.h"
#include <windows.h>
#include <stdlib.h>
#ifndef _CVTBUFSIZE
#define _CVTBUFSIZE (309 + 40)
#endif
#ifndef _countof
#define _countof(_array) (sizeof(_array) / sizeof((_array)[0]))
#endif

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Defines the _gcvt functions, which convert a floating point value to a narrow
// character string.  It attempts to produce 'precision' significant digits in
// the Fortran F format if possible, otherwise the E format.  Trailing zeroes may
// be suppressed.  The _s-suffixed function returns zero on success; an error
// code on failure.  If the buffer is too small, that is an error.
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#ifdef __cplusplus
extern "C"
#endif
errno_t __cdecl _gcvt_s(
	char         *buffer,
	const size_t buffer_count,
	const double value,
	const int    precision)
{
	#define restricted_count (size_t)7 // "1#SNAN" + 1 null terminator

	int     decimal_point_char;
	char    result_string[restricted_count];
	_strflt strflt;
	int     magnitude;
	char    *p;
	char    *stop;

	_VALIDATE_RETURN_ERRCODE(buffer != NULL, EINVAL);
	_VALIDATE_RETURN_ERRCODE(buffer_count > 0, EINVAL);
	_RESET_STRING(buffer, buffer_count);
	_VALIDATE_RETURN_ERRCODE((size_t)precision < buffer_count, ERANGE);
	// Additional validation will be performed in the fp_format functions.

	if (GetLocaleInfoA(GetThreadLocale(), LOCALE_SDECIMAL, (char *)&decimal_point_char, sizeof(decimal_point_char) / sizeof(char)))
		decimal_point_char = *(char *)&decimal_point_char;
	else
		decimal_point_char = '.';

	// We only call fltintrn_fltout in order to parse the correct exponent value (strflt.decpt).
	// Therefore, we don't want to generate any digits, so we pass a buffer size only large
	// enough to hold the inf, nan, or ind string to prevent failure.

	fltintrn_fltout(
		&value,
		precision,
		&strflt,
		result_string,
		restricted_count);

	magnitude = strflt.decpt - 1;

	// Output the result according to the Fortran G format as outlined in the
	// Fortran language specification.
	if (magnitude < -1 || magnitude > precision - 1)
	{
		char    scratch_buffer[_CVTBUFSIZE + 1];
		errno_t e;

		// Ew.d where d = precision
		e = fltintrn_fp_format(
			&value,
			buffer,
			buffer_count,
			scratch_buffer,
			_countof(scratch_buffer),
			'e',
			precision - 1,
			_CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS,
			decimal_point_char);

		if (e != 0)
			return errno = e;
	}
	else
	{
		char    scratch_buffer[_CVTBUFSIZE + 1];
		errno_t e;

		// Fw.d where d = precision-string->decpt
		e = fltintrn_fp_format(
			&value,
			buffer,
			buffer_count,
			scratch_buffer,
			_countof(scratch_buffer),
			'f',
			precision - strflt.decpt,
			_CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS,
			decimal_point_char);

		if (e != 0)
			return errno = e;
	}

	// Remove the trailing zeroes before the exponent; we don't need to check
	// for buffer_count:
	p = buffer;
	while (*p && *p != (char)decimal_point_char)
		++p;

	if (*p == '\0')
		return 0;

	++p;

	while (*p && *p != 'e')
		++p;

	stop = p;
	--p;

	while (*p == '0')
		--p;

	while ((*++p = *stop++) != '\0') {}

	return 0;

	#undef restricted_count
}

#ifdef __cplusplus
extern "C"
#endif
char *__cdecl _gcvt(
	double value,
	int    precision,
	char   *buffer)
{
	errno_t e;

	e = _gcvt_s(buffer, _CRT_UNBOUNDED_BUFFER_SIZE, value, precision);
	if (e != 0)
		return NULL;

	return buffer;
}
