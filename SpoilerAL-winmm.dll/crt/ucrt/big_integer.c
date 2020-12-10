#include "big_integer.h"
#include <stdlib.h>
#ifndef _countof
#define _countof(_array) (sizeof(_array) / sizeof((_array)[0]))
#endif
#include <crtdbg.h>

#if defined(_MSC_VER) && _MSC_VER < 1600
typedef unsigned __int8  bool;
typedef unsigned __int8  uint8_t;
typedef unsigned __int16 uint16_t;
typedef __int32          int32_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#define false       0
#define true        1
#define UINT32_MAX  _UI32_MAX
#define UINT32_C(x) (x ## U)
#endif

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// A lightweight high precision integer type for use by the binary floating
// point <=> decimal string conversion functions.
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
bool __fastcall big_integer_less_than(const big_integer *lhs, const big_integer *rhs)
{
	uint32_t       i;
	const uint32_t *left;
	const uint32_t *right;

	if (lhs->used != rhs->used)
		return lhs->used < rhs->used;
	if (!(i = lhs->used))
		return false;
	left = lhs->data - 1;
	right = rhs->data - 1;
	do
		if (left[i] != right[i])
			return left[i] < right[i];
	while (--i);
	return false;
}

// Shifts the high precision integer x by n bits to the left.  Returns true if
// the left shift was successful; false if it overflowed.  When overflow occurs,
// the high precision integer is reset to zero.
bool __fastcall big_integer_shift_left(big_integer *x, const uint32_t n)
{
	uint32_t unit_shift;
	uint32_t bit_shift;
	uint32_t msb_bits;
	uint32_t lsb_bits;
	uint32_t lsb_mask;
	uint32_t msb_mask;
	bool     bit_shifts_into_next_unit;
	bool     unit_shift_will_overflow;
	bool     bit_shift_will_overflow;
	uint32_t max_destination_index;
	uint32_t destination_index;

	unit_shift = n / BIG_INTEGER_ELEMENT_BITS;
	bit_shift = n % BIG_INTEGER_ELEMENT_BITS;

	msb_bits = bit_shift;
	lsb_bits = BIG_INTEGER_ELEMENT_BITS - msb_bits;

	lsb_mask = (UINT32_C(1) << lsb_bits) - 1;
	msb_mask = ~lsb_mask;

	bit_shifts_into_next_unit = bit_shift > (BIG_INTEGER_ELEMENT_BITS - big_integer_bit_scan_reverse32(x->data[x->used - 1]));

	unit_shift_will_overflow = x->used + unit_shift > BIG_INTEGER_ELEMENT_COUNT;
	bit_shift_will_overflow =
		x->used + unit_shift == BIG_INTEGER_ELEMENT_COUNT &&
		bit_shifts_into_next_unit;

	if (unit_shift_will_overflow || bit_shift_will_overflow)
	{
		big_integer_clear(x);
		return false;
	}

	max_destination_index = __min(x->used + unit_shift, BIG_INTEGER_ELEMENT_COUNT - 1);

	for (destination_index = max_destination_index;
		destination_index != (uint32_t)-1 && destination_index >= unit_shift;
		--destination_index)
	{
		uint32_t upper_source_index;
		uint32_t lower_source_index;
		uint32_t upper_source;
		uint32_t lower_source;
		uint32_t shifted_upper_source;
		uint32_t shifted_lower_source;
		uint32_t combined_shifted_source;

		upper_source_index = destination_index - unit_shift;
		lower_source_index = destination_index - unit_shift - 1;

		upper_source = upper_source_index < x->used ? x->data[upper_source_index] : 0;
		lower_source = lower_source_index < x->used ? x->data[lower_source_index] : 0;

		shifted_upper_source = (upper_source & lsb_mask) << msb_bits;
		shifted_lower_source = (lower_source & msb_mask) >> lsb_bits;

		combined_shifted_source = shifted_upper_source | shifted_lower_source;

		x->data[destination_index] = combined_shifted_source;
	}

	memset(x->data, 0, unit_shift * sizeof(uint32_t));

	x->used = bit_shifts_into_next_unit
		? max_destination_index + 1
		: max_destination_index;

	return true;
}

// Adds a 32-bit value to the high-precision integer x.  Returns true if the
// addition was successful; false if it overflowed.  When overflow occurs, the
// high precision integer is reset to zero.
bool __fastcall big_integer_add(big_integer *x, const uint32_t value)
{
	uint32_t carry;

	if (carry = value)
	{
		if (x->used)
		{
			uint32_t i;

			i = 0;
			do
			{
				uint64_t uu;

				uu = (uint64_t)x->data[i] + carry;
				x->data[i] = (uint32_t)uu;
				if (!(carry = (uint32_t)(uu >> 32)))
					return true;
			} while (++i != x->used);
			if (i >= BIG_INTEGER_ELEMENT_COUNT)
			{
				big_integer_clear(x);
				return false;
			}
		}
		x->data[x->used] = carry;
		++x->used;
	}
	return true;
}

