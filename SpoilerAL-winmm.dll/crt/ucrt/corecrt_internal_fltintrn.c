#define _CRT_SECURE_NO_WARNINGS
#include "corecrt_internal.h"
#include "corecrt_internal_fltintrn.h"
#include "big_integer.h"
#include "corecrt_stdio_config.h"
#include <windows.h>
#include <math.h>

#if defined(_MSC_VER) && _MSC_VER < 1600
typedef unsigned __int8  bool;
typedef unsigned __int16 uint16_t;
typedef __int32          int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64          int64_t;
typedef unsigned __int64 uint64_t;
#define false       0
#define true        1
#define INT32_MIN   _I32_MIN
#define INT32_MAX   _I32_MAX
#define INT64_MAX   _I64_MAX
#define UINT32_C(x) (x ## U)
#define UINT64_C(x) (x ## UI64)
#endif

#pragma warning(disable: 4615)

#pragma function(memset, strlen, log10)

#define USE_PRINTF 0

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Internal functions.
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
static __forceinline int internal_signbit(const double *x)
{
	return *(int64_t *)x < 0;
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Floating Point Conversion Routines
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
static uint32_t __fastcall fltintrn_fp_classify(const double *value)
{
	#define value_is_nan_or_infinity (components->exponent == (UINT32_C(1) << FLOATING_TRAITS_EXPONENT_BITS) - 1)

	const floating_traits *components;

	components = (const floating_traits *)value;
	if (!value_is_nan_or_infinity)
		return FP_CLASS_FINITE;
	else if (components->mantissa == 0)
		return FP_CLASS_INFINITY;
	else if (internal_signbit(&components->value) && components->mantissa == FLOATING_TRAITS_SPECIAL_NAN_MANTISSA_MASK)
		return FP_CLASS_INDETERMINATE;
	else if (components->mantissa & FLOATING_TRAITS_SPECIAL_NAN_MANTISSA_MASK) // Quiet NAN
		return FP_CLASS_QUIET_NAN;
	else // Signaling NAN
		return FP_CLASS_SIGNALING_NAN;

	#undef value_is_nan_or_infinity
}

errno_t __fastcall fltintrn_fp_strflt_to_string(
	char         *buffer,
	const size_t buffer_count,
	int          digits,
	_strflt      *pflt)
{
	char *buffer_it;
	char *mantissa_it;

	_VALIDATE_RETURN_ERRCODE(buffer != NULL, EINVAL);
	_VALIDATE_RETURN_ERRCODE(buffer_count > 0, EINVAL);
	buffer[0] = '\0';

	_VALIDATE_RETURN_ERRCODE(buffer_count > (size_t)((digits > 0 ? digits : 0) + 1), ERANGE);
	_VALIDATE_RETURN_ERRCODE(pflt != NULL, EINVAL);

	buffer_it = buffer;
	mantissa_it = pflt->mantissa;

	// The buffer will contain 'digits' decimal digits plus an optional overflow
	// digit for the rounding.

	// Initialize the first digit in the buffer to '0' (Note: not '\0') and set
	// the pointer to the second digit of the buffer.  The first digit is used
	// to handle overflow on rounding (e.g. 9.999... becomes 10.000...), which
	// requires a carry into the first digit.
	*buffer_it++ = '0';

	// Copy the digits of the value into the buffer (with '0' padding) and
	// insert the null terminator:
	while (digits > 0)
	{
		*buffer_it++ = *mantissa_it ? *mantissa_it++ : '0';
		--digits;
	}

	*buffer_it = '\0';

	// Do any rounding which may be needed.  Note:  if digits < 0, we don't do
	// any rounding because in this case, the rounding occurs in a digit which
	// will not be output because of the precision requested.
	if (digits >= 0 && *mantissa_it >= '5')
	{
		buffer_it--;

		while (*buffer_it == '9')
			*buffer_it-- = '0';

		*buffer_it += 1;
	}

	if (*buffer == '1')
		// The rounding caused overflow into the leading digit (e.g. 9.999...
		// became 10.000...), so increment the decimal point position by 1:
		pflt->decpt++;
	else
		// Move the entire string to the left one digit to remove the unused
		// overflow digit:
		strcpy(buffer, buffer + 1);

	return 0;
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Floating point binary to decimal conversion routines
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

// This function converts a finite, positive floating point value into its
// decimal representation.  The decimal mantissa and exponent are returned
// via the out parameters.  If the value is zero, negative, infinity, or nan,
// the behavior is undefined.
//
// This function is based on the digit generation algorithm described in the
// paper "Printing floating point numbers quickly and accurately," by Robert G.
// Burger and R. Kent Dybvig, from ACM SIGPLAN 1996 Conference on Programming
// Language Design and Implementation, June 1996.  This function deviates from
// that algorithm in its termination conditions:  the Burger-Dybvig algorithm
// terminates when it runs out of significant digits; this function continues
// generating digits, even if they are insignificant (this allows us to print
// large, exactly representable values exactly, e.g. large powers of two).
//
// This function stops generating digits when the first of the following
// conditions is true:  [1] the mantissa buffer is exhausted, [2] sufficient
// digits have been generated for a %f specifier with the requested precision,
// or [3] all remaining digits are known to be zero.
static __forceinline void convert_to_fos_high_precision(
	const double   *value,
	const uint32_t precision,
	int            *exponent,
	char           *mantissa_buffer,
	const size_t   mantissa_buffer_count)
{
	const floating_traits *components;
	bool                  is_denormal;
	uint64_t              mantissa_adjustment;
	int32_t               exponent_adjustment;
	uint64_t              f;
	int32_t               e;
	unsigned int          environment;
	int32_t               k;
	big_integer           r;
	big_integer           s;
	char                  *mantissa_it;
	uint32_t              initial_digit;
	uint32_t              required_digits;
	const char            *mantissa_last;

	_ASSERTE(mantissa_buffer_count > 0);

	components = (const floating_traits *)value;

	// Special handling is required for denormal values:  because the implicit
	// high order bit is a zero, not a one, we need to [1] not set that bit when
	// we expand the mantissa, and [2] increment the exponent to account for the
	// extra shift.
	is_denormal = components->exponent == 0;

	mantissa_adjustment = is_denormal
		? 0
		: (UINT64_C(1) << (FLOATING_TRAITS_MANTISSA_BITS - 1));

	exponent_adjustment = is_denormal
		? 2
		: 1;

	// f and e are the unbiased mantissa and exponent, respectively.  (Where one-
	// -letter variable names are used, they match the same variables from the
	// Burger-Dybvig paper.)
	f = components->mantissa + mantissa_adjustment;
	e =
		(int32_t)(components->exponent) -
		FLOATING_TRAITS_EXPONENT_BIAS -
		FLOATING_TRAITS_MANTISSA_BITS +
		exponent_adjustment;

	// k is the decimal exponent, such that the resulting decimal number is of
	// the form 0.mmmmm * 10^k, where mmmm are the mantissa digits we generate.
	// Note that the use of log10 here may not give the correct result:  it may
	// be off by one, e.g. as is the case for powers of ten (log10(100) is two,
	// but the correct value of k for 100 is 3).  We detect off-by-one errors
	// later and correct for them.
	environment = _controlfp(CW_DEFAULT & MCW_EM, MCW_EM);
	k = (int32_t)(ceil(log10(components->value)));
	_controlfp(environment, MCW_EM);
	if (k == INT32_MAX || k == INT32_MIN)
	{
		_ASSERTE(("unexpected input value; log10 failed", 0));
		k = 0;
	}

	// The floating point number is represented as a fraction, r / s.  The
	// initialization of these two values is as described in the Burger-Dybvig
	// paper.
	big_integer_assign_uint64(&r, f);
	if (e >= 0)
	{
		if (f != (UINT64_C(1) << (FLOATING_TRAITS_MANTISSA_BITS - 1)))
		{
			big_integer_shift_left(&r, e + 1);      // f * b^e * 2
			big_integer_assign_uint32(&s, 2);       // b * 2
		}
		else
		{
			big_integer_shift_left(&r, e + 2);      // f * b^(e+1) * 2
			big_integer_assign_uint32(&s, 4);       // b * 2
		}
	}
	else
	{
		if (e == FLOATING_TRAITS_MINIMUM_BINARY_EXPONENT ||
			f != (UINT64_C(1) << (FLOATING_TRAITS_MANTISSA_BITS - 1)))
		{
			big_integer_shift_left(&r, 1);          // f * 2
			big_integer_power_of_two(&s, -e + 1);   // b^-e * 2
		}
		else
		{
			big_integer_shift_left(&r, 2);          // f * b * 2
			big_integer_power_of_two(&s, -e + 2);   // b^(-e+1) * 2
		}
	}

	if (k >= 0)
		big_integer_multiply_by_power_of_ten(&s, k);
	else
		big_integer_multiply_by_power_of_ten(&r, -k);

	mantissa_it = mantissa_buffer;

	// Perform a trial digit generation to handle off-by-one errors in the
	// computation of 'k':  There are three possible cases, which we handle
	// in turn:
	big_integer_multiply_by_uint32(&r, 10);
	initial_digit = (uint32_t)big_integer_divide(&r, &s);

	// If the initial digit was computed as 10, k is too small.  We increment k
	// and adjust s as if it had been computed with the correct k above.  We
	// then treat the digit as a one, which is what it would have been extracted
	// as had k, r, and s been correct to begin with.
	if (initial_digit == 10)
	{
		++k;
		*mantissa_it++ = '1';
		big_integer_multiply_by_uint32(&s, 10);
	}
	// If the initial digit is zero, k is too large.  We decrement k and ignore
	// the zero that we read (the next digit that we read will be the "real"
	// initial digit.
	else if (initial_digit == 0)
	{
		--k;
	}
	// Otherwise, k was correct and the digit we read was the actual initial
	// digit of the number; just store it.
	else
	{
		*mantissa_it++ = (char)('0' + initial_digit);
	}

	*exponent = k; // k is now correct; store it for our caller

	// convert_to_fos_high_precision() generates digits assuming we're formatting with %f
	// When %e is the format specifier, adding the exponent to the number of required digits
	// is not needed. To avoid a change in export surface, this is supported by controlling the
	// size of the mantissa buffer by the caller (see fp_format_e()).
	required_digits = k >= 0 && precision <= INT_MAX
		? k + precision
		: precision;

	mantissa_last = mantissa_buffer + __min(mantissa_buffer_count - 1, required_digits);
	for (;;)
	{
		#define digits_per_iteration            UINT32_C(9)
		#define digits_per_iteration_multiplier UINT32_C(1000000000)

		uint32_t quotient;
		uint32_t i;

		if (mantissa_it == mantissa_last)
			break;

		if (big_integer_is_zero(&r))
			break;

		// To reduce the number of expensive high precision division operations,
		// we generate multiple digits per iteration.  Our quotient type is a
		// uint32_t, and the largest power of ten representable by a uint32_t is
		// 10^9, so we generate nine digits per iteration.

		big_integer_multiply_by_uint32(&r, digits_per_iteration_multiplier);
		quotient = (uint32_t)big_integer_divide(&r, &s);

		_ASSERTE(quotient < digits_per_iteration_multiplier);

		// Decompose the quotient into its nine component digits by repeatedly
		// dividing by ten.  This generates digits in reverse order.
		#pragma warning(suppress: 6293) // For-loop counts down from minimum
		for (i = digits_per_iteration - 1; i != (uint32_t)-1; --i)
		{
			char d;

			d = (char)('0' + quotient % 10);
			quotient /= 10;

			// We may not have room in the mantissa buffer for all of the digits;
			// ignore the ones for which we do not have room:
			if ((uint32_t)(mantissa_last - mantissa_it) < i)
				continue;

			mantissa_it[i] = d;
		}

		mantissa_it += __min(digits_per_iteration, (uint32_t)(mantissa_last - mantissa_it));

		#undef digits_per_iteration
		#undef digits_per_iteration_multiplier
	}

	*mantissa_it = '\0';
}

void __fastcall fltintrn_fltout(
	const double       *value,
	const unsigned int precision,
	_strflt            *flt,
	char               *result,
	const size_t       result_count)
{
	bool     value_is_zero;
	uint64_t absolute_value;
	uint32_t classification;

	flt->sign = internal_signbit(value) ? '-' : ' ';
	flt->decpt = 0;
	flt->mantissa = result;

	// Make the number positive before we pass it to the digit generator:
	absolute_value = *(uint64_t *)value & INT64_MAX;

	value_is_zero = !absolute_value;
	if (value_is_zero)
	{
		strncpy(result, "0", result_count);
		return;
	}

	// Handle special cases:
	classification = fltintrn_fp_classify(value);
	if (classification != FP_CLASS_FINITE)
		flt->decpt = 1;

	switch (classification)
	{
	case FP_CLASS_INFINITY:      strncpy(result, "1#INF" , result_count); return;
	case FP_CLASS_QUIET_NAN:     strncpy(result, "1#QNAN", result_count); return;
	case FP_CLASS_SIGNALING_NAN: strncpy(result, "1#SNAN", result_count); return;
	case FP_CLASS_INDETERMINATE: strncpy(result, "1#IND" , result_count); return;
	}

	// The digit generator produces a truncated sequence of digits.  To allow
	// our caller to correctly round the mantissa, we need to generate an extra
	// digit.
	convert_to_fos_high_precision((const double *)&absolute_value, precision + 1, &flt->decpt, result, result_count);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Functions for formatting floating point values with the %a, %e, %f, and %g
// printf format specifiers.
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

// The C String pointed to by string is shifted distance bytes to the right.
// If distance is negative, the string is shifted to the left.
// The C String pointed to by string and all shifting operations must be
// contained within buffer_base or buffer_count.
static __forceinline void shift_bytes(
	char         *buffer_base,
	const size_t buffer_count,
	char         *string,
	const int    distance)
{
	UNREFERENCED_PARAMETER(buffer_base);
	UNREFERENCED_PARAMETER(buffer_count);

	if (distance != 0)
		memmove(string + distance, string, strlen(string) + 1);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// NaN and Infinity Formatting
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
static __forceinline errno_t fp_format_nan_or_infinity(
	const uint32_t classification,
	const bool     is_negative,
	char           *result_buffer,
#if USE_PRINTF
	size_t         result_buffer_count,
	const bool     use_capitals)
#else
	size_t         result_buffer_count)
#endif
{
#if USE_PRINTF
	static const char *strings[][4] =
	{
		{ "INF",       "INF", "inf",       "inf" }, // Infinity
		{ "NAN",       "NAN", "nan",       "nan" }, // Quiet NAN
		{ "NAN(SNAN)", "NAN", "nan(snan)", "nan" }, // Signaling NAN
		{ "NAN(IND)",  "NAN", "nan(ind)",  "nan" }, // Indeterminate
	};
#else
	static const char *strings[][2] =
	{
		{ "inf",       "inf" }, // Infinity
		{ "nan",       "nan" }, // Quiet NAN
		{ "nan(snan)", "nan" }, // Signaling NAN
		{ "nan(ind)",  "nan" }, // Indeterminate
	};
#endif

	uint32_t row;
#if USE_PRINTF
	uint32_t column;
#endif
	bool     long_string_will_fit;

	#define restricted_count (size_t)4 // "INF" + 1 null terminator

	// Ensure that we have sufficient space to store at least the basic three-
	// character INF or NAN string, plus the minus sign if required:
	if (result_buffer_count < restricted_count + is_negative)
	{
		*result_buffer = '\0';
		return ENOMEM;
	}

	#undef restricted_count

	if (is_negative)
	{
		*result_buffer++ = '-';
		*result_buffer = '\0';
		if (result_buffer_count != _CRT_UNBOUNDED_BUFFER_SIZE)
			--result_buffer_count;
	}

#if USE_PRINTF
	row = (uint32_t)classification - 1;
	column = use_capitals ? 0 : 2;

	long_string_will_fit = result_buffer_count > strlen(strings[row][column]);
	strncpy(
		result_buffer,
		strings[row][column + !long_string_will_fit],
		result_buffer_count);
#else
	row = (uint32_t)classification - 1;

	long_string_will_fit = result_buffer_count > strlen(strings[row][0]);
	strncpy(
		result_buffer,
		strings[row][!long_string_will_fit],
		result_buffer_count);
#endif
	return 0;
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// %e formatting
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// These functions handle the formatting of floating point values in the %e
// printf format.  This format has the form [-]d.ddde(+/-)ddd, where there will
// be 'precision' digits following the decimal point.  If the precision is less
// than or equal to zero, no decimal point will appear.  The low order digit is
// rounded.  If 'capitals' is true, then the exponent will appear as E(+/-)ddd.
#if USE_PRINTF
static errno_t __fastcall fp_format_e_internal(
#else
static __forceinline errno_t fp_format_e_internal(
#endif
	char               *result_buffer,
	const size_t       result_buffer_count,
	const int          precision,
	const int          decimal_point_char,
#if USE_PRINTF
	const bool         capitals,
#endif
	const unsigned int min_exponent_digits,
	const _strflt      *pflt,
	const bool         g_fmt)
{
	char *p;
	char *exponentpos;

	// The max length if calculated like this:
	// 3 = sign + first digit + decimal point
	// precision = decimal digits
	// 5 = exponent letter (e or E), exponent sign, three digits exponent
	// 1 = extra space for rounding
	_VALIDATE_RETURN_ERRCODE(result_buffer_count > (size_t)(3 + (precision > 0 ? precision : 0) + 5 + 1), ERANGE);

	// Place the output in the buffer and round.  Leave space in the buffer
	// for the '-' sign (if any) and the decimal point (if any):
	if (g_fmt)
	{
		char *p;

		// Shift it right one place if nec. for decimal point:
		p = result_buffer + (pflt->sign == '-');
		shift_bytes(result_buffer, result_buffer_count, p, precision > 0);
	}

	// Now fix the number up to be in e format:
	p = result_buffer;

	// Put in negative sign if needed:
	if (pflt->sign == '-')
		*p++ = '-';

	// Put in decimal point if needed.  Copy the first digit to the place left
	// for it and put the decimal point in its place:
	if (precision > 0)
	{
		*p = *(p + 1);
		*++p = (char)decimal_point_char;
	}

	// Find the end of the string, attach the exponent field and save the
	// exponent position:
	p = p + precision + (g_fmt ? 0 : 1);
	strncpy(
		p, "e+000",
		result_buffer_count == _CRT_UNBOUNDED_BUFFER_SIZE
			? result_buffer_count
			: result_buffer_count - (p - result_buffer));
	exponentpos = p + 2;

#if USE_PRINTF
	// Adjust exponent indicator according to capitals flag and increment
	// pointer to point to exponent sign:
	if (capitals)
		*p = 'E';
#endif

	++p;

	// If mantissa is zero, then the number is 0 and we are done; otherwise
	// adjust the exponent sign (if necessary) and value:
	if (*pflt->mantissa != '0')
	{
		// Check to see if exponent is negative; if so adjust exponent sign and
		// exponent value:
		int exp = pflt->decpt - 1;
		if (exp < 0)
		{
			exp = -exp;
			*p = '-';
		}

		++p;

		if (exp >= 100)
		{
			*p += (char)(exp / 100);
			exp %= 100;
		}

		++p;

		if (exp >= 10)
		{
			*p += (char)(exp / 10);
			exp %= 10;
		}

		*++p += (char)exp;
	}

	if (min_exponent_digits == 2)
	{
		// If possible, reduce the exponent to two digits:
		if (*exponentpos == '0')
			memmove(exponentpos, exponentpos + 1, 3);
	}

	return 0;
}

static errno_t __fastcall fp_format_e(
	const double       *value,
	char               *result_buffer,
	const size_t       result_buffer_count,
	char               *scratch_buffer,
	const size_t       scratch_buffer_count,
	const int          precision,
	const int          decimal_point_char,
#if USE_PRINTF
	const bool         capitals,
#endif
	const unsigned int min_exponent_digits)
{
	_strflt      strflt;
	unsigned int displayed_digits_count;
	size_t       calculated_digits_count;
	size_t       scratch_buffer_restricted_count;
	errno_t      e;

	// The precision passed to fltintrn_fltout is the number of fractional digits.
	// To ensure that we get enough digits, we require a total of precision + 1 digits,
	// to account for the digit placed to the left of the decimal point when all digits are fractional.
	displayed_digits_count = precision + 1; // +1 for digit to left of decimal point

	// Restrict buffer size because we know exactly how much space is needed to display %e formatted floating point numbers.
	// With this change, we can optimize the %e path to not calculate precision past where will be displayed without
	// modifying the binary interface of fltintrn_fltout, which will calculate all digits that would be needed by %f formatting.
	calculated_digits_count = displayed_digits_count + 1; // +1 to calculate the value after last displayed digit (for rounding)
	scratch_buffer_restricted_count = min(calculated_digits_count + 1, scratch_buffer_count); // +1 for null terminator

	fltintrn_fltout(value, displayed_digits_count, &strflt, scratch_buffer, scratch_buffer_restricted_count);

	e = fltintrn_fp_strflt_to_string(
		result_buffer + (strflt.sign == '-') + (precision > 0),
		(result_buffer_count == _CRT_UNBOUNDED_BUFFER_SIZE
			? result_buffer_count
			: result_buffer_count - (strflt.sign == '-') - (precision > 0)),
		precision + 1,
		&strflt);

	if (e != 0)
	{
		result_buffer[0] = '\0';
		return e;
	}

#if USE_PRINTF
	return fp_format_e_internal(result_buffer, result_buffer_count, precision, decimal_point_char, capitals, min_exponent_digits, &strflt, false);
#else
	return fp_format_e_internal(result_buffer, result_buffer_count, precision, decimal_point_char, min_exponent_digits, &strflt, false);
#endif
}

#if USE_PRINTF
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// %a formatting
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// These functions handle the formatting of floating point values in the %a
// printf format.  This format has the form [-]0xh.hhhhp(+/-)d, where there will
// be 'precision' hex digits following the decimal point.  If the precision is
// less than or equal to zero, no decimal point will appear.  If 'capitals' is
// true, then the number will appear as [-]0xH.HHHHP(+/-)d.
static __forceinline errno_t fp_format_a(
	const double       *value,
	char               *result_buffer,
	const size_t       result_buffer_count,
	char               *scratch_buffer,
	const size_t       scratch_buffer_count,
	int                precision,
	const int          decimal_point_char,
	const bool         capitals,
	const unsigned int min_exponent_digits)
{
	const floating_traits *components;
	int                   hexadd;
	uint64_t              debias;
	char                  *pos;
	int64_t               exponent;

	if (precision < 0)
		precision = 0;

	result_buffer[0] = '\0';

	// the constraint for the size of buffer is:
	// 1 (sign)
	// 4 ("0xh.")
	// precision (decimal digits)
	// 6 ("p+0000")
	_VALIDATE_RETURN_ERRCODE(result_buffer_count > (size_t)(1 + 4 + precision + 6), ERANGE);

	// Let __acrt_fp_format_e handle the special cases like SNAN, etc.:
	components = (const floating_traits *)value;
	if (components->exponent == FLOATING_TRAITS_EXPONENT_MASK)
	{
		errno_t e;
		char    *p;

		e = fp_format_e(
			value,
			result_buffer,
			result_buffer_count,
			scratch_buffer,
			scratch_buffer_count,
			precision,
			decimal_point_char,
			false,
			min_exponent_digits);

		if (e != 0)
		{
			// An error occurred
			result_buffer[0] = '\0';
			return e;
		}

		// Substitute the e with p:
		p = strrchr(result_buffer, 'e');
		if (p)
		{
			*p = capitals ? 'P' : 'p';

			// Trim the exponent (which is 0) to only one digit; skip the
			// exponent sign and the first digit and put the terminating 0:
			p += 3;
			*p = 0;
		}
		return e;
	}

	// Sign:
	if (internal_signbit(&components->value))
		*result_buffer++ = '-';

	hexadd = (capitals ? 'A' : 'a') - '9' - 1;

	// Leading digit (and set the debias):
	debias = FLOATING_TRAITS_EXPONENT_BIAS;
	if (components->exponent == 0)
	{
		*result_buffer++ = '0';
		if (components->mantissa == 0)
			// Zero:
			debias = 0;
		else
			// Denormal:
			debias--;
	}
	else
	{
		*result_buffer++ = '1';
	}

	// Decimal point (save the position in pos):
	pos = result_buffer++;
	if (precision == 0)
		// If precision is 0, then we don't have to print the decimal point:
		// we mark this putting 0 instead of the decimal point itself
		*pos = 0;
	else
		*pos = (char)decimal_point_char;

	// Mantissa:
	if (components->mantissa > 0)
	{
		short    maskpos;
		uint64_t mask;

		// Print 4 bits at a time, and skip the initial zeroes
		// Prepare the mask to read the first 4 bits
		maskpos = (FLOATING_TRAITS_MANTISSA_BITS - 1) - 4;

		mask = 0xF;
		mask <<= maskpos;

		while (maskpos >= 0 && precision > 0)
		{
			unsigned short digit;

			digit = (unsigned short)((components->mantissa & mask) >> maskpos);
			digit += '0';
			if (digit > '9')
				digit += (unsigned short)hexadd;
			*result_buffer++ = (char)digit;
			mask >>= 4;
			maskpos -= 4;
			--precision;
		}

		// Round the mantissa:
		if (maskpos >= 0)
		{
			unsigned short digit;

			digit = (unsigned short)((components->mantissa & mask) >> maskpos);
			if (digit > 8)
			{
				char *p;

				p = result_buffer;
				--p;
				// If the last digit is 'f', we need to add one to the previous
				// digit, too; pos is the position of the decimal point
				while (*p == 'f' || *p == 'F')
					*p-- = '0';
				// If we reached the decimal point, it means we are rounding
				// something like 0x0.fffff so this will become 0x1.00000 :
				if (p != pos)
				{
					if (*p == '9')
						*p += (char)(1 + hexadd);
					else
						*p += 1;
				}
				else // p == pos
				{
					// Skip the decimal point:
					--p;

					// The first digit is always 0 or 1, so we don't need to
					// add hexadd:
					*p += 1;
				}
			}
		}
	}

	// Add the final zeroes, if needed:
	for (; precision > 0; --precision)
		*result_buffer++ = '0';

	// Move back the buffer pointer if there is no decimal point:
	if (*pos == 0)
		result_buffer = pos;

	// Exponent:
	*result_buffer++ = capitals ? 'P' : 'p';
	exponent = components->exponent - debias;
	if (exponent >= 0)
	{
		*result_buffer++ = '+';
	}
	else
	{
		*result_buffer++ = '-';
		exponent = -exponent;
	}
	// Save the position in pos and write a '0':
	pos = result_buffer;
	*pos = '0';

	if (exponent >= 1000)
	{
		*result_buffer++ = '0' + (char)(exponent / 1000);
		exponent %= 1000;
	}
	if (result_buffer != pos || exponent >= 100)
	{
		*result_buffer++ = '0' + (char)(exponent / 100);
		exponent %= 100;
	}
	if (result_buffer != pos || exponent >= 10)
	{
		*result_buffer++ = '0' + (char)(exponent / 10);
		exponent %= 10;
	}

	*result_buffer++ = '0' + (char)exponent;

	// Terminate the string:
	*result_buffer = '\0';

	return 0;
}
#endif

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// %f formatting
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// These functions handle the formatting of floating point values in the %f
// printf format.  This format has the form [-]ddddd.ddddd, where there will be
// precision digits following the decimal point.  If precision is less than or
// equal to zero, no decimal point will appear.  The low order digit is rounded.
#if USE_PRINTF
static errno_t __fastcall fp_format_f_internal(
#else
static __forceinline errno_t fp_format_f_internal(
#endif
	char           *buffer,
	const size_t   buffer_count,
	const int      precision,
	const int      decimal_point_char,
	const _strflt  *pflt,
	const bool     g_fmt)
{
	int  g_magnitude;
	char *p;

	g_magnitude = pflt->decpt - 1;

	// Place the output in the user's buffer and round.  Save space for the
	// minus sign now if it will be needed:
	if (g_fmt && g_magnitude == precision)
	{
		char *p;

		p = g_magnitude + buffer + (pflt->sign == '-');
		p[0] = '0';
		p[1] = '\0';

		// Allows for extra place-holding '0' in the exponent == precision case
		// of the %g format.
	}

	// Now fix up the number to be in the correct %f format:
	p = buffer;

	// Put in a negative sign, if necessary:
	if (pflt->sign == '-')
		*p++ = '-';

	// Insert a leading zero for purely fractional values and position ourselves
	// at the correct spot for inserting the decimal point:
	if (pflt->decpt <= 0)
	{
		shift_bytes(buffer, buffer_count, p, 1);
		*p++ = '0';
	}
	else
	{
		p += pflt->decpt;
	}

	// Put a decimal point if required, and add any needed zero padding:
	if (precision > 0)
	{
		shift_bytes(buffer, buffer_count, p, 1);
		*p++ = (char)decimal_point_char;

		// If the value is less than 1 then we may need to put zeroes out in
		// front of the first non-zero digit of the mantissa:
		if (pflt->decpt < 0)
		{
			int computed_precision;

			computed_precision = (g_fmt || -pflt->decpt < precision)
				? -pflt->decpt
				: precision;

			shift_bytes(buffer, buffer_count, p, computed_precision);
			memset(p, '0', computed_precision);
		}
	}

	return 0;
}

static __forceinline errno_t fp_format_f(
	const double   *value,
	char           *result_buffer,
	const size_t   result_buffer_count,
	char           *scratch_buffer,
	const size_t   scratch_buffer_count,
	const int      precision,
	const int      decimal_point_char)
{
	_strflt strflt;
	errno_t e;

	fltintrn_fltout(value, precision, &strflt, scratch_buffer, scratch_buffer_count);

	e = fltintrn_fp_strflt_to_string(
		result_buffer + (strflt.sign == '-'),
		(result_buffer_count == _CRT_UNBOUNDED_BUFFER_SIZE ? result_buffer_count : result_buffer_count - (strflt.sign == '-')),
		precision + strflt.decpt,
		&strflt);

	if (e != 0)
	{
		result_buffer[0] = '\0';
		return e;
	}

	return fp_format_f_internal(result_buffer, result_buffer_count, precision, decimal_point_char, &strflt, false);
}

#if USE_PRINTF
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// %g formatting
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// These functions handle the formatting of floating point values in the %g
// printf format.  The form used depends on the value converted.  The printf %e
// form will be used if the magnitude of the value is less than -4 or is greater
// than 'precision', otherwise %f will be used.  The 'precision' always specifies
// the number of digits following the decimal point.  The low order digit is
// appropriately rounded.
static __forceinline errno_t fp_format_g(
	const double       *value,
	char               *result_buffer,
	const size_t       result_buffer_count,
	char               *scratch_buffer,
	const size_t       scratch_buffer_count,
	const int          precision,
	const int          decimal_point_char,
	const bool         capitals,
	const unsigned int min_exponent_digits)
{
	_strflt strflt;
	size_t  minus_sign_length;
	int     g_magnitude;
	char    *p;
	size_t  buffer_count_for_fptostr;
	errno_t fptostr_result;
	bool    g_round_expansion;

	fltintrn_fltout(value, precision, &strflt, scratch_buffer, scratch_buffer_count);

	minus_sign_length = strflt.sign == '-' ? 1 : 0;

	g_magnitude = strflt.decpt - 1;
	p = result_buffer + minus_sign_length;

	buffer_count_for_fptostr = result_buffer_count == _CRT_UNBOUNDED_BUFFER_SIZE
		? result_buffer_count
		: result_buffer_count - minus_sign_length;

	fptostr_result = fltintrn_fp_strflt_to_string(p, buffer_count_for_fptostr, precision, &strflt);
	if (fptostr_result != 0)
	{
		result_buffer[0] = '\0';
		return fptostr_result;
	}

	g_round_expansion = g_magnitude < (strflt.decpt - 1);

	// Compute the magnitude of value:
	g_magnitude = strflt.decpt - 1;

	// Convert value to the C Language g format:
	if (g_magnitude < -4 || g_magnitude >= precision) // Use e format
	{
		// We can ignore the round expansion flag here:  the extra digit will be
		// overwritten by "e+xxx".
		return fp_format_e_internal(result_buffer, result_buffer_count, precision, decimal_point_char, capitals, min_exponent_digits, &strflt, true);
	}
	else // Use f format
	{
		if (g_round_expansion)
		{
			// Throw away extra final digit from expansion:
			while (*p++);
			*(p - 2) = '\0';
		}

		return fp_format_f_internal(result_buffer, result_buffer_count, precision, decimal_point_char, &strflt, true);
	}
}
#endif

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Format Dispatch
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// The main floating point formatting dispatch function.  This function just
// looks at the 'format' character, calls the right formatting function, and
// returns the result.  The other parameters are passed on to the selected
// formatting function and are used as described in the documentation for
// those functions.
errno_t __fastcall fltintrn_fp_format(
	const double   *value,
	char           *result_buffer,
	const size_t   result_buffer_count,
	char           *scratch_buffer,
	const size_t   scratch_buffer_count,
	const int      format,
	const int      precision,
	const uint64_t options,
	const int      decimal_point_char)
{
#if USE_PRINTF
	bool         use_capitals;
#endif
	unsigned int min_exponent_digits;

	_VALIDATE_RETURN_ERRCODE(result_buffer != NULL, EINVAL);
	_VALIDATE_RETURN_ERRCODE(result_buffer_count > 0, EINVAL);
	_VALIDATE_RETURN_ERRCODE(scratch_buffer != NULL, EINVAL);
	_VALIDATE_RETURN_ERRCODE(scratch_buffer_count > 0, EINVAL);

#if USE_PRINTF
	use_capitals = format == 'A' || format == 'E' || format == 'F' || format == 'G';
#endif

	// Detect special cases (NaNs and infinities) and handle them specially.
	// Note that the underlying fltintrn_fltout function will also handle these
	// special cases, but it does so using the legacy strings (e.g. 1.#INF).
	// Our special handling here uses the C99 strings (e.g. INF).
#if USE_PRINTF
	if ((options & _CRT_INTERNAL_PRINTF_LEGACY_MSVCRT_COMPATIBILITY) == 0)
#endif
	{
		uint32_t classification;

		classification = fltintrn_fp_classify(value);
		if (classification != FP_CLASS_FINITE)
			return fp_format_nan_or_infinity(
				classification,
				(bool)internal_signbit(value),
				result_buffer,
#if USE_PRINTF
				result_buffer_count,
				use_capitals);
#else
				result_buffer_count);
#endif
	}

#if USE_PRINTF
	min_exponent_digits = (options & _CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS) != 0 ? 3 : 2;

	switch (format)
	{
	case 'a':
	case 'A':
		return fp_format_a(value, result_buffer, result_buffer_count, scratch_buffer, scratch_buffer_count, precision, decimal_point_char, use_capitals, min_exponent_digits);

	case 'e':
	case 'E':
		return fp_format_e(value, result_buffer, result_buffer_count, scratch_buffer, scratch_buffer_count, precision, decimal_point_char, use_capitals, min_exponent_digits);

	case 'f':
	case 'F':
		return fp_format_f(value, result_buffer, result_buffer_count, scratch_buffer, scratch_buffer_count, precision, decimal_point_char);

	default:
		_ASSERTE(("Unsupported format specifier", 0));
	case 'g':
	case 'G':
		return fp_format_g(value, result_buffer, result_buffer_count, scratch_buffer, scratch_buffer_count, precision, decimal_point_char, use_capitals, min_exponent_digits);
	}
#else
	min_exponent_digits = 3;

	switch (format)
	{
	case 'e':
		return fp_format_e(value, result_buffer, result_buffer_count, scratch_buffer, scratch_buffer_count, precision, decimal_point_char, min_exponent_digits);

	case 'f':
		return fp_format_f(value, result_buffer, result_buffer_count, scratch_buffer, scratch_buffer_count, precision, decimal_point_char);

	default:
		__assume(0);
	}
#endif
}
