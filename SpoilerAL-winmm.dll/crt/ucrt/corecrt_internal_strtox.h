//
// corecrt_internal_strtox.h
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// This file defines the core implementations of the numeric parsers that parse
// integer or floating point numbers stored as strings.  These general parsers
// operate on abstract character sources, which allow the functions to be used
// to parse both random access and nonseekable ranges of characters (to support
// both strtod-style functions and fscanf-style functions).
//
#ifndef _CORECRT_INTERNAL_STRTOX_H_
#define _CORECRT_INTERNAL_STRTOX_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "corecrt_internal.h"
#include "corecrt_internal_fltintrn.h"
#include "big_integer.h"
#include <tchar.h>
#include <windows.h>
#include <float.h>
#include <errno.h>
#include <stdlib.h>
#ifndef _countof
#define _countof(_array) (sizeof(_array) / sizeof((_array)[0]))
#endif

#if defined(_MSC_VER) && _MSC_VER < 1600
typedef unsigned __int8  bool;
typedef unsigned __int8  uint8_t;
typedef __int32          int32_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#define false       0
#define true        1
#define UINT64_C(x) (x ## UI64)
#endif

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// String-to-Integer Conversion
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
unsigned int __fastcall strtox_parse_digit(const unsigned int c);

// The digit and nondigit categories include 0-9, a-z, A-Z, and _.  They are not
// locale-dependent, so we do not call isalnum (which uses the current locale to
// test for alphabetic characters).
bool __fastcall strtox_is_digit_or_nondigit(const unsigned int c);

