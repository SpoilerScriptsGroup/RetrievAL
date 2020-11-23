#include "corecrt_internal.h"
#include "corecrt_internal_fltintrn.h"
#include "corecrt_internal_securecrt.h"
#if !defined(_MSC_VER) || _MSC_VER >= 1600
#include <stdbool.h>
#endif
#include <stdlib.h>
#ifndef _countof
#define _countof(_array) (sizeof(_array) / sizeof((_array)[0]))
#endif
#ifndef _CVTBUFSIZE
#define _CVTBUFSIZE (309 + 40)
#endif

#if defined(_MSC_VER) && _MSC_VER < 1600
typedef unsigned __int8  bool;
typedef unsigned __int16 uint16_t;
#endif

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Defines the _ecvt and _fcvt families of functions.
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

// An internal helper that wraps the call to convert the STRFLT to a string and
// updates all of the data that is used by its callers.
static errno_t internal_to_string(
	char         *buffer,
	const size_t buffer_count,
	_strflt      *strflt,
	const int    requested_digits,
	int          *decimal_point,
	int          *sign)
{
	size_t  minimum_buffer_count;
	int     capped_digits;
	errno_t e;

	// Make sure we don't overflow the buffer.  If the user asks for more digits
	// than the buffer can handle, truncate it to the maximum size allowed in
	// the buffer.  The maximum size is two less than the buffer size because we
	// use one character for overflow and one for the null terminator.
	minimum_buffer_count = (requested_digits > 0 ? requested_digits : 0) + 2;

	_VALIDATE_RETURN_ERRCODE(buffer_count >= minimum_buffer_count, ERANGE);

	capped_digits = __min(requested_digits, (int)(buffer_count - 2));

	e = __acrt_fp_strflt_to_string(buffer, buffer_count, capped_digits, strflt);

	if (e != 0)
		return errno = e;

	*sign = strflt->sign == '-' ? 1 : 0;
	*decimal_point = strflt->decpt;

	return 0;
}

// The _fcvt functions, like the _ecvt functions, convert a floating point value
// to a narrow character string.  The functions prepare the data for the Fortran
// F-format with the number of digits following the decimal point specified by
// requested_digits.  The position of the decimal point is returned indirectly
// through *decimal_point.  The correct digit for Fortran F-format is rounded.
//
// These functions update either (a) the user-provided string (_s-suffixed
// function) or (b) the per-thread conversion buffer.  The _s-suffixed
// function returns zero on success, or an error code on failure.  The
// *decimal_point and *sign values are updated with the results of the
// conversion.
#ifdef __cplusplus
extern "C"
#endif
errno_t __cdecl _fcvt_s(
	char   *buffer,
	size_t buffer_count,
	double value,
	int    requested_digits,
	int    *decimal_point,
	int    *sign)
{
	char    result_string[_CVTBUFSIZE + 1];
	_strflt strflt;
	int     actual_digits;
	bool    buffer_insufficiently_large;
	int     capped_digits;

	_VALIDATE_RETURN_ERRCODE(buffer != NULL, EINVAL);
	_VALIDATE_RETURN_ERRCODE(buffer_count > 0, EINVAL);
	_RESET_STRING(buffer, buffer_count);
	_VALIDATE_RETURN_ERRCODE(decimal_point != NULL, EINVAL);
	_VALIDATE_RETURN_ERRCODE(sign != NULL, EINVAL);

	__acrt_fltout(
		&value,
		_countof(result_string),
		&strflt,
		result_string,
		_countof(result_string));

	actual_digits = strflt.decpt + requested_digits;

	buffer_insufficiently_large =
		requested_digits > 0 && strflt.decpt > 0 &&
		actual_digits < requested_digits;

	capped_digits = buffer_insufficiently_large ? INT_MAX : actual_digits;

	return internal_to_string(buffer, buffer_count, &strflt, capped_digits, decimal_point, sign);
}