__forceinline static uint32_t add_multiply_carry(uint32_t *u_add, const uint32_t u_mul_1, const uint32_t u_mul_2, const uint32_t u_carry)
{
	uint64_t uu_res;

	uu_res = (uint64_t)u_mul_1 * u_mul_2 + *u_add + u_carry;
	*u_add = (uint32_t)uu_res;
	return ((uint32_t *)&uu_res)[1];
}

__forceinline static uint32_t multiply_core(uint32_t *multiplicand, const uint32_t multiplicand_count, const uint32_t multiplier)
{
	uint32_t carry;
	uint32_t i;

	carry = 0;
	for (i = 0; i != multiplicand_count; ++i)
	{
		uint64_t uu;

		uu = (uint64_t)multiplicand[i] * multiplier + carry;
		multiplicand[i] = (uint32_t)uu;
		carry = (uint32_t)(uu >> 32);
	}
	return carry;
}

// Multiplies the high precision multiplicand by a 32-bit multiplier.  Returns
// true if the multiplication was successful; false if it overflowed.  When
// overflow occurs, the multiplicand is reset to zero.
bool __fastcall big_integer_multiply_by_uint32(big_integer *multiplicand, const uint32_t multiplier)
{
	uint32_t carry;

	if (multiplier == 0)
	{
		big_integer_clear(multiplicand);
		return true;
	}

	if (multiplier == 1)
		return true;

	if (multiplicand->used == 0)
		return true;

	carry = multiply_core(multiplicand->data, multiplicand->used, multiplier);
	if (carry != 0)
	{
		if (multiplicand->used < BIG_INTEGER_ELEMENT_COUNT)
		{
			multiplicand->data[multiplicand->used] = carry;
			++multiplicand->used;
		}
		else
		{
			big_integer_clear(multiplicand);
			return false;
		}
	}

	return true;
}

// This high precision integer division implementation was translated from the
// implementation of System.Numerics.BigIntegerBuilder.Mul in the .NET Framework
// sources.  It multiplies the multiplicand by the multiplier and returns true
// if the multiplication was successful; false if it overflowed.  When overflow
// occurs, the multiplicand is reset to zero.
bool __fastcall big_integer_multiply(big_integer *multiplicand, const big_integer *multiplier)
{
	bool           multiplier_is_shorter;
	const uint32_t *rgu1;
	const uint32_t *rgu2;
	uint32_t       cu1;
	uint32_t       cu2;
	uint32_t       iu1, iu2;
    big_integer    result;

	if (multiplier->used <= 1)
	{
		if (!multiplier->used)
		{
			big_integer_clear(multiplicand);
			return true;
		}
		return big_integer_multiply_by_uint32(multiplicand, multiplier->data[0]);
	}

	if (multiplicand->used <= 1)
	{
		uint32_t small_multiplier;

		if (!multiplicand->used)
			return true;
		small_multiplier = multiplicand->data[0];
		big_integer_assign(multiplicand, multiplier);
		return big_integer_multiply_by_uint32(multiplicand, small_multiplier);
	}

	// We prefer more iterations on the inner loop and fewer on the outer:
	multiplier_is_shorter = multiplier->used < multiplicand->used;
	rgu1 = multiplier_is_shorter ? multiplier->data : multiplicand->data;
	rgu2 = multiplier_is_shorter ? multiplicand->data : multiplier->data;

	cu1 = multiplier_is_shorter ? multiplier->used : multiplicand->used;
	cu2 = multiplier_is_shorter ? multiplicand->used : multiplier->used;

	big_integer_clear(&result);
	for (iu1 = 0; iu1 != cu1; ++iu1)
	{
		uint32_t u_cur;
		uint32_t u_carry;
		uint32_t iu_res;

		u_cur = rgu1[iu1];
		if (u_cur == 0)
		{
			if (iu1 == result.used)
			{
				result.data[iu1] = 0;
				result.used = iu1 + 1;
			}

			continue;
		}

		u_carry = 0;
		iu_res = iu1;
		for (iu2 = 0; iu2 != cu2 && iu_res != BIG_INTEGER_ELEMENT_COUNT; ++iu2, ++iu_res)
		{
			if (iu_res == result.used)
			{
				result.data[iu_res] = 0;
				result.used = iu_res + 1;
			}

			u_carry = add_multiply_carry(&result.data[iu_res], u_cur, rgu2[iu2], u_carry);
		}

		while (u_carry != 0 && iu_res != BIG_INTEGER_ELEMENT_COUNT)
		{
			uint64_t uu;

			if (iu_res == result.used)
			{
				result.data[iu_res] = 0;
				result.used = iu_res + 1;
			}

			uu = (uint64_t)result.data[iu_res] + u_carry;
			result.data[iu_res++] = (uint32_t)uu;
			u_carry = (uint32_t)(uu >> 32);
		}

		if (iu_res == BIG_INTEGER_ELEMENT_COUNT)
		{
			big_integer_clear(multiplicand);
			return false;
		}
	}

	// Store the result in the multiplicand and compute the actual number of
	// elements used:
	big_integer_assign(multiplicand, &result);
	return true;
}

