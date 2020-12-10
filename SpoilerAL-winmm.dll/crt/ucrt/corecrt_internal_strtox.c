#include "corecrt_internal_strtox.h"

#if defined(_MSC_VER) && _MSC_VER < 1600
typedef unsigned __int16 uint16_t;
#endif

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// String-to-Integer Conversion
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
unsigned int __fastcall strtox_parse_digit(const unsigned int c)
{
	switch (c)
	{
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		return c - '0';
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
		return c - 'A' + 10;
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
		return c - 'a' + 10;
	default:
		return -1;
	}
}

// The digit and nondigit categories include 0-9, a-z, A-Z, and _.  They are not
// locale-dependent, so we do not call isalnum (which uses the current locale to
// test for alphabetic characters).
bool __fastcall strtox_is_digit_or_nondigit(const unsigned int c)
{
	switch (c)
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

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// String-to-Floating-Point Conversion
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

// Stores a positive or negative zero into the result object
__forceinline static void assemble_floating_point_zero(const bool is_negative, double *result)
{
	/*
		floating_traits *components;

		components = (floating_traits *)result;
		components->sign     = is_negative ? 1 : 0;
		components->exponent = 0;
		components->mantissa = 0;
	*/
	*(uint64_t *)result = is_negative ?
		MAKE_FLOATING_TRAITS_UI64(1, 0, 0) :
		MAKE_FLOATING_TRAITS_UI64(0, 0, 0);
}

// Stores a positive or negative infinity into the result object
__forceinline static void assemble_floating_point_infinity(const bool is_negative, double *result)
{
	/*
		floating_traits *components;

		components = (floating_traits *)result;
		components->sign     = is_negative ? 1 : 0;
		components->exponent = FLOATING_TRAITS_EXPONENT_MASK;
		components->mantissa = 0;
	*/
	*(uint64_t *)result = is_negative ?
		MAKE_FLOATING_TRAITS_UI64(1, FLOATING_TRAITS_EXPONENT_MASK, 0) :
		MAKE_FLOATING_TRAITS_UI64(0, FLOATING_TRAITS_EXPONENT_MASK, 0);
}

// Stores a positive or negative quiet NaN into the result object
__forceinline static void assemble_floating_point_qnan(const bool is_negative, double *result)
{
	/*
		floating_traits *components;

		components = (floating_traits *)result;
		components->sign     = is_negative ? 1 : 0;
		components->exponent = FLOATING_TRAITS_EXPONENT_MASK;
		components->mantissa = FLOATING_TRAITS_DENORMAL_MANTISSA_MASK;
	*/
	*(uint64_t *)result = is_negative ?
		MAKE_FLOATING_TRAITS_UI64(1, FLOATING_TRAITS_EXPONENT_MASK, FLOATING_TRAITS_DENORMAL_MANTISSA_MASK) :
		MAKE_FLOATING_TRAITS_UI64(0, FLOATING_TRAITS_EXPONENT_MASK, FLOATING_TRAITS_DENORMAL_MANTISSA_MASK);
}

// Stores a positive or negative signaling NaN into the result object
__forceinline static void assemble_floating_point_snan(const bool is_negative, double *result)
{
	/*
		floating_traits *components;

		components = (floating_traits *)result;
		components->sign     = is_negative ? 1 : 0;
		components->exponent = FLOATING_TRAITS_EXPONENT_MASK;
		components->mantissa = 1;
	*/
	*(uint64_t *)result = is_negative ?
		MAKE_FLOATING_TRAITS_UI64(1, FLOATING_TRAITS_EXPONENT_MASK, 1) :
		MAKE_FLOATING_TRAITS_UI64(0, FLOATING_TRAITS_EXPONENT_MASK, 1);
}

// Stores an indeterminate into the result object (the indeterminate is "negative")
__forceinline static void assemble_floating_point_ind(double *result)
{
	/*
		floating_traits *components;

		components = (floating_traits *)result;
		components->sign     = 1;
		components->exponent = FLOATING_TRAITS_EXPONENT_MASK;
		components->mantissa = FLOATING_TRAITS_SPECIAL_NAN_MANTISSA_MASK;
	*/
	*(uint64_t *)result =
		MAKE_FLOATING_TRAITS_UI64(1, FLOATING_TRAITS_EXPONENT_MASK, FLOATING_TRAITS_SPECIAL_NAN_MANTISSA_MASK);
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
	bool     is_exactly_representable;
#ifdef _M_IX86
	uint16_t rounding;
#endif

	// If there are no insignificant set bits, the value is exactly representable
	// and should not be rounded in any rounding mode:
	is_exactly_representable = !round_bit && !has_tail_bits;
	if (is_exactly_representable)
		return false;

	// If there are insignificant set bits, we need to round according to the
	// current rounding mode.  For RC_NEAR, we need to handle two cases:
	// we round up if either [1] the value is slightly greater than the midpoint
	// between two exactly representable values or [2] the value is exactly the
	// midpoint between two exactly representable values and the greater of the
	// two is even (this is "round-to-even").
#ifndef _M_IX86
	switch (_controlfp(0, 0) & MCW_RC)
	{
	case RC_NEAR: return round_bit && (has_tail_bits || lsb_bit);
	case RC_DOWN: return is_negative;
	case RC_UP:   return !is_negative;
#if defined(_MSC_VER) && _MSC_VER >= 1200
	case RC_CHOP: return false;
	default:      __assume(0);
#else
	default:      return false;
#endif
	}
#else
	__asm   fstcw   word ptr [rounding]
	switch (rounding & 0x0C00)
	{
	case 0x0000: return round_bit && (has_tail_bits || lsb_bit);
	case 0x0400: return is_negative;
	case 0x0800: return !is_negative;
#if defined(_MSC_VER) && _MSC_VER >= 1200
	case 0x0C00: return false;
	default:     __assume(0);
#else
	default:     return false;
#endif
	}
#endif

/*
	_ASSERTE(("unexpected rounding mode", false));
	return false;
*/
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
		return 0;

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

static SLD_STATUS __fastcall assemble_floating_point_value(
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
				// We add one to the denormal_mantissa_shift to account for the
				// hidden mantissa bit (we subtracted one to account for this bit
				// when we computed the denormal_mantissa_shift above).
				exponent =
					initial_exponent -
					(denormal_mantissa_shift + 1) -
					normal_mantissa_shift;
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
static SLD_STATUS __fastcall assemble_floating_point_value_from_big_integer(
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

		exponent = base_exponent + bottom_element_index * 32;

		mantissa =
			integer_value->data[bottom_element_index] +
			((uint64_t)integer_value->data[middle_element_index] << 32);

		has_zero_tail = !has_nonzero_fractional_part;
		if (has_zero_tail)
		{
			uint32_t i;

			for (i = 0; i != bottom_element_index; ++i)
			{
				if (integer_value->data[i])
				{
					has_zero_tail = false;
					break;
				}
			}
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

	if (has_zero_tail)
	{
		uint32_t i;

		for (i = 0; i != bottom_element_index; ++i)
		{
			if (integer_value->data[i])
			{
				has_zero_tail = false;
				break;
			}
		}
	}

	return assemble_floating_point_value(mantissa, exponent, is_negative, has_zero_tail, result);
}

// Accumulates the decimal digits in [first_digit, last_digit) into the result
// high precision integer.  This function assumes that no overflow will occur.
static void __fastcall accumulate_decimal_digits_into_big_integer(
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
			big_integer_multiply_by_uint32(result, 1000000000);
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
		big_integer_shift_left(&fractional_numerator, fractional_shift);

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
			return assemble_floating_point_value_from_big_integer(
				&integer_value,
				integer_bits_of_precision,
				data->is_negative,
				(bool)(fractional_digits_present != 0),
				result);

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
		has_zero_tail &= (fractional_mantissa & ((UINT64_C(1) << shift) - 1)) == 0;
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
	while (mantissa_it != mantissa_last)
	{
		if (*mantissa_it++)
		{
			has_zero_tail = false;
			break;
		}
	}

	return assemble_floating_point_value(mantissa, exponent, data->is_negative, has_zero_tail, result);
}

__forceinline static SLD_STATUS convert_hexadecimal_string_to_floating_type(
	const floating_point_string *data,
	double                      *result)
{
	return convert_hexadecimal_string_to_floating_type_common(data, (floating_traits *)result);
}

SLD_STATUS __fastcall strtox_parse_floating_point_write_result(
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