#ifdef __cplusplus
extern "C"
#endif
char *__cdecl _fcvt(
	double value,
	int    requested_digits,
	int    *decimal_point,
	int    *sign)
{
	static char buffer[_CVTBUFSIZE];

	char     result_string[_CVTBUFSIZE + 1];
	_strflt  strflt;
	int      capped_digits;
	errno_t  status;

	__acrt_fltout(
		&value,
		_countof(result_string),
		&strflt,
		result_string,
		_countof(result_string));

	// Make sure we don't overflow the buffer.  If the user asks for more digits
	// than the buffer can handle, truncate it to the maximum size allowed in
	// the buffer.  The maximum size is two less than the buffer size because we
	// use one character for overflow and one for the null terminator.
	capped_digits = __min(requested_digits, _CVTBUFSIZE - 2 - strflt.decpt);

	status = _fcvt_s(buffer, _CVTBUFSIZE, value, capped_digits, decimal_point, sign);
	if (status != 0)
		return NULL;

	return buffer;
}

// The _ecvt functions, which convert a floating point value to a string.  The
// position of the decimal point relative to the beginning of the string is
// stored indirectly through the decimal_point argument, where a negative value
// means that the decimal point is to the left of the returned digits.  If the
// sign of the result is negative, the word pointed to by sign is nonzero;
// otherwise it is zero.  The low order digit is rounded.
//
// These functions update either (a) the user-provided string (_s-suffixed
// function) or (b) the per-thread conversion buffer.  The _s-suffixed
// function returns zero on success, or an error code on failure.  The
// *decimal_point and *sign values are updated with the results of the
// conversion.
#ifdef __cplusplus
extern "C"
#endif
errno_t __cdecl _ecvt_s(
	char   *buffer,
	size_t buffer_count,
	double value,
	int    requested_digits,
	int    *decimal_point,
	int    *sign)
{
	char     result_string[_CVTBUFSIZE + 1];
	_strflt  strflt;
	errno_t  e;
	int      capped_digits;

	_VALIDATE_RETURN_ERRCODE(buffer != NULL, EINVAL);
	_VALIDATE_RETURN_ERRCODE(buffer_count > 0, EINVAL);
	_RESET_STRING(buffer, buffer_count);
	_VALIDATE_RETURN_ERRCODE(decimal_point != NULL, EINVAL);
	_VALIDATE_RETURN_ERRCODE(sign != NULL, EINVAL);

	__acrt_fltout(
		&value,
		_countof(result_string),
		&strflt,
		result_string,
		_countof(result_string));

	e = internal_to_string(buffer, buffer_count, &strflt, requested_digits, decimal_point, sign);

	// Make sure we don't overflow the buffer.  If the user asks for more digits
	// than the buffer can handle, truncate it to the maximum size allowed in
	// the buffer.  The maximum size is two less than the buffer size because we
	// use one character for overflow and one for the null terminator.
	capped_digits = __min(requested_digits, (int)(buffer_count - 2));

	// The conversion function occasionally returns an extra char in the buffer:
	if (capped_digits >= 0 && buffer[capped_digits])
		buffer[capped_digits] = '\0';

	return e;
}

#ifdef __cplusplus
extern "C"
#endif
char *__cdecl _ecvt(
	double value,
	int    requested_digits,
	int    *decimal_point,
	int    *sign)
{
	static char buffer[_CVTBUFSIZE];

	int     capped_digits;
	errno_t e;

	// Make sure we don't overflow the buffer.  If the user asks for more digits
	// than the buffer can handle, truncate it to the maximum size allowed in
	// the buffer.  The maximum size is two less than the buffer size because we
	// use one character for overflow and one for the null terminator.
	capped_digits = __min(requested_digits, _CVTBUFSIZE - 2);

	e = _ecvt_s(buffer, _CVTBUFSIZE, value, capped_digits, decimal_point, sign);
	if (e != 0)
		return NULL;

	return buffer;
}
