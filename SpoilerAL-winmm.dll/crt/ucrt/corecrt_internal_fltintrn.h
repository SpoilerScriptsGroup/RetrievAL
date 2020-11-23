//
// corecrt_internal_fltintrn.h
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// Floating point conversion routines for internal use.  This is a C++ header.
//
#ifndef _CORECRT_INTERNAL_FLTINTRN_H_
#define _CORECRT_INTERNAL_FLTINTRN_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <stddef.h>
#if !defined(_MSC_VER) || _MSC_VER >= 1600
#include <stdint.h>
#else
typedef unsigned __int64 uint64_t;
#endif
#include <errno.h>
typedef int errno_t;
#include <float.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Types
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#define FLOATING_TRAITS_MANTISSA_BITS               DBL_MANT_DIG
#define FLOATING_TRAITS_EXPONENT_BITS               (sizeof(double) * CHAR_BIT - DBL_MANT_DIG)
#define FLOATING_TRAITS_MAXIMUM_BINARY_EXPONENT     (DBL_MAX_EXP - 1)
#define FLOATING_TRAITS_MINIMUM_BINARY_EXPONENT     (DBL_MIN_EXP - 1)
#define FLOATING_TRAITS_EXPONENT_BIAS               1023
#define FLOATING_TRAITS_EXPONENT_MASK               ((uint64_t)1 << FLOATING_TRAITS_EXPONENT_BITS) - 1)
#define FLOATING_TRAITS_NORMAL_MANTISSA_MASK        ((uint64_t)1 << FLOATING_TRAITS_MANTISSA_BITS) - 1)
#define FLOATING_TRAITS_DENORMAL_MANTISSA_MASK      ((uint64_t)1 << (FLOATING_TRAITS_MANTISSA_BITS - 1)) - 1)
#define FLOATING_TRAITS_SPECIAL_NAN_MANTISSA_MASK   ((uint64_t)1 << (FLOATING_TRAITS_MANTISSA_BITS - 2))

typedef union {
	struct {
		uint64_t mantissa : FLOATING_TRAITS_MANTISSA_BITS - 1;
		uint64_t exponent : FLOATING_TRAITS_EXPONENT_BITS;
		uint64_t sign     : 1;
	};
	double value;
} floating_traits;

#define FP_CLASS_FINITE         0
#define FP_CLASS_INFINITY       1
#define FP_CLASS_QUIET_NAN      2
#define FP_CLASS_SIGNALING_NAN  3
#define FP_CLASS_INDETERMINATE  4

typedef struct {
	int  sign;      // Zero if positive otherwise negative
	int  decpt;     // Exponent of floating point number
	char *mantissa; // Pointer to mantissa in string form
} _strflt;

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Floating Point Conversion Routines
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
errno_t __fastcall __acrt_fp_strflt_to_string(
	char         *buffer,
	const size_t buffer_count,
	int          digits,
	_strflt      *pflt);

void __fastcall __acrt_fltout(
	const double       *value,
	const unsigned int precision,
	_strflt            *flt,
	char               *result,
	const size_t       result_count);

errno_t __fastcall __acrt_fp_format(
	const double   *value,
	char           *result_buffer,
	const size_t   result_buffer_count,
	char           *scratch_buffer,
	const size_t   scratch_buffer_count,
	const int      format,
	const int      precision,
	const uint64_t options,
	const int      decimal_point_char);

#ifdef __cplusplus
}
#endif

#endif	// _CORECRT_INTERNAL_FLTINTRN_H_