// Multiplies the high precision integer x by 10^power.  Returns true if the
// multiplication was successful; false if it overflowed.  When overflow occurs,
// the high precision integer is reset to zero.
bool __fastcall big_integer_multiply_by_power_of_ten(big_integer *x, const uint32_t power)
{
	// To improve performance, we use a table of precomputed powers of ten, from
	// 10^10 through 10^380, in increments of ten.  In its unpacked form, as an
	// array of big_integer objects, this table consists mostly of zero elements.
	// Thus, we store the table in a packed form, trimming leading and trailing
	// zero elements.  We provide an index that is used to unpack powers from the
	// table, using the function that appears after this function in this file.
	//
	// The minimum value representable with double precision is 5E-324.  With
	// this table we can thus compute most multiplications with a single multiply.
	static const uint32_t large_power_data[] =
	{
		0x540BE400, 0x00000002, 0x63100000, 0x6BC75E2D, 0x00000005, 0x40000000, 0x4674EDEA, 0x9F2C9CD0,
		0x0000000C, 0xB9F56100, 0x5CA4BFAB, 0x6329F1C3, 0x0000001D, 0xB5640000, 0xC40534FD, 0x926687D2,
		0x6C3B15F9, 0x00000044, 0x10000000, 0x946590D9, 0xD762422C, 0x9A224501, 0x4F272617, 0x0000009F,
		0x07950240, 0x245689C1, 0xC5FAA71C, 0x73C86D67, 0xEBAD6DDC, 0x00000172, 0xCEC10000, 0x63A22764,
		0xEFA418CA, 0xCDD17B25, 0x6BDFEF70, 0x9DEA3E1F, 0x0000035F, 0xE4000000, 0xCDC3FE6E, 0x66BC0C6A,
		0x2E391F32, 0x5A450203, 0x71D2F825, 0xC3C24A56, 0x000007DA, 0xA82E8F10, 0xAAB24308, 0x8E211A7C,
		0xF38ACE40, 0x84C4CE0B, 0x7CEB0B27, 0xAD2594C3, 0x00001249, 0xDD1A4000, 0xCC9F54DA, 0xDC5961BF,
		0xC75CABAB, 0xF505440C, 0xD1BC1667, 0xFBB7AF52, 0x608F8D29, 0x00002A94, 0x21000000, 0x17BB8A0C,
		0x56AF8EA4, 0x06479FA9, 0x5D4BB236, 0x80DC5FE0, 0xF0FEAA0A, 0xA88ED940, 0x6B1A80D0, 0x00006323,
		0x324C3864, 0x8357C796, 0xE44A42D5, 0xD9A92261, 0xBD3C103D, 0x91E5F372, 0xC0591574, 0xEC1DA60D,
		0x102AD96C, 0x0000E6D3, 0x1E851000, 0x6E4F615B, 0x187B2A69, 0x0450E21C, 0x2FDD342B, 0x635027EE,
		0xA6C97199, 0x8E4AE916, 0x17082E28, 0x1A496E6F, 0x0002196E, 0x32400000, 0x04AD4026, 0xF91E7250,
		0x2994D1D5, 0x665BCDBB, 0xA23B2E96, 0x65FA7DDB, 0x77DE53AC, 0xB020A29B, 0xC6BFF953, 0x4B9425AB,
		0x0004E34D, 0xFBC32D81, 0x5222D0F4, 0xB70F2850, 0x5713F2F3, 0xDC421413, 0xD6395D7D, 0xF8591999,
		0x0092381C, 0x86B314D6, 0x7AA577B9, 0x12B7FE61, 0x000B616A, 0x1D11E400, 0x56C3678D, 0x3A941F20,
		0x9B09368B, 0xBD706908, 0x207665BE, 0x9B26C4EB, 0x1567E89D, 0x9D15096E, 0x7132F22B, 0xBE485113,
		0x45E5A2CE, 0x001A7F52, 0xBB100000, 0x02F79478, 0x8C1B74C0, 0xB0F05D00, 0xA9DBC675, 0xE2D9B914,
		0x650F72DF, 0x77284B4C, 0x6DF6E016, 0x514391C2, 0x2795C9CF, 0xD6E2AB55, 0x9CA8E627, 0x003DB1A6,
		0x40000000, 0xF4ECD04A, 0x7F2388F0, 0x580A6DC5, 0x43BF046F, 0xF82D5DC3, 0xEE110848, 0xFAA0591C,
		0xCDF4F028, 0x192EA53F, 0xBCD671A0, 0x7D694487, 0x10F96E01, 0x791A569D, 0x008FA475, 0xB9B2E100,
		0x8288753C, 0xCD3F1693, 0x89B43A6B, 0x089E87DE, 0x684D4546, 0xFDDBA60C, 0xDF249391, 0x3068EC13,
		0x99B44427, 0xB68141EE, 0x5802CAC3, 0xD96851F1, 0x7D7625A2, 0x014E718D, 0xFB640000, 0xF25A83E6,
		0x9457AD0F, 0x0080B511, 0x2029B566, 0xD7C5D2CF, 0xA53F6D7D, 0xCDB74D1C, 0xDA9D70DE, 0xB716413D,
		0x71D0CA4E, 0xD7E41398, 0x4F403A90, 0xF9AB3FE2, 0x264D776F, 0x030AAFE6, 0x10000000, 0x09AB5531,
		0xA60C58D2, 0x566126CB, 0x6A1C8387, 0x7587F4C1, 0x2C44E876, 0x41A047CF, 0xC908059E, 0xA0BA063E,
		0xE7CFC8E8, 0xE1FAC055, 0xEF0144B2, 0x24207EB0, 0xD1722573, 0xE4B8F981, 0x071505AE, 0x7A3B6240,
		0xCEA45D4F, 0x4FE24133, 0x210F6D6D, 0xE55633F2, 0x25C11356, 0x28EBD797, 0xD396EB84, 0x1E493B77,
		0x471F2DAE, 0x96AD3820, 0x8AFACED1, 0x4EDECDDB, 0x5568C086, 0xB2695DA1, 0x24123C89, 0x107D4571,
		0x1C410000, 0x6E174A27, 0xEC62AE57, 0xEF2289AA, 0xB6A2FBDD, 0x17E1EFE4, 0x3366BDF2, 0x37B48880,
		0xBFB82C3E, 0x19ACDE91, 0xD4F46408, 0x35FF6A4E, 0x67566A0E, 0x40DBB914, 0x782A3BCA, 0x6B329B68,
		0xF5AFC5D9, 0x266469BC, 0xE4000000, 0xFB805FF4, 0xED55D1AF, 0x9B4A20A8, 0xAB9757F8, 0x01AEFE0A,
		0x4A2CA67B, 0x1EBF9569, 0xC7C41C29, 0xD8D5D2AA, 0xD136C776, 0x93DA550C, 0x9AC79D90, 0x254BCBA8,
		0x0DF07618, 0xF7A88809, 0x3A1F1074, 0xE54811FC, 0x59638EAD, 0x97CBE710, 0x26D769E8, 0xB4E4723E,
		0x5B90AA86, 0x9C333922, 0x4B7A0775, 0x2D47E991, 0x9A6EF977, 0x160B40E7, 0x0C92F8C4, 0xF25FF010,
		0x25C36C11, 0xC9F98B42, 0x730B919D, 0x05FF7CAF, 0xB0432D85, 0x2D2B7569, 0xA657842C, 0xD01FEF10,
		0xC77A4000, 0xE8B862E5, 0x10D8886A, 0xC8CD98E5, 0x108955C5, 0xD059B655, 0x58FBBED4, 0x03B88231,
		0x034C4519, 0x194DC939, 0x1FC500AC, 0x794CC0E2, 0x3BC980A1, 0xE9B12DD1, 0x5E6D22F8, 0x7B38899A,
		0xCE7919D8, 0x78C67672, 0x79E5B99F, 0xE494034E, 0x00000001, 0xA1000000, 0x6C5CD4E9, 0x9BE47D6F,
		0xF93BD9E7, 0x77626FA1, 0xC68B3451, 0xDE2B59E8, 0xCF3CDE58, 0x2246FF58, 0xA8577C15, 0x26E77559,
		0x17776753, 0xEBE6B763, 0xE3FD0A5F, 0x33E83969, 0xA805A035, 0xF631B987, 0x211F0F43, 0xD85A43DB,
		0xAB1BF596, 0x683F19A2, 0x00000004, 0xBE7DFE64, 0x4BC9042F, 0xE1F5EDB0, 0x8FA14EDA, 0xE409DB73,
		0x674FEE9C, 0xA9159F0D, 0xF6B5B5D6, 0x7338960E, 0xEB49C291, 0x5F2B97CC, 0x0F383F95, 0x2091B3F6,
		0xD1783714, 0xC1D142DF, 0x153E22DE, 0x8AAFDF57, 0x77F5E55F, 0xA3E7CA8B, 0x032F525B, 0x42E74F3D,
		0x0000000A, 0xF4DD1000, 0x5D450952, 0xAEB442E1, 0xA3B3342E, 0x3FCDA36F, 0xB4287A6E, 0x4BC177F7,
		0x67D2C8D0, 0xAEA8F8E0, 0xADC93B67, 0x6CC856B3, 0x959D9D0B, 0x5B48C100, 0x4ABE8A3D, 0x52D936F4,
		0x71DBE84D, 0xF91C21C5, 0x4A458109, 0xD7AAD86A, 0x08E14C7C, 0x759BA59C, 0xE43C8800, 0x00000017,
		0x92400000, 0x04F110D4, 0x186472BE, 0x8736C10C, 0x1478ABFB, 0xFC51AF29, 0x25EB9739, 0x4C2B3015,
		0xA1030E0B, 0x28FE3C3B, 0x7788FCBA, 0xB89E4358, 0x733DE4A4, 0x7C46F2C2, 0x8F746298, 0xDB19210F,
		0x2EA3B6AE, 0xAA5014B2, 0xEA39AB8D, 0x97963442, 0x01DFDFA9, 0xD2F3D3FE, 0xA0790280, 0x00000037,
		0x509C9B01, 0xC7DCADF1, 0x383DAD2C, 0x73C64D37, 0xEA6D67D0, 0x519BA806, 0xC403F2F8, 0xA052E1A2,
		0xD710233A, 0x448573A9, 0xCF12D9BA, 0x70871803, 0x52DC3A9B, 0xE5B252E8, 0x0717FB4E, 0xBE4DA62F,
		0x0AABD7E1, 0x8C62ED4F, 0xCEB9EC7B, 0xD4664021, 0xA1158300, 0xCCE375E6, 0x842F29F2, 0x00000081,
		0x7717E400, 0xD3F5FB64, 0xA0763D71, 0x7D142FE9, 0x33F44C66, 0xF3B8F12E, 0x130F0D8E, 0x734C9469,
		0x60260FA8, 0x3C011340, 0xCC71880A, 0x37A52D21, 0x8ADAC9EF, 0x42BB31B4, 0xD6F94C41, 0xC88B056C,
		0xE20501B8, 0x5297ED7C, 0x62C361C4, 0x87DAD8AA, 0xB833EADE, 0x94F06861, 0x13CC9ABD, 0x8DC1D56A,
		0x0000012D, 0x13100000, 0xC67A36E8, 0xF416299E, 0xF3493F0A, 0x77A5A6CF, 0xA4BE23A3, 0xCCA25B82,
		0x3510722F, 0xBE9D447F, 0xA8C213B8, 0xC94C324E, 0xBC9E33AD, 0x76ACFEBA, 0x2E4C2132, 0x3E13CD32,
		0x70FE91B4, 0xBB5CD936, 0x42149785, 0x46CC1AFD, 0xE638DDF8, 0x690787D2, 0x1A02D117, 0x3EB5F1FE,
		0xC3B9ABAE, 0x1C08EE6F, 0x000002BE, 0x40000000, 0x8140C2AA, 0x2CF877D9, 0x71E1D73D, 0xD5E72F98,
		0x72516309, 0xAFA819DD, 0xD62A5A46, 0x2A02DCCE, 0xCE46DDFE, 0x2713248D, 0xB723D2AD, 0xC404BB19,
		0xB706CC2B, 0x47B1EBCA, 0x9D094BDC, 0xC5DC02CA, 0x31E6518E, 0x8EC35680, 0x342F58A8, 0x8B041E42,
		0xFEBFE514, 0x05FFFC13, 0x6763790F, 0x66D536FD, 0xB9E15076, 0x00000662, 0x67B06100, 0xD2010A1A,
		0xD005E1C0, 0xDB12733B, 0xA39F2E3F, 0x61B29DE2, 0x2A63DCE2, 0x942604BC, 0x6170D59B, 0xC2E32596,
		0x140B75B9, 0x1F1D2C21, 0xB8136A60, 0x89D23BA2, 0x60F17D73, 0xC6CAD7DF, 0x0669DF2B, 0x24B88737,
		0x669306ED, 0x19496EEB, 0x938DDB6F, 0x5E748275, 0xC56E9A36, 0x3690B731, 0xC82842C5, 0x24AE798E,
		0x00000EDE, 0x41640000, 0xD5889AC1, 0xD9432C99, 0xA280E71A, 0x6BF63D2E, 0x8249793D, 0x79E7A943,
		0x22FDE64A, 0xE0D6709A, 0x05CACFEF, 0xBD8DA4D7, 0xE364006C, 0xA54EDCB3, 0xA1A8086E, 0x748F459E,
		0xFC8E54C8, 0xCC74C657, 0x42B8C3D4, 0x57D9636E, 0x35B55BCC, 0x6C13FEE9, 0x1AC45161, 0xB595BADB,
		0xA1F14E9D, 0xDCF9E750, 0x07637F71, 0xDE2F9F2B, 0x0000229D, 0x10000000, 0x3C5EBD89, 0xE3773756,
		0x3DCBA338, 0x81D29E4F, 0xA4F79E2C, 0xC3F9C774, 0x6A1CE797, 0xAC5FE438, 0x07F38B9C, 0xD588ECFA,
		0x3E5AC1AC, 0x85AFCCCE, 0x9D1F3F70, 0xE82D6DD3, 0x177D180C, 0x5E69946F, 0x648E2CE1, 0x95A13948,
		0x340FE011, 0xB4173C58, 0x2748F694, 0x7C2657BD, 0x758BDA2E, 0x3B8090A0, 0x2DDBB613, 0x6DCF4890,
		0x24E4047E, 0x00005099,
	};

	typedef struct {
		uint16_t offset;    // The offset of this power's initial byte in the array
		uint8_t  zeroes;    // The number of omitted leading zero elements
		uint8_t  size;      // The number of elements present for this power
	} unpack_index;

	static const unpack_index large_power_indices[] =
	{
		{   0,  0,  2 }, {   2,  0,  3 }, {   5,  0,  4 }, {   9,  1,  4 },
		{  13,  1,  5 }, {  18,  1,  6 }, {  24,  2,  6 }, {  30,  2,  7 },
		{  37,  2,  8 }, {  45,  3,  8 }, {  53,  3,  9 }, {  62,  3, 10 },
		{  72,  4, 10 }, {  82,  4, 11 }, {  93,  4, 12 }, { 105,  5, 12 },
		{ 117,  5, 13 }, { 130,  5, 14 }, { 144,  5, 15 }, { 159,  6, 15 },
		{ 174,  6, 16 }, { 190,  6, 17 }, { 207,  7, 17 }, { 224,  7, 18 },
		{ 242,  7, 19 }, { 261,  8, 19 }, { 280,  8, 21 }, { 301,  8, 22 },
		{ 323,  9, 22 }, { 345,  9, 23 }, { 368,  9, 24 }, { 392, 10, 24 },
		{ 416, 10, 25 }, { 441, 10, 26 }, { 467, 10, 27 }, { 494, 11, 27 },
		{ 521, 11, 28 }, { 549, 11, 29 },
	};

	static const uint32_t small_powers_of_ten[9] =
	{
		        10,
		       100,
		      1000,
		     10000,
		    100000,
		   1000000,
		  10000000,
		 100000000,
		1000000000
	};

	uint32_t large_power;
	uint32_t small_power;

	large_power = power / 10;
	while (large_power != 0)
	{
		uint32_t           current_power;
		const unpack_index *index;
		big_integer        multiplier;
		const uint32_t     *source;

		current_power = large_power > _countof(large_power_indices)
			? _countof(large_power_indices)
			: large_power;

		index = &large_power_indices[current_power - 1];

		multiplier.used = index->size + index->zeroes;

		source = large_power_data + index->offset;

		memset(multiplier.data, 0, index->zeroes * sizeof(uint32_t));
		memcpy(multiplier.data + index->zeroes, source, index->size * sizeof(uint32_t));

		if (!big_integer_multiply(x, &multiplier))
		{
			big_integer_clear(x);
			return false;
		}

		large_power -= current_power;
	}

	small_power = power % 10;
	if (small_power != 0)
		if (!big_integer_multiply_by_uint32(x, small_powers_of_ten[small_power - 1]))
			return false;

	return true;
}