__forceinline static bool is_space(const TCHAR c)
{
#ifndef _UNICODE
	return c == ' ' || (unsigned char)(c - '\t') < '\r' - '\t' + 1;
#else
	return c == ' ' || (unsigned short)(c - '\t') < '\r' - '\t' + 1;
#endif
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// String-to-Floating-Point Conversion
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#define SLD_OK        0
#define SLD_NODIGITS  0
#define SLD_UNDERFLOW 1
#define SLD_OVERFLOW  1

typedef int SLD_STATUS;

// This is the internal result type of an attempt to parse a floating point value
// from a string.  The SLD_STATUS type (above) is the type returned to callers of
// the top-level parse_floating_point.
#define FLOATING_POINT_PARSE_RESULT_DECIMAL_DIGITS      0
#define FLOATING_POINT_PARSE_RESULT_HEXADECIMAL_DIGITS  1
#define FLOATING_POINT_PARSE_RESULT_ZERO                2
#define FLOATING_POINT_PARSE_RESULT_INFINITY            3
#define FLOATING_POINT_PARSE_RESULT_QNAN                4
#define FLOATING_POINT_PARSE_RESULT_SNAN                5
#define FLOATING_POINT_PARSE_RESULT_INDETERMINATE       6
#define FLOATING_POINT_PARSE_RESULT_NO_DIGITS           7
#define FLOATING_POINT_PARSE_RESULT_UNDERFLOW           8
#define FLOATING_POINT_PARSE_RESULT_OVERFLOW            9

typedef int floating_point_parse_result;

#define MAXIMUM_TEMPORARY_DECIMAL_EXPONENT              5200
#define MINIMUM_TEMPORARY_DECIMAL_EXPONENT              -5200

// This type is used to hold a partially-parsed string representation of a
// floating point number.  The number is stored in the following form:
//
//     [sign] 0.mantissa * B^exponent
//
// The mantissa buffer stores the mantissa digits in big endian, binary coded
// decimal form.  The mantissa_count stores the number of digits present in the
// mantissa buffer.  The base B is not stored; it must be tracked separately.
// Note that the base of the mantissa digits may not be the same as B (e.g., for
// hexadecimal floating point hexadecimal, the mantissa digits are in base 16
// but the exponent is a base 2 exponent).
//
// We consider up to 768 decimal digits during conversion.  In most cases, we
// require nowhere near this many bits of precision to compute the correctly
// rounded binary floating point value for the input string.  768 bits gives us
// room to handle the exact decimal representation of the smallest denormal
// value, 2^-1074 (752 decimal digits after trimming zeroes) with a bit of slack
// space.
//
// NOTE:  The mantissa buffer count here must be kept in sync with the precision
// of the big_integer type, defined in <corecrt_internal_big_integer.h>.  See that file
// for details.
typedef struct {
	int32_t  exponent;
	uint32_t mantissa_count;
	uint8_t  mantissa[768];
	bool     is_negative;
} floating_point_string;

static floating_point_parse_result __fastcall parse_floating_point_possible_infinity(
	const TCHAR *p,
	const TCHAR **source)
{
	if ((p[0] | ('a' - 'A')) != 'n' ||
		(p[1] | ('a' - 'A')) != 'f')
	{
		return FLOATING_POINT_PARSE_RESULT_NO_DIGITS;
	}
	if ((p[2] | ('a' - 'A')) != 'i' ||
		(p[3] | ('a' - 'A')) != 'n' ||
		(p[4] | ('a' - 'A')) != 'i' ||
		(p[5] | ('a' - 'A')) != 't' ||
		(p[6] | ('a' - 'A')) != 'y')
	{
		*source = p + 2;
	}
	else
	{
		*source = p + 7;
	}
	return FLOATING_POINT_PARSE_RESULT_INFINITY;
}

static floating_point_parse_result __fastcall parse_floating_point_possible_nan(
	const TCHAR *p,
	const TCHAR **source)
{
	const TCHAR *stored_state;
	TCHAR       c;
	TCHAR       lowercase;

	if ((p[0] | ('a' - 'A')) != 'a' ||
		(p[1] | ('a' - 'A')) != 'n')
	{
		return FLOATING_POINT_PARSE_RESULT_NO_DIGITS;
	}

	stored_state = p += 2;
	c = *p++;

	if (c != '(')
	{
		*source = stored_state;
		return FLOATING_POINT_PARSE_RESULT_QNAN;
	}

	c = *p++; // Advance past the left parenthesis

	// After we've parsed a left parenthesis, test to see whether the parenthesized
	// string represents a signaling NaN "(SNAN)" or an indeterminate "(IND)".  If
	// so, we return the corresponding kind of NaN:
	if ((lowercase = (c | ('a' - 'A'))) == 's')
	{
		if (((c = *p++) | ('a' - 'A')) == 'n' &&
			((c = *p++) | ('a' - 'A')) == 'a' &&
			((c = *p++) | ('a' - 'A')) == 'n' &&
			 (c = *p++)                == ')')
		{
			*source = p - 1;
			return FLOATING_POINT_PARSE_RESULT_SNAN;
		}
	}
	else if (lowercase == 'i')
	{
		if (((c = *p++) | ('a' - 'A')) == 'n' &&
			((c = *p++) | ('a' - 'A')) == 'd' &&
			 (c = *p++)                == ')')
		{
			*source = p - 1;
			return FLOATING_POINT_PARSE_RESULT_INDETERMINATE;
		}
	}

	// Otherwise, we didn't match one of the two special parenthesized strings.
	// Keep eating chracters until we come across the right parenthesis or the
	// end of the character sequence:
	while (c != ')')
	{
		if (strtox_is_digit_or_nondigit(c))
		{
			c = *p++;
		}
		else
		{
			p = stored_state;
			break;
		}
	}

	*source = p;
	return FLOATING_POINT_PARSE_RESULT_QNAN;
}

__forceinline static floating_point_parse_result parse_floating_point_from_source(
	const TCHAR           **source,
	floating_point_string *fp_string)
{
	const TCHAR   *p;
	const TCHAR   *stored_state;
	TCHAR         c;
	TCHAR         lowercase;
	bool          is_hexadecimal;
	uint8_t       *mantissa_first;
	const uint8_t *mantissa_last;
	uint8_t       *mantissa_it;
	int           exponent_adjustment;
	bool          found_digits;
	unsigned int  max_digit_value;
	unsigned int  digit_value;
	int           radix_point;
	bool          has_exponent;
	int           exponent;
	int           exponent_adjustment_multiplier;

	p = *source;

	_ASSERTE(p != NULL);

	stored_state = p;

	// Skip past any leading whitespace:
	do
		c = *p++;
	while (is_space(c));

	// Check for the optional plus or minus sign:
	if ((fp_string->is_negative = c == '-') || c == '+')
		c = *p++;

	// Handle special cases "INF" and "INFINITY" (these are the only accepted
	// character sequences that start with 'I'):
	if ((lowercase = (c | ('a' - 'A'))) == 'i')
		return parse_floating_point_possible_infinity(p, source);

	// Handle special cases "NAN" and "NAN(...)" (these are the only accepted
	// character sequences that start with 'N'):
	if (lowercase == 'n')
		return parse_floating_point_possible_nan(p, source);

	// Check for optional "0x" or "0X" hexadecimal base prefix:
	if (is_hexadecimal = c == '0' && (*p | ('a' - 'A')) == 'x')
	{
		// If we match the hexadecimal base prefix we update the state to
		// reflect that we consumed the leading zero to handle the case
		// where a valid mantissa does not follow the base prefix.  In this
		// case, the "0x" string is treated as a decimal zero subject ("0")
		// followed by a final string starting with the "x".
		stored_state = p++;
		c = *p++;
	}

	mantissa_first = fp_string->mantissa;
	mantissa_last = fp_string->mantissa + _countof(fp_string->mantissa);
	mantissa_it = fp_string->mantissa;

	// The exponent adjustment holds the number of digits in the mantissa buffer
	// that appeared before the radix point.  It is positive for number strings
	// with an integer component and negative for number strings without.
	exponent_adjustment = 0;

	// Track whether we've seen any digits, so we know whether we've successfully
	// parsed something:
	found_digits = false;

	// Skip past any leading zeroes in the mantissa:
	if (found_digits = c == '0')
	{
		found_digits = true;
		do
			c = *p++;
		while (c == '0');
	}

	// Scan the integer part of the mantissa:
	max_digit_value = is_hexadecimal ? 0xFu : 9u;
	if ((digit_value = strtox_parse_digit(c)) <= max_digit_value)
	{
		found_digits = true;
		do
		{
			if (mantissa_it != mantissa_last)
				*mantissa_it++ = (uint8_t)digit_value;
			++exponent_adjustment;
		} while ((digit_value = strtox_parse_digit(c = *p++)) <= max_digit_value);
	}

	// If a radix point is present, scan the fractional part of the mantissa:
	if (!GetLocaleInfo(GetThreadLocale(), LOCALE_SDECIMAL, (TCHAR *)&radix_point, 2))
		radix_point = '.';
	if (c == (TCHAR)radix_point)
	{
		c = *p++;

		// If we haven't yet scanned any nonzero digits, continue skipping over
		// zeroes, updating the exponent adjustment to account for the zeroes
		// we are skipping:
		if (mantissa_it == mantissa_first && c == '0')
		{
			found_digits = true;
			do
				--exponent_adjustment;
			while ((c = *p++) == '0');
		}

		if ((digit_value = strtox_parse_digit(c)) <= max_digit_value)
		{
			found_digits = true;
			do
				if (mantissa_it != mantissa_last)
					*mantissa_it++ = (uint8_t)digit_value;
			while ((digit_value = strtox_parse_digit(c = *p++)) <= max_digit_value);
		}
	}

	if (!found_digits)
	{
		// We failed to parse any digits, so attempt to restore back to the last
		// good terminal state.  This may fail if we are reading from a stream,
		// we read a hexadecimal base prefix ("0x"), but we did not find any digits
		// following the base prefix.
		*source = stored_state;

		// If a hexadecimal base prefix was present ("0x"), then the string is a
		// valid input:  the "0" is the subject sequence and the "x" is the first
		// character of the final string.  Otherwise, the string is not a valid
		// input.
		return is_hexadecimal
			? FLOATING_POINT_PARSE_RESULT_ZERO
			: FLOATING_POINT_PARSE_RESULT_NO_DIGITS;
	}

	stored_state = p - 1;

	// Check for the optional 'e' or 'p' exponent introducer:
	switch (c)
	{
	default:
		has_exponent = false;
		break;

	case 'e':
	case 'E':
		has_exponent = !is_hexadecimal;
		break;

	case 'p':
	case 'P':
		has_exponent = is_hexadecimal;
		break;
	}

	// If there was an exponent introducer, scan the exponent:
	exponent = 0;
	if (has_exponent)
	{
		bool exponent_is_negative;
		bool has_exponent_digits;

		c = *p++; // Skip past exponent introducer character

		// Check for the optional plus or minus sign:
		exponent_is_negative = c == '-';
		if (c == '+' || c == '-')
			c = *p++;

		has_exponent_digits = false;

		if (c == '0')
		{
			has_exponent_digits = true;
			do
				c = *p++;
			while (c == '0');
		}

		if ((digit_value = strtox_parse_digit(c)) < 10)
		{
			has_exponent_digits = true;
			do
			{
				exponent = exponent * 10 + digit_value;
				if (exponent > MAXIMUM_TEMPORARY_DECIMAL_EXPONENT)
				{
					exponent = MAXIMUM_TEMPORARY_DECIMAL_EXPONENT + 1;
					break;
				}
			} while ((digit_value = strtox_parse_digit(c = *p++)) < 10);
		}

		// If the exponent is too large, skip over the remaining exponent digits
		// so we can correctly update the end pointer:
		while (strtox_parse_digit(c) < 10)
			c = *p++;

		if (exponent_is_negative)
			exponent = -exponent;

		// If no exponent digits were present, attempt to restore the last good
		// terminal state.
		if (!has_exponent_digits)
		{
			p = stored_state;

			// The call to restore_state will have ungotten the exponent
			// introducer.  Re-get this character, to restore us to the
			// state we had before we entered the exponent parsing block.
			c = *p++;
		}
	}

	// Unget the last character that we read that terminated input.  After this
	// point, we must not use the source, c, or stored_state.
	*source = p - 1;

	// Remove trailing zeroes from mantissa:
	while (mantissa_it != mantissa_first && *(mantissa_it - 1) == 0)
		--mantissa_it;

	// If the mantissa buffer is empty, the mantissa was composed of all zeroes
	// (so the mantissa is 0).  All such strings have the value zero, regardless
	// what the exponent is (because 0 x b^n == 0 for all b and n).  We can return
	// now.  Note that we defer this check until after we scan the exponent, so
	// that we can correctly update end_ptr to point past the end of the exponent.
	if (mantissa_it == mantissa_first)
		return FLOATING_POINT_PARSE_RESULT_ZERO;

	// Before we adjust the exponent, handle the case where we detected a wildly
	// out of range exponent during parsing and clamped the value:
	if (exponent > MAXIMUM_TEMPORARY_DECIMAL_EXPONENT)
		return FLOATING_POINT_PARSE_RESULT_OVERFLOW;

	if (exponent < MINIMUM_TEMPORARY_DECIMAL_EXPONENT)
		return FLOATING_POINT_PARSE_RESULT_UNDERFLOW;

	// In hexadecimal floating constants, the exponent is a base 2 exponent.  The
	// exponent adjustment computed during parsing has the same base as the
	// mantissa (so, 16 for hexadecimal floating constants).  We therefore need to
	// scale the base 16 multiplier to base 2 by multiplying by log2(16):
	exponent_adjustment_multiplier = is_hexadecimal ? 4 : 1;

	exponent += exponent_adjustment * exponent_adjustment_multiplier;

	// Verify that after adjustment the exponent isn't wildly out of range (if
	// it is, it isn't representable in any supported floating point format).
	if (exponent > MAXIMUM_TEMPORARY_DECIMAL_EXPONENT)
		return FLOATING_POINT_PARSE_RESULT_OVERFLOW;

	if (exponent < MINIMUM_TEMPORARY_DECIMAL_EXPONENT)
		return FLOATING_POINT_PARSE_RESULT_UNDERFLOW;

	fp_string->exponent = exponent;
	fp_string->mantissa_count = (uint32_t)(mantissa_it - mantissa_first);

	return is_hexadecimal
		? FLOATING_POINT_PARSE_RESULT_HEXADECIMAL_DIGITS
		: FLOATING_POINT_PARSE_RESULT_DECIMAL_DIGITS;
}

SLD_STATUS __fastcall strtox_parse_floating_point_write_result(
	const floating_point_parse_result parse_result,
	const floating_point_string       *fp_string,
	double                            *result);

__forceinline static SLD_STATUS parse_floating_point(
	const TCHAR **source,
	double      *result)
{
	floating_point_string       fp_string;
	floating_point_parse_result parse_result;

	_ASSERTE(result != NULL);

	// PERFORMANCE NOTE:  fp_string is intentionally left uninitialized.  Zero-
	// initialization is quite expensive and is unnecessary.  The benefit of not
	// zero-initializing is greatest for short inputs.

	parse_result = parse_floating_point_from_source(source, &fp_string);

	return strtox_parse_floating_point_write_result(parse_result, &fp_string, result);
}

#endif	// _CORECRT_INTERNAL_STRTOX_H_
