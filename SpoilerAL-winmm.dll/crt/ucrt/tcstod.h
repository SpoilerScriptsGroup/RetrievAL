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
typedef unsigned __int16 uint16_t;
typedef __int32          int32_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#define false       0
#define true        1
#define UINT64_C(x) (x ## UI64)
#endif

#ifndef _UNICODE
typedef uint8_t  uchar_t;
#else
typedef uint16_t uchar_t;
#endif

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Character Sources
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
typedef struct
{
	const TCHAR *p;
	const TCHAR **end;
} c_string_character_source;

__forceinline void c_string_character_source_ctor(c_string_character_source *this, const TCHAR *string, const TCHAR **end)
{
	this->p = string;
	this->end = end;
	if (end)
	{
		*end = string;
	}
}

__forceinline void c_string_character_source_dtor(c_string_character_source *this)
{
	if (this->end)
	{
		*this->end = this->p;
	}
}

__forceinline bool c_string_character_source_validate(c_string_character_source *this)
{
	_VALIDATE_RETURN(this->p != NULL, EINVAL, false);
	return true;
}

__forceinline TCHAR c_string_character_source_get(c_string_character_source *this)
{
	return *this->p++;
}

__forceinline void c_string_character_source_unget(c_string_character_source *this, const TCHAR c)
{
	--this->p;
	_VALIDATE_RETURN_VOID(c == '\0' || *this->p == c, EINVAL);
}

__forceinline const TCHAR *c_string_character_source_save_state(c_string_character_source *this)
{
	return this->p;
}