// The following non-compiled functions are the generators for the big powers of
// ten table found in big_integer_multiply_by_power_of_ten().  This code is provided for
// future use if the table needs to be amended.  Do not remove this code.
/*
#define _CRT_SECURE_NO_WARNINGS
#include "big_integer.h"
#include <stdio.h>

typedef unsigned __int8  uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;

static uint32_t count_leading_zeroes(big_integer *x)
{
	uint32_t i;

	for (i = 0; i != x->used; ++i)
	{
		if (x->data[i] != 0)
			return i;
	}

	return 0;
}

void generate_table()
{
	#define NUMBER_OF_INDICES (380 / 10)

	typedef struct {
		uint16_t offset;    // The offset of this power's initial byte in the array
		uint8_t  zeroes;    // The number of omitted leading zero elements
		uint8_t  size;      // The number of elements present for this power
	} unpack_index;

	unpack_index indices[NUMBER_OF_INDICES];
	size_t       indices_size;
	size_t       offset;
	uint32_t     i, j;
	big_integer  x;
	unpack_index *index;

	indices_size = 0;
	offset = 0;

	printf("static const uint32_t large_power_data[] =\n{");
	for (i = 10; i != NUMBER_OF_INDICES * 10 + 10; i += 10)
	{
		big_integer_assign_uint32(&x, 1);
		for (j = 0; j != i; ++j)
		{
			big_integer_multiply_by_uint32(&x, 10);
		}

		index = indices + indices_size++;
		index->offset = (uint16_t)offset;
		index->zeroes = (uint8_t)count_leading_zeroes(&x);
		index->size   = (uint8_t)(x.used - index->zeroes);

		for (j = index->zeroes; j != x.used; ++j)
		{
			printf("%s0x%08X,", offset++ % 8 == 0 ? "\n\t" : " ", x.data[j]);
		}
	}
	printf("\n};\n\n");

	printf("static const unpack_index large_power_indices[] =\n{");
	for (i = 0; i != NUMBER_OF_INDICES; ++i)
	{
		printf("%s{ %3u, %2u, %2u },",
			i % 4 == 0 ? "\n\t" : " ",
			indices[i].offset,
			indices[i].zeroes,
			indices[i].size);
	}
	printf("\n};\n\n");

	#undef NUMBER_OF_INDICES
}
*/

