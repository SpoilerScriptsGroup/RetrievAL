//
// big_integer.h
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// A lightweight high precision integer type for use by the binary floating
// point <=> decimal string conversion functions.
//
#ifndef _BIG_INTEGER_H_
#define _BIG_INTEGER_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <limits.h>
#if !defined(_MSC_VER) || _MSC_VER >= 1600
#include <stdbool.h>
#include <stdint.h>
#else
typedef unsigned __int8  bool;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#endif
#include <memory.h>
#if defined(_MSC_VER) && _MSC_VER >= 1310
#include <intrin.h>
#endif

#pragma function(memcpy, memset)

#ifdef __cplusplus
extern "C" {
#endif

// A lightweight, sufficiently functional high-precision integer type for use in
// the binary floating point <=> decimal string conversions.  We define only the
// operations (and in some cases parts of operations) that are actually used.
//
// We require sufficient precision to represent the reciprocal of the smallest
// representable value (the smallest denormal, 2^-1074).  During parsing, we may
// also consider up to 768 decimal digits.  For this, we require an additional
// log2(10^768) bits of precision.  Finally, we require 32 bits of space for
// pre-division numerator shifting.
//
// PERFORMANCE NOTE:  We intentionally do not initialize the _data array when a
// big_integer object is constructed.  Profiling showed that zero initialization
// caused a substantial performance hit.  Initialization of the _data array is
// not necessary:  all operations on the big_integer type are carefully written
// to only access elements at indices [0, _used], and all operations correctly
// update _used as the utilized size increases.

#define BIG_INTEGER_MAXIMUM_BITS    (1074 + /* 1074 bits required to represent 2^1074 */ \
                                     2552 + /* ceil(log2(10^768)) */                     \
                                     32     /* shift space */)
#define BIG_INTEGER_ELEMENT_BITS    (sizeof(uint32_t) * CHAR_BIT)
#define BIG_INTEGER_ELEMENT_COUNT   ((BIG_INTEGER_MAXIMUM_BITS + BIG_INTEGER_ELEMENT_BITS - 1) / BIG_INTEGER_ELEMENT_BITS)

typedef struct {
	uint32_t used;                              // The number of elements currently in use
	uint32_t data[BIG_INTEGER_ELEMENT_COUNT];   // The number, stored in little endian form
} big_integer;

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Intrinsic functions.
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(_BitScanReverse)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline static unsigned __int64 __reg64return_BitScanReverse(unsigned long Mask)
{
	__asm
	{
		bsr     edx, dword ptr [Mask]
		setnz   al
	}
}
__forceinline static unsigned char _BitScanReverse(unsigned long *Index, unsigned long Mask)
{
	unsigned __int64 x = __reg64return_BitScanReverse(Mask);
	*Index = (unsigned long)(x >> 32);
	return (unsigned char)x;
}
#else
__forceinline static unsigned char _BitScanReverse(unsigned long *Index, unsigned long Mask)
{
	if (Mask)
	{
		unsigned long i;

		for (i = 31; (long)Mask >= 0; Mask <<= 1)
			i--;
		*Index = i;
		return 1;
	}
	else
	{
		return 0;
	}
}
#endif

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// A lightweight high precision integer type for use by the binary floating
// point <=> decimal string conversion functions.
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
__forceinline static big_integer *big_integer_clear(big_integer *x)
{
	x->used = 0;
	return x;
}

__forceinline static big_integer *big_integer_assign(big_integer *x, const big_integer *value)
{
	memcpy(x->data, value->data, (x->used = value->used) * sizeof(uint32_t));
	return x;
}

__forceinline static big_integer *big_integer_assign_uint32(big_integer *x, const uint32_t value)
{
	x->used = 1;
	x->data[0] = (uint32_t)value;
	return x;
}

