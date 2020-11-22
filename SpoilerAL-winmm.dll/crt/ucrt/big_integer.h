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
typedef __int8           int8_t;
typedef unsigned __int8  uint8_t;
typedef __int16          int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32          int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64          int64_t;
typedef unsigned __int64 uint64_t;
#define false       0
#define true        1
#define INT8_MIN    _I8_MIN
#define INT8_MAX    _I8_MAX
#define INT16_MIN   _I16_MIN
#define INT16_MAX   _I16_MAX
#define INT32_MIN   _I32_MIN
#define INT32_MAX   _I32_MAX
#define INT64_MIN   _I64_MIN
#define INT64_MAX   _I64_MAX
#define INTMAX_MIN  _I64_MIN
#define INTMAX_MAX  _I64_MAX
#define UINT8_MAX   _UI8_MAX
#define UINT16_MAX  _UI16_MAX
#define UINT32_MAX  _UI32_MAX
#define UINT64_MAX  _UI64_MAX
#define INT8_C(x)   (x)
#define INT16_C(x)  (x)
#define INT32_C(x)  (x)
#define INT64_C(x)  (x ## I64)
#define UINT8_C(x)  (x)
#define UINT16_C(x) (x)
#define UINT32_C(x) (x ## U)
#define UINT64_C(x) (x ## UI64)
#endif
#include <memory.h>

#pragma function(memcpy)

#ifdef __cplusplus
extern "C" {
#endif

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// A lightweight high precision integer type for use by the binary floating
// point <=> decimal string conversion functions.
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#define BIG_INTEGER_MAXIMUM_BITS    (1074 + /* 1074 bits required to represent 2^1074 */ \
                                     2552 + /* ceil(log2(10^768)) */                     \
                                     32     /* shift space */)
#define BIG_INTEGER_ELEMENT_BITS    (sizeof(uint32_t) * CHAR_BIT)
#define BIG_INTEGER_ELEMENT_COUNT   ((BIG_INTEGER_MAXIMUM_BITS + BIG_INTEGER_ELEMENT_BITS - 1) / BIG_INTEGER_ELEMENT_BITS)

typedef struct {
	uint32_t used;                              // The number of elements currently in use
	uint32_t data[BIG_INTEGER_ELEMENT_COUNT];   // The number, stored in little endian form
} big_integer;

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

bool __cdecl big_integer_equals(const big_integer *lhs, const big_integer *rhs);

bool __cdecl big_integer_less_than(const big_integer *lhs, const big_integer *rhs);

big_integer *__cdecl big_integer_power_of_two(big_integer *x, const uint32_t power);

__forceinline static bool big_integer_is_zero(big_integer *value)
{
	return value->used == 0;
}

uint32_t __cdecl big_integer_bit_scan_reverse32(const uint32_t value);

uint32_t __cdecl big_integer_bit_scan_reverse64(const uint64_t value);

uint32_t __cdecl big_integer_bit_scan_reverse(const big_integer *x);

// Shifts the high precision integer x by n bits to the left.  Returns true if
// the left shift was successful; false if it overflowed.  When overflow occurs,
// the high precision integer is reset to zero.
bool __cdecl big_integer_shift_left(big_integer *x, const uint32_t n);

// Adds a 32-bit value to the high-precision integer x.  Returns true if the
// addition was successful; false if it overflowed.  When overflow occurs, the
// high precision integer is reset to zero.
bool __cdecl big_integer_add(big_integer *x, const uint32_t value);

// Multiplies the high precision multiplicand by a 32-bit multiplier.  Returns
// true if the multiplication was successful; false if it overflowed.  When
// overflow occurs, the multiplicand is reset to zero.
bool __cdecl big_integer_multiply_by_uint32(big_integer *multiplicand, const uint32_t multiplier);

// This high precision integer division implementation was translated from the
// implementation of System.Numerics.BigIntegerBuilder.Mul in the .NET Framework
// sources.  It multiplies the multiplicand by the multiplier and returns true
// if the multiplication was successful; false if it overflowed.  When overflow
// occurs, the multiplicand is reset to zero.
bool __cdecl big_integer_multiply(big_integer *multiplicand, const big_integer *multiplier);

// Multiplies the high precision integer x by 10^power.  Returns true if the
// multiplication was successful; false if it overflowed.  When overflow occurs,
// the high precision integer is reset to zero.
bool __cdecl big_integer_multiply_by_power_of_ten(big_integer *x, const uint32_t power);

// This high precision integer division implementation was translated from the
// implementation of System.Numerics.BigIntegerBuilder.ModDivCore in the .NET
// Framework sources.  It computes both quotient and remainder:  the remainder
// is stored in the numerator argument, and the least significant 32 bits of the
// quotient are returned from the function.
uint64_t __cdecl big_integer_divide(big_integer *numerator, const big_integer *denominator);

#ifdef __cplusplus
}
#endif

#endif	// _BIG_INTEGER_H_