// Computes the number of zeroes higher than the most significant set bit in 'u'
__forceinline static uint32_t count_sequential_high_zeroes(const uint32_t u)
{
	unsigned long result;

	return _BitScanReverse(&result, u) ? 31 - result : 32;
}

// PERFORMANCE NOTE:  On x86, for multiplication of a 64-bit unsigned integer by
// a 32-bit unsigned integer, the compiler will generate a call to _allmul.  For
// division-heavy conversions, the inline assembly version presented here gives a
// 10% overall performance improvement (not 10% faster division--10% faster total).
// This function [1] uses only two 32-bit multiplies instead of the three required
// for general 64-bit x 64-bit multiplication, and [2] is inlineable, allowing the
// compile to elide the extreme overhead of calling the _allmul function.
#if defined(_M_IX86) && !defined(_M_HYBRID_X86_ARM64)
__forceinline static uint64_t multiply_64_32(const uint64_t multiplicand, const uint32_t multiplier)
{
	__asm
	{
		mov     ecx, dword ptr [multiplicand + 4]
		mov     eax, dword ptr [multiplicand]
		imul    ecx, multiplier
		mul     multiplier
		add     edx, ecx
	}
}
#else
#define multiply_64_32(multiplicand, multiplier) (uint64_t)((uint64_t)(multiplicand) * (uint32_t)(multiplier))
#endif