__forceinline static big_integer *big_integer_assign_uint64(big_integer* x, const uint64_t value)
{
	uint32_t high;

	if (!(high = (uint32_t)(value >> 32)))
		return big_integer_assign_uint32(x, (uint32_t)value);
	x->used = 2;
	x->data[0] = (uint32_t)value;
	x->data[1] = high;
	return x;
}

__forceinline static bool big_integer_equals(const big_integer *lhs, const big_integer *rhs)
{
	return
		lhs->used == rhs->used &&
		memcmp(lhs->data, rhs->data, lhs->used * sizeof(*lhs->data)) == 0;
}

bool __fastcall big_integer_less_than(const big_integer *lhs, const big_integer *rhs);

__forceinline static big_integer *big_integer_power_of_two(big_integer *x, const uint32_t power)
{
	uint32_t element_index;
	uint32_t bit_index;

	element_index = power / BIG_INTEGER_ELEMENT_BITS;
	bit_index = power % BIG_INTEGER_ELEMENT_BITS;
	x->used = element_index + 1;
	x->data[element_index] = ((uint32_t)1 << bit_index);
	memset(x->data, 0, element_index * sizeof(uint32_t));
	return x;
}

__forceinline static bool big_integer_is_zero(big_integer *value)
{
	return value->used == 0;
}

__forceinline static uint32_t big_integer_bit_scan_reverse32(const uint32_t value)
{
	unsigned long index;

	if (_BitScanReverse(&index, value))
		return index + 1;
	return 0;
}

__forceinline static uint32_t big_integer_bit_scan_reverse64(const uint64_t value)
{
	unsigned long index;

	if (_BitScanReverse(&index, (unsigned long)(value >> 32)))
		return index + 33;
	if (_BitScanReverse(&index, (unsigned long)value))
		return index + 1;
	return 0;
}

__forceinline static uint32_t big_integer_bit_scan_reverse(const big_integer *x)
{
	if (x->used == 0)
		return 0;
	return (x->used - 1) * BIG_INTEGER_ELEMENT_BITS + big_integer_bit_scan_reverse32(x->data[x->used - 1]);
}

// Shifts the high precision integer x by n bits to the left.  Returns true if
// the left shift was successful; false if it overflowed.  When overflow occurs,
// the high precision integer is reset to zero.
bool __fastcall big_integer_shift_left(big_integer *x, const uint32_t n);

// Adds a 32-bit value to the high-precision integer x.  Returns true if the
// addition was successful; false if it overflowed.  When overflow occurs, the
// high precision integer is reset to zero.
bool __fastcall big_integer_add(big_integer *x, const uint32_t value);

// Multiplies the high precision multiplicand by a 32-bit multiplier.  Returns
// true if the multiplication was successful; false if it overflowed.  When
// overflow occurs, the multiplicand is reset to zero.
bool __fastcall big_integer_multiply_by_uint32(big_integer *multiplicand, const uint32_t multiplier);

// This high precision integer division implementation was translated from the
// implementation of System.Numerics.BigIntegerBuilder.Mul in the .NET Framework
// sources.  It multiplies the multiplicand by the multiplier and returns true
// if the multiplication was successful; false if it overflowed.  When overflow
// occurs, the multiplicand is reset to zero.
bool __fastcall big_integer_multiply(big_integer *multiplicand, const big_integer *multiplier);

// Multiplies the high precision integer x by 10^power.  Returns true if the
// multiplication was successful; false if it overflowed.  When overflow occurs,
// the high precision integer is reset to zero.
bool __fastcall big_integer_multiply_by_power_of_ten(big_integer *x, const uint32_t power);

// This high precision integer division implementation was translated from the
// implementation of System.Numerics.BigIntegerBuilder.ModDivCore in the .NET
// Framework sources.  It computes both quotient and remainder:  the remainder
// is stored in the numerator argument, and the least significant 32 bits of the
// quotient are returned from the function.
uint64_t __fastcall big_integer_divide(big_integer *numerator, const big_integer *denominator);

#ifdef __cplusplus
}
#endif

#endif	// _BIG_INTEGER_H_