__forceinline void c_string_character_source_restore_state(c_string_character_source *this, const TCHAR *state)
{
	this->p = state;
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// String-to-Integer Conversion
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
static unsigned int parse_digit(const TCHAR c)
{
	switch ((uchar_t)c)
	{
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		return (unsigned int)(uchar_t)c - '0';
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
		return (unsigned int)(uchar_t)c - 'A' + 10;
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
		return (unsigned int)(uchar_t)c - 'a' + 10;
	default:
		return -1;
	}
}

// The digit and nondigit categories include 0-9, a-z, A-Z, and _.  They are not
// locale-dependent, so we do not call isalnum (which uses the current locale to
// test for alphabetic characters).
__forceinline static bool is_digit_or_nondigit(const int c)
{
	switch ((uchar_t)c)
	{
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
	case '_':
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
		return true;
	default:
		return false;
	}
}

__forceinline static bool is_space(const TCHAR c)
{
	return c == ' ' || (uchar_t)((c) - '\t') < '\r' - '\t' + 1;
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// String-to-Floating-Point Conversion
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
typedef enum
{
	SLD_OK,

	SLD_NODIGITS,

	SLD_UNDERFLOW,
	SLD_OVERFLOW
} SLD_STATUS;

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
typedef struct
{
	int32_t  exponent;
	uint32_t mantissa_count;
	uint8_t  mantissa[768];
	bool     is_negative;
} floating_point_string;

// Stores a positive or negative zero into the result object
static void assemble_floating_point_zero(const bool is_negative, double *result)
{
	floating_traits *components;

	components = (floating_traits *)result;
	components->sign     = is_negative ? 1 : 0;
	components->exponent = 0;
	components->mantissa = 0;
}

// Stores a positive or negative infinity into the result object
static void assemble_floating_point_infinity(const bool is_negative, double *result)
{
	floating_traits *components;

	components = (floating_traits *)result;
	components->sign     = is_negative ? 1 : 0;
	components->exponent = FLOATING_TRAITS_EXPONENT_MASK;
	components->mantissa = 0;
}

// Stores a positive or negative quiet NaN into the result object
__forceinline static void assemble_floating_point_qnan(const bool is_negative, double *result)
{
	floating_traits *components;

	components = (floating_traits *)result;
	components->sign     = is_negative ? 1 : 0;
	components->exponent = FLOATING_TRAITS_EXPONENT_MASK;
	components->mantissa = FLOATING_TRAITS_DENORMAL_MANTISSA_MASK;
}

// Stores a positive or negative signaling NaN into the result object
__forceinline static void assemble_floating_point_snan(const bool is_negative, double *result)
{
	floating_traits *components;

	components = (floating_traits *)result;
	components->sign     = is_negative ? 1 : 0;
	components->exponent = FLOATING_TRAITS_EXPONENT_MASK;
	components->mantissa = 1;
}

// Stores an indeterminate into the result object (the indeterminate is "negative")
__forceinline static void assemble_floating_point_ind(double *result)
{
	floating_traits *components;

	components = (floating_traits *)result;
	components->sign     = 1;
	components->exponent = FLOATING_TRAITS_EXPONENT_MASK;
	components->mantissa = FLOATING_TRAITS_SPECIAL_NAN_MANTISSA_MASK;
}

// Determines whether a mantissa should be rounded up in the current rounding
// mode given [1] the value of the least significant bit of the mantissa, [2]
// the value of the next bit after the least significant bit (the "round" bit)
// and [3] whether any trailing bits after the round bit are set.
//
// The mantissa is treated as an unsigned integer magnitude; the sign is used
// only to compute the correct rounding direction for directional rounding modes.
//
// For this function, "round up" is defined as "increase the magnitude" of the
// mantissa.  (Note that this means that if we need to round a negative value to
// the next largest representable value, we return false, because the next
// largest representable value has a smaller magnitude.)
__forceinline static bool should_round_up(
	const bool is_negative,
	const bool lsb_bit,
	const bool round_bit,
	const bool has_tail_bits)
{
	bool is_exactly_representable;

	// If there are no insignificant set bits, the value is exactly representable
	// and should not be rounded in any rounding mode:
	is_exactly_representable = !round_bit && !has_tail_bits;
	if (is_exactly_representable)
	{
		return false;
	}

	// If there are insignificant set bits, we need to round according to the
	// current rounding mode.  For RC_NEAR, we need to handle two cases:
	// we round up if either [1] the value is slightly greater than the midpoint
	// between two exactly representable values or [2] the value is exactly the
	// midpoint between two exactly representable values and the greater of the
	// two is even (this is "round-to-even").
	switch (_controlfp(0, 0) & MCW_RC)
	{
	case RC_NEAR: return round_bit && (has_tail_bits || lsb_bit);
	case RC_UP:   return !is_negative;
	case RC_DOWN: return is_negative;
	case RC_CHOP: return false;
	}

	_ASSERTE(("unexpected rounding mode", false));
	return false;
}

// Computes value / 2^shift, then rounds the result according to the current
// rounding mode.  By the time we call this function, we will already have
// discarded most digits.  The caller must pass true for has_zero_tail if
// all discarded bits were zeroes.
__forceinline static uint64_t right_shift_with_rounding(
	const bool     is_negative,
	const uint64_t value,
	const uint32_t shift,
	const bool     has_zero_tail)
{
	uint64_t extra_bits_mask;
	uint64_t round_bit_mask;
	uint64_t lsb_bit_mask;
	bool     tail_bits;
	bool     round_bit;
	bool     lsb_bit;

	// If we'd need to shift further than it is possible to shift, the answer
	// is always zero:
	if (shift >= sizeof(uint64_t) * CHAR_BIT)
	{
		return 0;
	}

	extra_bits_mask = (UINT64_C(1) << (shift - 1)) - 1;
	round_bit_mask = (UINT64_C(1) << (shift - 1));
	lsb_bit_mask = UINT64_C(1) << shift;

	tail_bits = !has_zero_tail || (value & extra_bits_mask) != 0;
	round_bit = (value & round_bit_mask) != 0;
	lsb_bit = (value & lsb_bit_mask) != 0;

	return (value >> shift) + should_round_up(is_negative, lsb_bit, round_bit, tail_bits);
}

// Converts the floating point value [sign] 0.mantissa * 2^exponent into the
// correct form for double *and stores the result into the result object.
// The caller must ensure that the mantissa and exponent are correctly computed
// such that either [1] the most significant bit of the mantissa is in the
// correct position for the double, or [2] the exponent has been correctly
// adjusted to account for the shift of the mantissa that will be required.
//
// This function correctly handles range errors and stores a zero or infinity in
// the result object on underflow and overflow errors, respectively.  This
// function correctly forms denormal numbers when required.
//
// If the provided mantissa has more bits of precision than can be stored in the
// result object, the mantissa is rounded to the available precision.  Thus, if
// possible, the caller should provide a mantissa with at least one more bit of
// precision than is required, to ensure that the mantissa is correctly rounded.
// (The caller should not round the mantissa before calling this function.)
__forceinline static SLD_STATUS assemble_floating_point_value_t(
	const bool     is_negative,
	const int32_t  exponent,
	const uint64_t mantissa,
	double         *result)
{
	floating_traits *components;

	components = (floating_traits *)result;
	components->sign     = is_negative;
	components->exponent = exponent + FLOATING_TRAITS_EXPONENT_BIAS;
	components->mantissa = mantissa;
	return SLD_OK;
}

static SLD_STATUS assemble_floating_point_value(
	const uint64_t  initial_mantissa,
	const int32_t   initial_exponent,
	const bool      is_negative,
	const bool      has_zero_tail,
	floating_traits *result)
{
	uint32_t initial_mantissa_bits;
	int32_t  normal_mantissa_shift;
	int32_t  normal_exponent;
	uint64_t mantissa;
	int32_t  exponent;

	// Assume that the number is representable as a normal value.  Compute the
	// number of bits by which we must adjust the mantissa to shift it into the
	// correct position, and compute the resulting base two exponent for the
	// normalized mantissa:
	initial_mantissa_bits = big_integer_bit_scan_reverse64(initial_mantissa);
	normal_mantissa_shift = (int32_t)(FLOATING_TRAITS_MANTISSA_BITS - initial_mantissa_bits);
	normal_exponent = initial_exponent - normal_mantissa_shift;

	mantissa = initial_mantissa;
	exponent = normal_exponent;

	if (normal_exponent > FLOATING_TRAITS_MAXIMUM_BINARY_EXPONENT)
	{
		// The exponent is too large to be represented by the floating point
		// type; report the overflow condition:
		assemble_floating_point_infinity(is_negative, &result->value);
		return SLD_OVERFLOW;
	}
	else if (normal_exponent < FLOATING_TRAITS_MINIMUM_BINARY_EXPONENT)
	{
		int32_t denormal_mantissa_shift;

		// The exponent is too small to be represented by the floating point
		// type as a normal value, but it may be representable as a denormal
		// value.  Compute the number of bits by which we need to shift the
		// mantissa in order to form a denormal number.  (The subtraction of
		// an extra 1 is to account for the hidden bit of the mantissa that
		// is not available for use when representing a denormal.)
		denormal_mantissa_shift =
			normal_mantissa_shift +
			normal_exponent +
			FLOATING_TRAITS_EXPONENT_BIAS -
			1;

		// Denormal values have an exponent of zero, so the debiased exponent is
		// the negation of the exponent bias:
		exponent = -FLOATING_TRAITS_EXPONENT_BIAS;

		if (denormal_mantissa_shift < 0)
		{
			// Use two steps for right shifts:  for a shift of N bits, we first
			// shift by N-1 bits, then shift the last bit and use its value to
			// round the mantissa.
			mantissa = right_shift_with_rounding(is_negative, mantissa, -denormal_mantissa_shift, has_zero_tail);

			// If the mantissa is now zero, we have underflowed:
			if (mantissa == 0)
			{
				assemble_floating_point_zero(is_negative, &result->value);
				return SLD_UNDERFLOW;
			}

			// When we round the mantissa, the result may be so large that the
			// number becomes a normal value.  For example, consider the single
			// precision case where the mantissa is 0x01ffffff and a right shift
			// of 2 is required to shift the value into position. We perform the
			// shift in two steps:  we shift by one bit, then we shift again and
			// round using the dropped bit.  The initial shift yields 0x00ffffff.
			// The rounding shift then yields 0x007fffff and because the least
			// significant bit was 1, we add 1 to this number to round it.  The
			// final result is 0x00800000.
			//
			// 0x00800000 is 24 bits, which is more than the 23 bits available
			// in the mantissa.  Thus, we have rounded our denormal number into
			// a normal number.
			//
			// We detect this case here and re-adjust the mantissa and exponent
			// appropriately, to form a normal number:
			if (mantissa > FLOATING_TRAITS_DENORMAL_MANTISSA_MASK)
			{
				// We add one to the denormal_mantissa_shift to account for the
				// hidden mantissa bit (we subtracted one to account for this bit
				// when we computed the denormal_mantissa_shift above).
				exponent =
					initial_exponent -
					(denormal_mantissa_shift + 1) -
					normal_mantissa_shift;
			}
		}
		else
		{
			mantissa <<= denormal_mantissa_shift;
		}
	}
	else
	{
		if (normal_mantissa_shift < 0)
		{
			// Use two steps for right shifts:  for a shift of N bits, we first
			// shift by N-1 bits, then shift the last bit and use its value to
			// round the mantissa.
			mantissa = right_shift_with_rounding(is_negative, mantissa, -normal_mantissa_shift, has_zero_tail);

			// When we round the mantissa, it may produce a result that is too
			// large.  In this case, we divide the mantissa by two and increment
			// the exponent (this does not change the value).
			if (mantissa > FLOATING_TRAITS_NORMAL_MANTISSA_MASK)
			{
				mantissa >>= 1;
				++exponent;

				// The increment of the exponent may have generated a value too
				// large to be represented.  In this case, report the overflow:
				if (exponent > FLOATING_TRAITS_MAXIMUM_BINARY_EXPONENT)
				{
					assemble_floating_point_infinity(is_negative, &result->value);
					return SLD_OVERFLOW;
				}
			}
		}
		else if (normal_mantissa_shift > 0)
		{
			mantissa <<= normal_mantissa_shift;
		}
	}

	// Unset the hidden bit in the mantissa and assemble the floating point value
	// from the computed components:
	mantissa &= FLOATING_TRAITS_DENORMAL_MANTISSA_MASK;

	return assemble_floating_point_value_t(is_negative, exponent, mantissa, &result->value);
}

// This function is part of the fast track for integer floating point strings.
// It takes an integer and a sign and converts the value into its double
// representation, storing the result in the result object.  If the value is not
// representable, +/-infinity is stored and overflow is reported (since this
// function only deals with integers, underflow is impossible).
static SLD_STATUS assemble_floating_point_value_from_big_integer(
	const big_integer *integer_value,
	const uint32_t    integer_bits_of_precision,
	const bool        is_negative,
	const bool        has_nonzero_fractional_part,
	floating_traits   *result)
{
	const int32_t base_exponent = FLOATING_TRAITS_MANTISSA_BITS - 1;

	uint32_t top_element_bits;
	uint32_t top_element_index;
	uint32_t middle_element_index;
	uint32_t bottom_element_index;
	uint32_t top_element_mask;
	uint32_t top_element_shift;
	uint32_t middle_element_shift;
	uint32_t bottom_element_bits;
	uint32_t bottom_element_mask;
	uint32_t bottom_element_shift;
	int32_t  exponent;
	uint64_t mantissa;
	bool     has_zero_tail;
	uint32_t i;

	// Very fast case:  If we have fewer than 64 bits of precision, we can just
	// take the two low order elements from the big_integer:
	if (integer_bits_of_precision <= 64)
	{
		int32_t  exponent;
		uint32_t mantissa_low;
		uint32_t mantissa_high;
		uint64_t mantissa;

		exponent = base_exponent;

		mantissa_low = integer_value->used > 0 ? integer_value->data[0] : 0;
		mantissa_high = integer_value->used > 1 ? integer_value->data[1] : 0;
		mantissa =
			mantissa_low +
			((uint64_t)mantissa_high << 32);

		return assemble_floating_point_value(mantissa, exponent, is_negative, (bool)!has_nonzero_fractional_part, result);
	}

	top_element_bits = integer_bits_of_precision % 32;
	top_element_index = integer_bits_of_precision / 32;

	middle_element_index = top_element_index - 1;
	bottom_element_index = top_element_index - 2;

	// Pretty fast case:  If the top 64 bits occupy only two elements, we can
	// just combine those two elements:
	if (top_element_bits == 0)
	{
		int32_t  exponent;
		uint64_t mantissa;
		bool     has_zero_tail;
		uint32_t i;

		exponent = base_exponent + bottom_element_index * 32;

		mantissa =
			integer_value->data[bottom_element_index] +
			((uint64_t)integer_value->data[middle_element_index] << 32);

		has_zero_tail = !has_nonzero_fractional_part;
		for (i = 0; i != bottom_element_index; ++i)
		{
			has_zero_tail &= integer_value->data[i] == 0;
		}

		return assemble_floating_point_value(mantissa, exponent, is_negative, has_zero_tail, result);
	}

	// Not quite so fast case:  The top 64 bits span three elements in the big
	// integer.  Assemble the three pieces:
	top_element_mask = (1u << top_element_bits) - 1;
	top_element_shift = 64 - top_element_bits; // Left

	middle_element_shift = top_element_shift - 32; // Left

	bottom_element_bits = 32 - top_element_bits;
	bottom_element_mask = ~top_element_mask;
	bottom_element_shift = 32 - bottom_element_bits; // Right

	exponent = base_exponent + bottom_element_index * 32 + top_element_bits;

	mantissa =
		((uint64_t)(integer_value->data[top_element_index] & top_element_mask) << top_element_shift) +
		((uint64_t)integer_value->data[middle_element_index] << middle_element_shift) +
		((uint64_t)(integer_value->data[bottom_element_index] & bottom_element_mask) >> bottom_element_shift);

	has_zero_tail =
		!has_nonzero_fractional_part &&
		(integer_value->data[bottom_element_index] & top_element_mask) == 0;

	for (i = 0; i != bottom_element_index; ++i)
	{
		has_zero_tail &= integer_value->data[i] == 0;
	}

	return assemble_floating_point_value(mantissa, exponent, is_negative, has_zero_tail, result);
}

// Accumulates the decimal digits in [first_digit, last_digit) into the result
// high precision integer.  This function assumes that no overflow will occur.
static void accumulate_decimal_digits_into_big_integer(
	const uint8_t *first_digit,
	const uint8_t *last_digit,
	big_integer   *result)
{
	// We accumulate nine digit chunks, transforming the base ten string into
	// base one billion on the fly, allowing us to reduce the number of high
	// precision multiplication and addition operations by 8/9.
	uint32_t      accumulator;
	uint32_t      accumulator_count;
	const uint8_t *it;

	big_integer_clear(result);
	accumulator = 0;
	accumulator_count = 0;
	for (it = first_digit; it != last_digit; ++it)
	{
		if (accumulator_count == 9)
		{
			big_integer_multiply_by_uint32(result, 1000 * 1000 * 1000);
			big_integer_add(result, accumulator);

			accumulator = 0;
			accumulator_count = 0;
		}

		accumulator *= 10;
		accumulator += *it;
		++accumulator_count;
	}

	if (accumulator_count != 0)
	{
		big_integer_multiply_by_power_of_ten(result, accumulator_count);
		big_integer_add(result, accumulator);
	}
}

// The core floating point string parser for decimal strings.  After a subject
// string is parsed and converted into a floating_point_string object, if the
// subject string was determined to be a decimal string, the object is passed to
// this function.  This function converts the decimal real value to floating
// point.
__forceinline static SLD_STATUS convert_decimal_string_to_floating_type_common(
	const floating_point_string *data,
	floating_traits             *result)
{
	uint32_t      required_bits_of_precision;
	uint32_t      positive_exponent;
	uint32_t      integer_digits_present;
	uint32_t      integer_digits_missing;
	const uint8_t *integer_first;
	const uint8_t *integer_last;
	const uint8_t *fractional_first;
	const uint8_t *fractional_last;
	uint32_t      fractional_digits_present;
	big_integer   integer_value;
	uint32_t      integer_bits_of_precision;
	big_integer   fractional_numerator;
	uint32_t      fractional_denominator_exponent;
	big_integer   fractional_denominator;
	uint32_t      fractional_numerator_bits;
	uint32_t      fractional_denominator_bits;
	uint32_t      fractional_shift;
	uint32_t      required_fractional_bits_of_precision;
	uint32_t      remaining_bits_of_precision_required;
	uint32_t      fractional_exponent;
	uint64_t      fractional_mantissa;
	bool          has_zero_tail;
	uint32_t      fractional_mantissa_bits;
	uint32_t      integer_mantissa_low;
	uint32_t      integer_mantissa_high;
	uint64_t      integer_mantissa;
	uint64_t      complete_mantissa;
	int32_t       final_exponent;

	// To generate an N bit mantissa we require N + 1 bits of precision.  The
	// extra bit is used to correctly round the mantissa (if there are fewer bits
	// than this available, then that's totally okay; in that case we use what we
	// have and we don't need to round).
	required_bits_of_precision = FLOATING_TRAITS_MANTISSA_BITS + 1;

	// The input is of the form 0.mantissa x 10^exponent, where 'mantissa' are
	// the decimal digits of the mantissa and 'exponent' is the decimal exponent.
	// We decompose the mantissa into two parts: an integer part and a fractional
	// part.  If the exponent is positive, then the integer part consists of the
	// first 'exponent' digits, or all present digits if there are fewer digits.
	// If the exponent is zero or negative, then the integer part is empty.  In
	// either case, the remaining digits form the fractional part of the mantissa.
	positive_exponent = (uint32_t)__max(0, data->exponent);
	integer_digits_present = __min(positive_exponent, data->mantissa_count);
	integer_digits_missing = positive_exponent - integer_digits_present;
	integer_first = data->mantissa;
	integer_last = data->mantissa + integer_digits_present;

	fractional_first = integer_last;
	fractional_last = data->mantissa + data->mantissa_count;
	fractional_digits_present = (uint32_t)(fractional_last - fractional_first);

	// First, we accumulate the integer part of the mantissa into a big_integer:
	accumulate_decimal_digits_into_big_integer(integer_first, integer_last, &integer_value);

	if (integer_digits_missing > 0)
	{
		if (!big_integer_multiply_by_power_of_ten(&integer_value, integer_digits_missing))
		{
			assemble_floating_point_infinity(data->is_negative, &result->value);
			return SLD_OVERFLOW;
		}
	}

	// At this point, the integer_value contains the value of the integer part
	// of the mantissa.  If either [1] this number has more than the required
	// number of bits of precision or [2] the mantissa has no fractional part,
	// then we can assemble the result immediately:
	integer_bits_of_precision = big_integer_bit_scan_reverse(&integer_value);
	if (integer_bits_of_precision >= required_bits_of_precision ||
		fractional_digits_present == 0)
	{
		return assemble_floating_point_value_from_big_integer(
			&integer_value,
			integer_bits_of_precision,
			data->is_negative,
			(bool)(fractional_digits_present != 0),
			result);
	}

	// Otherwise, we did not get enough bits of precision from the integer part,
	// and the mantissa has a fractional part.  We parse the fractional part of
	// the mantsisa to obtain more bits of precision.  To do this, we convert
	// the fractional part into an actual fraction N/M, where the numerator N is
	// computed from the digits of the fractional part, and the denominator M is
	// computed as the power of 10 such that N/M is equal to the value of the
	// fractional part of the mantissa.
	accumulate_decimal_digits_into_big_integer(fractional_first, fractional_last, &fractional_numerator);

	fractional_denominator_exponent = data->exponent < 0
		? fractional_digits_present + (uint32_t)(-data->exponent)
		: fractional_digits_present;

	big_integer_assign_uint32(&fractional_denominator, 1);
	if (!big_integer_multiply_by_power_of_ten(&fractional_denominator, fractional_denominator_exponent))
	{
		// If there were any digits in the integer part, it is impossible to
		// underflow (because the exponent cannot possibly be small enough),
		// so if we underflow here it is a true underflow and we return zero.
		assemble_floating_point_zero(data->is_negative, &result->value);
		return SLD_UNDERFLOW;
	}

	// Because we are using only the fractional part of the mantissa here, the
	// numerator is guaranteed to be smaller than the denominator.  We normalize
	// the fraction such that the most significant bit of the numerator is in
	// the same position as the most significant bit in the denominator.  This
	// ensures that when we later shift the numerator N bits to the left, we
	// will produce N bits of precision.
	fractional_numerator_bits = big_integer_bit_scan_reverse(&fractional_numerator);
	fractional_denominator_bits = big_integer_bit_scan_reverse(&fractional_denominator);

	fractional_shift = fractional_denominator_bits > fractional_numerator_bits
		? fractional_denominator_bits - fractional_numerator_bits
		: 0;

	if (fractional_shift > 0)
	{
		big_integer_shift_left(&fractional_numerator, fractional_shift);
	}

	required_fractional_bits_of_precision =
		required_bits_of_precision -
		integer_bits_of_precision;

	remaining_bits_of_precision_required = required_fractional_bits_of_precision;
	if (integer_bits_of_precision > 0)
	{
		// If the fractional part of the mantissa provides no bits of precision
		// and cannot affect rounding, we can just take whatever bits we got from
		// the integer part of the mantissa.  This is the case for numbers like
		// 5.0000000000000000000001, where the significant digits of the fractional
		// part start so far to the right that they do not affect the floating
		// point representation.
		//
		// If the fractional shift is exactly equal to the number of bits of
		// precision that we require, then no fractional bits will be part of the
		// result, but the result may affect rounding.  This is e.g. the case for
		// large, odd integers with a fractional part greater than or equal to .5.
		// Thus, we need to do the division to correctl round the result.
		if (fractional_shift > remaining_bits_of_precision_required)
		{
			return assemble_floating_point_value_from_big_integer(
				&integer_value,
				integer_bits_of_precision,
				data->is_negative,
				(bool)(fractional_digits_present != 0),
				result);
		}

		remaining_bits_of_precision_required -= fractional_shift;
	}

	// If there was no integer part of the mantissa, we will need to compute the
	// exponent from the fractional part.  The fractional exponent is the power
	// of two by which we must multiply the fractional part to move it into the
	// range [1.0, 2.0).  This will either be the same as the shift we computed
	// earlier, or one greater than that shift:
	fractional_exponent = big_integer_less_than(&fractional_numerator, &fractional_denominator)
		? fractional_shift + 1
		: fractional_shift;

	big_integer_shift_left(&fractional_numerator, remaining_bits_of_precision_required);
	fractional_mantissa = big_integer_divide(&fractional_numerator, &fractional_denominator);

	has_zero_tail = fractional_numerator.used == 0;

	// We may have produced more bits of precision than were required.  Check,
	// and remove any "extra" bits:
	fractional_mantissa_bits = big_integer_bit_scan_reverse64(fractional_mantissa);
	if (fractional_mantissa_bits > required_fractional_bits_of_precision)
	{
		uint32_t shift;

		shift = fractional_mantissa_bits - required_fractional_bits_of_precision;
		has_zero_tail = has_zero_tail && (fractional_mantissa & ((UINT64_C(1) << shift) - 1)) == 0;
		fractional_mantissa >>= shift;
	}

	// Compose the mantissa from the integer and fractional parts:
	integer_mantissa_low = integer_value.used > 0 ? integer_value.data[0] : 0;
	integer_mantissa_high = integer_value.used > 1 ? integer_value.data[1] : 0;
	integer_mantissa =
		integer_mantissa_low +
		((uint64_t)integer_mantissa_high << 32);

	complete_mantissa =
		(integer_mantissa << required_fractional_bits_of_precision) +
		fractional_mantissa;

	// Compute the final exponent:
	// * If the mantissa had an integer part, then the exponent is one less than
	//   the number of bits we obtained from the integer part.  (It's one less
	//   because we are converting to the form 1.11111, with one 1 to the left
	//   of the decimal point.)
	// * If the mantissa had no integer part, then the exponent is the fractional
	//   exponent that we computed.
	// Then, in both cases, we subtract an additional one from the exponent, to
	// account for the fact that we've generated an extra bit of precision, for
	// use in rounding.
	final_exponent = integer_bits_of_precision > 0
		? integer_bits_of_precision - 2
		: -(int32_t)fractional_exponent - 1;

	return assemble_floating_point_value(complete_mantissa, final_exponent, data->is_negative, has_zero_tail, result);
}

__forceinline static SLD_STATUS convert_decimal_string_to_floating_type(
	const floating_point_string *data,
	double                      *result)
{
	return convert_decimal_string_to_floating_type_common(data, (floating_traits *)result);
}

__forceinline static SLD_STATUS convert_hexadecimal_string_to_floating_type_common(
	const floating_point_string *data,
	floating_traits             *result)
{
	uint64_t      mantissa;
	int32_t       exponent;
	const uint8_t *mantissa_last;
	const uint8_t *mantissa_it;
	bool          has_zero_tail;

	mantissa = 0;
	exponent = data->exponent + FLOATING_TRAITS_MANTISSA_BITS - 1;

	// Accumulate bits into the mantissa buffer
	mantissa_last = data->mantissa + data->mantissa_count;
	mantissa_it = data->mantissa;
	while (mantissa_it != mantissa_last && mantissa <= FLOATING_TRAITS_NORMAL_MANTISSA_MASK)
	{
		mantissa *= 16;
		mantissa += *mantissa_it++;
		exponent -= 4; // The exponent is in binary; log2(16) == 4
	}

	has_zero_tail = true;
	while (mantissa_it != mantissa_last && has_zero_tail)
	{
		has_zero_tail = has_zero_tail && *mantissa_it++ == 0;
	}

	return assemble_floating_point_value(mantissa, exponent, data->is_negative, has_zero_tail, result);
}

__forceinline static SLD_STATUS convert_hexadecimal_string_to_floating_type(
	const floating_point_string *data,
	double                      *result)
{
	return convert_hexadecimal_string_to_floating_type_common(data, (floating_traits *)result);
}

static bool parse_next_characters_from_source(
	const TCHAR               *uppercase,
	const TCHAR               *lowercase,
	const size_t              count,
	TCHAR                     *c,
	c_string_character_source *source)
{
	size_t i;

	for (i = 0; i != count; ++i)
	{
		if (*c != uppercase[i] && *c != lowercase[i])
		{
			return false;
		}

		*c = c_string_character_source_get(source);
	}

	return true;
}

__forceinline static bool parse_floating_point_possible_nan_is_snan(
	TCHAR                     *c,
	c_string_character_source *source)
{
	static const TCHAR uppercase[] = { 'S', 'N', 'A', 'N', ')' };
	static const TCHAR lowercase[] = { 's', 'n', 'a', 'n', ')' };

	return parse_next_characters_from_source(uppercase, lowercase, _countof(uppercase), c, source);
}

__forceinline static bool parse_floating_point_possible_nan_is_ind(
	TCHAR                     *c,
	c_string_character_source *source)
{
	static const TCHAR uppercase[] = { 'I', 'N', 'D', ')' };
	static const TCHAR lowercase[] = { 'i', 'n', 'd', ')' };

	return parse_next_characters_from_source(uppercase, lowercase, _countof(uppercase), c, source);
}

__forceinline static floating_point_parse_result parse_floating_point_possible_infinity(
	TCHAR                     *c,
	c_string_character_source *source,
	const TCHAR               *stored_state)
{
	static const TCHAR inf_uppercase[]   = { 'I', 'N', 'F' };
	static const TCHAR inf_lowercase[]   = { 'i', 'n', 'f' };
	static const TCHAR inity_uppercase[] = { 'I', 'N', 'I', 'T', 'Y' };
	static const TCHAR inity_lowercase[] = { 'i', 'n', 'i', 't', 'y' };

	if (!parse_next_characters_from_source(inf_uppercase, inf_lowercase, _countof(inf_uppercase), c, source))
	{
		c_string_character_source_unget(source, *c);
		*c = '\0';
		c_string_character_source_restore_state(source, stored_state);
		return FLOATING_POINT_PARSE_RESULT_NO_DIGITS;
	}

	c_string_character_source_unget(source, *c);
	stored_state = c_string_character_source_save_state(source);
	*c = c_string_character_source_get(source);

	if (!parse_next_characters_from_source(inity_uppercase, inity_lowercase, _countof(inity_uppercase), c, source))
	{
		c_string_character_source_unget(source, *c);
		*c = '\0';
		c_string_character_source_restore_state(source, stored_state);
		return FLOATING_POINT_PARSE_RESULT_INFINITY;
	}

	c_string_character_source_unget(source, *c);
	return FLOATING_POINT_PARSE_RESULT_INFINITY;
}

__forceinline static floating_point_parse_result parse_floating_point_possible_nan(
	TCHAR                     *c,
	c_string_character_source *source,
	const TCHAR               *stored_state)
{
	static const TCHAR uppercase[] = { 'N', 'A', 'N' };
	static const TCHAR lowercase[] = { 'n', 'a', 'n' };

	if (!parse_next_characters_from_source(uppercase, lowercase, _countof(uppercase), c, source))
	{
		c_string_character_source_unget(source, *c);
		*c = '\0';
		c_string_character_source_restore_state(source, stored_state);
		return FLOATING_POINT_PARSE_RESULT_NO_DIGITS;
	}

	c_string_character_source_unget(source, *c);
	stored_state = c_string_character_source_save_state(source);
	*c = c_string_character_source_get(source);

	if (*c != '(')
	{
		c_string_character_source_unget(source, *c);
		*c = '\0';
		c_string_character_source_restore_state(source, stored_state);
		return FLOATING_POINT_PARSE_RESULT_QNAN;
	}

	*c = c_string_character_source_get(source); // Advance past the left parenthesis

	// After we've parsed a left parenthesis, test to see whether the parenthesized
	// string represents a signaling NaN "(SNAN)" or an indeterminate "(IND)".  If
	// so, we return the corresponding kind of NaN:
	if (parse_floating_point_possible_nan_is_snan(c, source))
	{
		c_string_character_source_unget(source, *c);
		return FLOATING_POINT_PARSE_RESULT_SNAN;
	}

	if (parse_floating_point_possible_nan_is_ind(c, source))
	{
		c_string_character_source_unget(source, *c);
		return FLOATING_POINT_PARSE_RESULT_INDETERMINATE;
	}

	// Otherwise, we didn't match one of the two special parenthesized strings.
	// Keep eating chracters until we come across the right parenthesis or the
	// end of the character sequence:
	while (*c != ')' && *c != '\0')
	{
		if (!is_digit_or_nondigit(*c))
		{
			c_string_character_source_unget(source, *c);
			*c = '\0';
			c_string_character_source_restore_state(source, stored_state);
			return FLOATING_POINT_PARSE_RESULT_QNAN;
		}

		*c = c_string_character_source_get(source);
	}

	if (*c != ')')
	{
		c_string_character_source_unget(source, *c);
		*c = '\0';
		c_string_character_source_restore_state(source, stored_state);
		return FLOATING_POINT_PARSE_RESULT_QNAN;
	}

	return FLOATING_POINT_PARSE_RESULT_QNAN;
}

__forceinline static floating_point_parse_result parse_floating_point_from_source(
	c_string_character_source *source,
	floating_point_string     *fp_string)
{
	const TCHAR   *stored_state;
	TCHAR         c;
	bool          is_hexadecimal;
	uint8_t       *mantissa_first;
	const uint8_t *mantissa_last;
	uint8_t       *mantissa_it;
	int           exponent_adjustment;
	bool          found_digits;
	int           radix_point;
	bool          has_exponent;
	int           exponent;
	int           exponent_adjustment_multiplier;

	if (!c_string_character_source_validate(source))
	{
		return FLOATING_POINT_PARSE_RESULT_NO_DIGITS;
	}

	stored_state = c_string_character_source_save_state(source);
	c = c_string_character_source_get(source);

	// Skip past any leading whitespace:
	while (is_space(c))
	{
		c = c_string_character_source_get(source);
	}

	// Check for the optional plus or minus sign:
	fp_string->is_negative = c == '-';
	if (c == '-' || c == '+')
	{
		c = c_string_character_source_get(source);
	}

	// Handle special cases "INF" and "INFINITY" (these are the only accepted
	// character sequences that start with 'I'):
	if (c == 'I' || c == 'i')
	{
		return parse_floating_point_possible_infinity(&c, source, stored_state);
	}

	// Handle special cases "NAN" and "NAN(...)" (these are the only accepted
	// character sequences that start with 'N'):
	if (c == 'N' || c == 'n')
	{
		return parse_floating_point_possible_nan(&c, source, stored_state);
	}

	// Check for optional "0x" or "0X" hexadecimal base prefix:
	is_hexadecimal = false;
	if (c == '0')
	{
		const TCHAR *next_stored_state;
		TCHAR       next_c;

		next_stored_state = c_string_character_source_save_state(source);

		next_c = c_string_character_source_get(source);
		if (next_c == 'x' || next_c == 'X')
		{
			is_hexadecimal = true;
			c = c_string_character_source_get(source);

			// If we match the hexadecimal base prefix we update the state to
			// reflect that we consumed the leading zero to handle the case
			// where a valid mantissa does not follow the base prefix.  In this
			// case, the "0x" string is treated as a decimal zero subject ("0")
			// followed by a final string starting with the "x".
			stored_state = next_stored_state;
		}
		else
		{
			c_string_character_source_unget(source, next_c);
		}
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
	while (c == '0')
	{
		found_digits = true;
		c = c_string_character_source_get(source);
	}

	// Scan the integer part of the mantissa:
	for (; ; c = c_string_character_source_get(source))
	{
		unsigned int max_digit_value;
		unsigned int digit_value;

		max_digit_value = is_hexadecimal ? 0xfu : 9u;

		digit_value = parse_digit(c);
		if (digit_value > max_digit_value)
		{
			break;
		}

		found_digits = true;
		if (mantissa_it != mantissa_last)
		{
			*mantissa_it++ = (uint8_t)digit_value;
		}

		++exponent_adjustment;
	}

	// If a radix point is present, scan the fractional part of the mantissa:
	if (!GetLocaleInfo(GetThreadLocale(), LOCALE_SDECIMAL, (TCHAR *)&radix_point, 2))
		radix_point = '.';
	if (c == (TCHAR)radix_point)
	{
		c = c_string_character_source_get(source);

		// If we haven't yet scanned any nonzero digits, continue skipping over
		// zeroes, updating the exponent adjustment to account for the zeroes
		// we are skipping:
		if (mantissa_it == mantissa_first)
		{
			while (c == '0')
			{
				found_digits = true;
				--exponent_adjustment;
				c = c_string_character_source_get(source);
			}
		}

		for (; ; c = c_string_character_source_get(source))
		{
			unsigned int max_digit_value;
			unsigned int digit_value;

			max_digit_value = is_hexadecimal ? 0xfu : 9u;

			digit_value = parse_digit(c);
			if (digit_value > max_digit_value)
				break;

			found_digits = true;
			if (mantissa_it != mantissa_last)
			{
				*mantissa_it++ = (uint8_t)digit_value;
			}
		}
	}

	if (!found_digits)
	{
		// We failed to parse any digits, so attempt to restore back to the last
		// good terminal state.  This may fail if we are reading from a stream,
		// we read a hexadecimal base prefix ("0x"), but we did not find any digits
		// following the base prefix.
		c_string_character_source_unget(source, c);
		c = '\0';
		c_string_character_source_restore_state(source, stored_state);

		// If a hexadecimal base prefix was present ("0x"), then the string is a
		// valid input:  the "0" is the subject sequence and the "x" is the first
		// character of the final string.  Otherwise, the string is not a valid
		// input.
		if (is_hexadecimal)
		{
			return FLOATING_POINT_PARSE_RESULT_ZERO;
		}
		else
		{
			return FLOATING_POINT_PARSE_RESULT_NO_DIGITS;
		}
	}

	c_string_character_source_unget(source, c);
	stored_state = c_string_character_source_save_state(source);
	c = c_string_character_source_get(source);

	// Check for the optional 'e' or 'p' exponent introducer:
	has_exponent = false;
	switch (c)
	{
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

		c = c_string_character_source_get(source); // Skip past exponent introducer character

		// Check for the optional plus or minus sign:
		exponent_is_negative = c == '-';
		if (c == '+' || c == '-')
		{
			c = c_string_character_source_get(source);
		}

		has_exponent_digits = false;

		while (c == '0')
		{
			has_exponent_digits = true;
			c = c_string_character_source_get(source);
		}

		for (; ; c = c_string_character_source_get(source))
		{
			unsigned int digit_value;

			digit_value = parse_digit(c);
			if (digit_value >= 10)
			{
				break;
			}

			has_exponent_digits = true;
			exponent = exponent * 10 + digit_value;
			if (exponent > MAXIMUM_TEMPORARY_DECIMAL_EXPONENT)
			{
				exponent = MAXIMUM_TEMPORARY_DECIMAL_EXPONENT + 1;
				break;
			}
		}

		// If the exponent is too large, skip over the remaining exponent digits
		// so we can correctly update the end pointer:
		while (parse_digit(c) < 10)
		{
			c = c_string_character_source_get(source);
		}

		if (exponent_is_negative)
		{
			exponent = -exponent;
		}

		// If no exponent digits were present, attempt to restore the last good
		// terminal state.
		if (!has_exponent_digits)
		{
			c_string_character_source_unget(source, c);
			c = '\0';
			c_string_character_source_restore_state(source, stored_state);

			// The call to restore_state will have ungotten the exponent
			// introducer.  Re-get this character, to restore us to the
			// state we had before we entered the exponent parsing block.
			c = c_string_character_source_get(source);
		}
	}

	// Unget the last character that we read that terminated input.  After this
	// point, we must not use the source, c, or stored_state.
	c_string_character_source_unget(source, c);

	// Remove trailing zeroes from mantissa:
	while (mantissa_it != mantissa_first && *(mantissa_it - 1) == 0)
	{
		--mantissa_it;
	}

	// If the mantissa buffer is empty, the mantissa was composed of all zeroes
	// (so the mantissa is 0).  All such strings have the value zero, regardless
	// what the exponent is (because 0 x b^n == 0 for all b and n).  We can return
	// now.  Note that we defer this check until after we scan the exponent, so
	// that we can correctly update end_ptr to point past the end of the exponent.
	if (mantissa_it == mantissa_first)
	{
		return FLOATING_POINT_PARSE_RESULT_ZERO;
	}

	// Before we adjust the exponent, handle the case where we detected a wildly
	// out of range exponent during parsing and clamped the value:
	if (exponent > MAXIMUM_TEMPORARY_DECIMAL_EXPONENT)
	{
		return FLOATING_POINT_PARSE_RESULT_OVERFLOW;
	}

	if (exponent < MINIMUM_TEMPORARY_DECIMAL_EXPONENT)
	{
		return FLOATING_POINT_PARSE_RESULT_UNDERFLOW;
	}

	// In hexadecimal floating constants, the exponent is a base 2 exponent.  The
	// exponent adjustment computed during parsing has the same base as the
	// mantissa (so, 16 for hexadecimal floating constants).  We therefore need to
	// scale the base 16 multiplier to base 2 by multiplying by log2(16):
	exponent_adjustment_multiplier = is_hexadecimal ? 4 : 1;

	exponent += exponent_adjustment * exponent_adjustment_multiplier;

	// Verify that after adjustment the exponent isn't wildly out of range (if
	// it is, it isn't representable in any supported floating point format).
	if (exponent > MAXIMUM_TEMPORARY_DECIMAL_EXPONENT)
	{
		return FLOATING_POINT_PARSE_RESULT_OVERFLOW;
	}

	if (exponent < MINIMUM_TEMPORARY_DECIMAL_EXPONENT)
	{
		return FLOATING_POINT_PARSE_RESULT_UNDERFLOW;
	}

	fp_string->exponent = exponent;
	fp_string->mantissa_count = (uint32_t)(mantissa_it - mantissa_first);

	return is_hexadecimal
		? FLOATING_POINT_PARSE_RESULT_HEXADECIMAL_DIGITS
		: FLOATING_POINT_PARSE_RESULT_DECIMAL_DIGITS;
}

__forceinline static SLD_STATUS parse_floating_point_write_result(
	const floating_point_parse_result parse_result,
	const floating_point_string       *fp_string,
	double                            *result)
{
	switch (parse_result)
	{
	case FLOATING_POINT_PARSE_RESULT_DECIMAL_DIGITS:     return convert_decimal_string_to_floating_type    (fp_string, result);
	case FLOATING_POINT_PARSE_RESULT_HEXADECIMAL_DIGITS: return convert_hexadecimal_string_to_floating_type(fp_string, result);

	case FLOATING_POINT_PARSE_RESULT_ZERO:          assemble_floating_point_zero    (fp_string->is_negative, result); return SLD_OK;
	case FLOATING_POINT_PARSE_RESULT_INFINITY:      assemble_floating_point_infinity(fp_string->is_negative, result); return SLD_OK;
	case FLOATING_POINT_PARSE_RESULT_QNAN:          assemble_floating_point_qnan    (fp_string->is_negative, result); return SLD_OK;
	case FLOATING_POINT_PARSE_RESULT_SNAN:          assemble_floating_point_snan    (fp_string->is_negative, result); return SLD_OK;
	case FLOATING_POINT_PARSE_RESULT_INDETERMINATE: assemble_floating_point_ind     (                        result); return SLD_OK;

	case FLOATING_POINT_PARSE_RESULT_NO_DIGITS:     assemble_floating_point_zero    (false,                  result); return SLD_NODIGITS;
	case FLOATING_POINT_PARSE_RESULT_UNDERFLOW:     assemble_floating_point_zero    (fp_string->is_negative, result); return SLD_UNDERFLOW;
	case FLOATING_POINT_PARSE_RESULT_OVERFLOW:      assemble_floating_point_infinity(fp_string->is_negative, result); return SLD_OVERFLOW;

	default:
		// Unreachable
		_ASSERTE(false);
		return SLD_NODIGITS;
	}
}

__forceinline static SLD_STATUS parse_floating_point(
	c_string_character_source *source,
	double                    *result)
{
	floating_point_string       fp_string;
	floating_point_parse_result parse_result;

	_VALIDATE_RETURN(result != NULL, EINVAL, SLD_NODIGITS);

	// PERFORMANCE NOTE:  fp_string is intentionally left uninitialized.  Zero-
	// initialization is quite expensive and is unnecessary.  The benefit of not
	// zero-initializing is greatest for short inputs.

	parse_result = parse_floating_point_from_source(source, &fp_string);

	return parse_floating_point_write_result(parse_result, &fp_string, result);
}

//
// Definitions of the functions that convert strings to floating point numbers.
// These functions, the strtox and wcstox functions for floating point types,
// recogize an optional sequence of tabs and space, then an optional sign, then
// a sequence of digits optionally containing a decimal point, then an optional
// e or E followed by an optionally signed integer, and converts all of this to
// a floating point number.  The first unrecognized character ends the string.
// The *end_ptr pointer is updated to point to the last character of the string
//
double __cdecl _tcstod(
	const TCHAR *string,
	TCHAR       **end_ptr)
{
	double                    result;
	c_string_character_source source;
	SLD_STATUS                status;

	if (end_ptr)
	{
		*end_ptr = (TCHAR *)string;
	}

	_VALIDATE_RETURN(string != NULL, EINVAL, 0.0);

	result = 0;
	c_string_character_source_ctor(&source, string, (const TCHAR **)end_ptr);
	status = parse_floating_point(&source, &result);
	c_string_character_source_dtor(&source);

	if (status == SLD_OVERFLOW || status == SLD_UNDERFLOW)
	{
		errno = ERANGE;
	}

	return result;
}
