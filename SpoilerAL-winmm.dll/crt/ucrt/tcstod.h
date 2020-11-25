//
// tcstod.h
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// Definitions of the functions that convert strings to floating point numbers.
// These functions, the strtox and wcstox functions for floating point types,
// recogize an optional sequence of tabs and space, then an optional sign, then
// a sequence of digits optionally containing a decimal point, then an optional
// e or E followed by an optionally signed integer, and converts all of this to
// a floating point number.  The first unrecognized character ends the string.
// The *end_ptr pointer is updated to point to the last character of the string
//
#include "corecrt_internal_strtox.h"

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Narrow Strings => Floating Point
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
double __cdecl _tcstod(
	const TCHAR *string,
	TCHAR       **end_ptr)
{
	double                    result;
	c_string_character_source source;
	SLD_STATUS                status;

	if (end_ptr)
		*end_ptr = (TCHAR *)string;

	_VALIDATE_RETURN(string != NULL, EINVAL, 0.0);

	result = 0;
	c_string_character_source_ctor(&source, string, (const TCHAR **)end_ptr);
	status = parse_floating_point(&source, &result);
	c_string_character_source_dtor(&source);

	if (status == SLD_OVERFLOW || status == SLD_UNDERFLOW)
		errno = ERANGE;

	return result;
}