// This high precision integer division implementation was translated from the
// implementation of System.Numerics.BigIntegerBuilder.ModDivCore in the .NET
// Framework sources.  It computes both quotient and remainder:  the remainder
// is stored in the numerator argument, and the least significant 32 bits of the
// quotient are returned from the function.
uint64_t __fastcall big_integer_divide(big_integer *numerator, const big_integer *denominator)
{
	uint32_t max_numerator_element_index;
	uint32_t max_denominator_element_index;
	uint32_t cu_den;
	int32_t  cu_diff;
	int32_t  cu_quo;
	int32_t  iu;
	uint32_t u_den;
	uint32_t u_den_next;
	uint32_t cbit_shift_left;
	uint32_t cbit_shift_right;
	uint64_t quotient;

	// If the numerator is zero, then both the quotient and remainder are zero:
	if (numerator->used == 0)
		return 0;

	// If the denominator is zero, then uh oh. We can't big_integer_divide by zero:
	if (denominator->used == 0)
	{
		_ASSERTE(("Division by zero", false));
		return 0;
	}

	max_numerator_element_index = numerator->used - 1;
	max_denominator_element_index = denominator->used - 1;

	// The numerator and denominator are both nonzero.  If the denominator is
	// only one element wide, we can take the fast route:
	if (max_denominator_element_index == 0)
	{
		uint32_t small_denominator;
		uint64_t quotient;
		uint64_t uu;
		uint32_t iv;

		small_denominator = denominator->data[0];

		if (small_denominator == 1)
		{
			uint32_t quotient;

			quotient = numerator->data[0];
			big_integer_clear(numerator);
			return quotient;
		}

		if (max_numerator_element_index == 0)
		{
			uint32_t small_numerator;

			small_numerator = numerator->data[0];
			big_integer_clear(numerator);
			numerator->data[0] = small_numerator % small_denominator;
			numerator->used = numerator->data[0] > 0 ? 1 : 0;
			return small_numerator / small_denominator;
		}

		// We count down in the next loop, so the last assignment to quotient
		// will be the correct one.
		quotient = 0;

		uu = 0;
		for (iv = max_numerator_element_index; iv != (uint32_t)-1; --iv)
		{
			uu = (uu << 32) | numerator->data[iv];
			quotient = (quotient << 32) + (uint32_t)(uu / small_denominator);
			uu %= small_denominator;
		}

		big_integer_clear(numerator);
		numerator->data[1] = (uint32_t)(uu >> 32);
		numerator->data[0] = (uint32_t)uu;
		numerator->used = numerator->data[1] > 0 ? 2 : 1;
		return quotient;
	}

	if (max_denominator_element_index > max_numerator_element_index)
		return 0;

	cu_den = max_denominator_element_index + 1;
	cu_diff = max_numerator_element_index - max_denominator_element_index;

	// Determine whether the result will have cu_diff or cu_diff + 1 digits:
	cu_quo = cu_diff;
	for (iu = max_numerator_element_index; ; --iu)
	{
		if (iu < cu_diff)
		{
			++cu_quo;
			break;
		}

		if (denominator->data[iu - cu_diff] != numerator->data[iu])
		{
			if (denominator->data[iu - cu_diff] < numerator->data[iu])
				++cu_quo;
			break;
		}
	}

	if (cu_quo == 0)
		return 0;

	// Get the uint to use for the trial divisions.  We normalize so the
	// high bit is set:
	u_den = denominator->data[cu_den - 1];
	u_den_next = denominator->data[cu_den - 2];

	cbit_shift_left = count_sequential_high_zeroes(u_den);
	cbit_shift_right = 32 - cbit_shift_left;
	if (cbit_shift_left > 0)
	{
		u_den = (u_den << cbit_shift_left) | (u_den_next >> cbit_shift_right);
		u_den_next <<= cbit_shift_left;

		if (cu_den > 2)
			u_den_next |= denominator->data[cu_den - 3] >> cbit_shift_right;
	}

	quotient = 0;
	for (iu = cu_quo; --iu >= 0; )
	{
		uint32_t u_num_hi;
		uint64_t uu_num;
		uint32_t u_num_next;
		uint64_t uu_quo;
		uint64_t uu_rem;

		// Get the high (normalized) bits of the numerator:
		u_num_hi = (iu + cu_den <= max_numerator_element_index)
			? numerator->data[iu + cu_den]
			: 0;

		uu_num = numerator->data[iu + cu_den - 1];
		((uint32_t *)&uu_num)[1] = u_num_hi;

		u_num_next = numerator->data[iu + cu_den - 2];
		if (cbit_shift_left > 0)
		{
			uu_num = (uu_num << cbit_shift_left) | (u_num_next >> cbit_shift_right);
			u_num_next <<= cbit_shift_left;

			if (iu + cu_den >= 3)
				u_num_next |= numerator->data[iu + cu_den - 3] >> cbit_shift_right;
		}

		// Divide to get the quotient digit:
		uu_quo = uu_num / u_den;
		uu_rem = (uint32_t)(uu_num % u_den);

		if (uu_quo > UINT32_MAX)
		{
			uu_rem += u_den * (uu_quo - UINT32_MAX);
			uu_quo = UINT32_MAX;
		}

		while (uu_rem <= UINT32_MAX && uu_quo * u_den_next > ((uu_rem << 32) | u_num_next))
		{
			--uu_quo;
			uu_rem += u_den;
		}

		// Multiply and subtract.  Note that uu_quo may be one too large.  If
		// we have a borrow at the end, we'll add the denominator back on and
		// decrement uu_quo.
		if (uu_quo > 0)
		{
			uint64_t uu_borrow;
			uint32_t iu2;

			uu_borrow = 0;

			for (iu2 = 0; iu2 < cu_den; ++iu2)
			{
				uint32_t u_sub;

				uu_borrow += multiply_64_32(uu_quo, denominator->data[iu2]);
				u_sub = (uint32_t)uu_borrow;
				uu_borrow >>= 32;
				if (numerator->data[iu + iu2] < u_sub)
					++uu_borrow;

				numerator->data[iu + iu2] -= u_sub;
			}

			if (u_num_hi < uu_borrow)
			{
				uint32_t u_carry;
				uint32_t iu2;

				// Add, tracking carry:
				u_carry = 0;
				for (iu2 = 0; iu2 < cu_den; ++iu2)
				{
					uint64_t sum;

					sum =
						(uint64_t)numerator->data[iu + iu2] +
						(uint64_t)denominator->data[iu2] +
						u_carry;
					numerator->data[iu + iu2] = (uint32_t)sum;
					u_carry = (uint32_t)(sum >> 32);
				}

				--uu_quo;
			}

			max_numerator_element_index = iu + cu_den - 1;
		}

		quotient = (quotient << 32) + (uint32_t)uu_quo;
	}

	// Trim the remainder:
	memset(
		numerator->data + max_numerator_element_index + 1,
		0,
		(numerator->used - (max_numerator_element_index + 1)) * sizeof(uint32_t));

	numerator->used = max_numerator_element_index + 1;
	while (numerator->used != 0 && numerator->data[numerator->used - 1] == 0)
		--numerator->used;

	return quotient;
}
