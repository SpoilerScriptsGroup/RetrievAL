#ifdef _UNICODE
typedef unsigned short wchar_t, TCHAR;
#define _vsntprintf _vsnwprintf
#else
typedef char TCHAR;
#define _vsntprintf _vsnprintf
#endif

#ifdef __BORLANDC__
#pragma warn -8027
#pragma warn -8060
#endif

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>    // using IsDBCSLeadByteEx
#undef isleadbyte
#define isleadbyte(c) IsDBCSLeadByteEx(CP_THREAD_ACP, c)
#else
#include <ctype.h>      // using isleadbyte
#endif

#ifdef _WIN32
#if defined(_MSC_VER) && _MSC_VER >= 1700
#include <winternl.h>   // using PANSI_STRING, PUNICODE_STRING
#else
#pragma pack(push, 1)
typedef struct {
	USHORT Length;
	USHORT MaximumLength;
	PCHAR  Buffer;
} ANSI_STRING, *PANSI_STRING;
typedef struct {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;
#pragma pack(pop)
#endif
#endif

#if !defined(_MSC_VER) || _MSC_VER >= 1700
typedef long long          long_long;
typedef unsigned long long unsigned_long_long;
#else
typedef __int64            long_long;
typedef unsigned __int64   unsigned_long_long;
#endif

#include <stddef.h>     // using ptrdiff_t
#include <limits.h>     // using CHAR_BIT, _I[N]_MIN, _I[N]_MAX, _UI[N]_MAX, CHAR_MAX, SHRT_MAX, LONG_MAX, LLONG_MAX, INT_MAX

// standard integer type definition
#if (!defined(_MSC_VER) || _MSC_VER >= 1600) && !defined(__BORLANDC__)
#include <stdint.h>
#else
typedef __int8           int8_t;
typedef unsigned __int8  uint8_t;
typedef __int16          int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32          int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64          int64_t;
typedef unsigned __int64 uint64_t;
typedef __int64          intmax_t;
typedef unsigned __int64 uintmax_t;
typedef ptrdiff_t        intptr_t;
typedef size_t           uintptr_t;
#ifdef __BORLANDC__
#define _I8_MIN   CHAR_MIN
#define _I8_MAX   CHAR_MAX
#define _I16_MIN  SHRT_MIN
#define _I16_MAX  SHRT_MAX
#define _I32_MIN  LONG_MIN
#define _I32_MAX  LONG_MAX
#define _UI8_MAX  UCHAR_MAX
#define _UI16_MAX USHRT_MAX
#define _UI32_MAX ULONG_MAX
#undef INTPTR_MIN
#undef INTPTR_MAX
#undef UINTPTR_MAX
#undef PTRDIFF_MIN
#undef PTRDIFF_MAX
#undef SIZE_MAX
#endif
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
#define UINTMAX_MAX _UI64_MAX
#ifdef _WIN64
#define INTPTR_MIN  _I64_MIN
#define INTPTR_MAX  _I64_MAX
#define UINTPTR_MAX _UI64_MAX
#else
#define INTPTR_MIN  _I32_MIN
#define INTPTR_MAX  _I32_MAX
#define UINTPTR_MAX _UI32_MAX
#endif
#define PTRDIFF_MIN INTPTR_MIN
#define PTRDIFF_MAX INTPTR_MAX
#define SIZE_MAX    UINTPTR_MAX
#define LLONG_MIN   _I64_MIN
#define LLONG_MAX   _I64_MAX
#define ULLONG_MAX  _UI64_MAX
#endif

#define INT_IS_CHAR      (INT_MAX == CHAR_MAX)
#define INT_IS_SHRT      (INT_MAX == SHRT_MAX)
#define INT_IS_LONG      (INT_MAX == LONG_MAX)
#define INT_IS_LLONG     (INT_MAX == LLONG_MAX)
#define INT_IS_INTMAX    (INT_MAX == INTMAX_MAX)
#define INTPTR_IS_CHAR   (INTPTR_MAX == CHAR_MAX)
#define INTPTR_IS_SHRT   (INTPTR_MAX == SHRT_MAX)
#define INTPTR_IS_LONG   (INTPTR_MAX == LONG_MAX)
#define INTPTR_IS_LLONG  (INTPTR_MAX == LLONG_MAX)
#define INTPTR_IS_INTMAX (INTPTR_MAX == INTMAX_MAX)
#define INTMAX_IS_LLONG  (INTMAX_MAX == LLONG_MAX)

#if INTPTR_MAX == INT8_MAX
#define INTPTR_BIT 8
#elif INTPTR_MAX == INT16_MAX
#define INTPTR_BIT 16
#elif INTPTR_MAX == INT32_MAX
#define INTPTR_BIT 32
#elif INTPTR_MAX == INT64_MAX
#define INTPTR_BIT 64
#else
#define INTPTR_BIT 128
#endif

// standard bool type definition
#if (!defined(_MSC_VER) || _MSC_VER >= 1600) && !defined(__BORLANDC__)
#include <stdbool.h>
#else
typedef unsigned char bool;
#define true  1
#define false 0
#endif

// byte-order definition
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__MINGW32__)
#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN    4321
#define __BYTE_ORDER    __LITTLE_ENDIAN
#elif defined(__GNUC__)
#include <sys/param.h>
#endif

#include <stdlib.h>     // using _countof
#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif

#include <errno.h>      // using ERANGE, EOVERFLOW
#ifndef ERANGE
#define ERANGE E2BIG
#endif
#ifndef EOVERFLOW
#define EOVERFLOW ERANGE
#endif

#include <math.h>       // using modf, floor, log10, exp10
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4163)
#pragma function(floor)
#pragma function(log10)
#pragma warning(pop)
#endif

#include <float.h>      // using DBL_MANT_DIG, DBL_MAX_EXP, DBL_MAX_10_EXP, DBL_DECIMAL_DIG
#ifndef DBL_DECIMAL_DIG
#define DBL_DECIMAL_DIG 17
#endif

#include <string.h>
#ifdef _UNICODE
#define _tcslen  wcslen
#define _tcsnlen wcsnlen
#else
#define _tcslen  strlen
#define _tcsnlen strnlen
#endif
#if (defined(_MSC_VER) && _MSC_VER < 1300) || defined(__BORLANDC__)
static __inline size_t __cdecl _tcsnlen(const TCHAR *string, size_t maxlen)
{
	const TCHAR *p = string;
	while (maxlen-- && *p)
		p++;
	return p - string;
}
#endif

#ifdef _DEBUG
#include <assert.h>     // using assert
#endif

#if defined(_MSC_VER) || defined(__BORLANDC__)
#define inline __inline
#endif

#ifndef NEAR
#define NEAR
#endif
#ifndef FAR
#define FAR
#endif

// floating-point constants
#define LONGDOUBLE_IS_DOUBLE       (!defined(LDBL_MANT_DIG) || (LDBL_MANT_DIG == DBL_MANT_DIG))
#define LONGDOUBLE_IS_X86_EXTENDED (defined(LDBL_MANT_DIG) && (LDBL_MANT_DIG == 64))
#define LONGDOUBLE_IS_QUAD         (defined(LDBL_MANT_DIG) && (LDBL_MANT_DIG == 113))
#define DOUBLE_IS_IEEE754          (DBL_MANT_DIG == 53)

#ifndef LDBL_DECIMAL_DIG
#if LONGDOUBLE_IS_DOUBLE
#define LDBL_DECIMAL_DIG 17
#elif LONGDOUBLE_IS_X86_EXTENDED
#define LDBL_DECIMAL_DIG 21
#elif LONGDOUBLE_IS_QUAD
#define LDBL_DECIMAL_DIG 36
#endif
#endif

#if LONGDOUBLE_IS_DOUBLE
#ifndef LDBL_MANT_DIG
#define LDBL_MANT_DIG DBL_MANT_DIG
#endif
#ifndef LDBL_MAX_10_EXP
#define LDBL_MAX_10_EXP DBL_MAX_10_EXP
#endif
#ifndef LDBL_MAX_EXP
#define LDBL_MAX_EXP DBL_MAX_EXP
#endif
#ifndef LDBL_MIN_EXP
#define LDBL_MIN_EXP DBL_MIN_EXP
#endif
#endif

#if LONGDOUBLE_IS_DOUBLE
#define LDBL_BIT 64
#elif LONGDOUBLE_IS_X86_EXTENDED
#define LDBL_BIT 80
#elif LONGDOUBLE_IS_QUAD
#define LDBL_BIT 128
#endif

#define LDBL_SIGN_BIT 1
#if !LONGDOUBLE_IS_X86_EXTENDED
#define LDBL_NORM_BIT 1
#else
#define LDBL_NORM_BIT 0
#endif
#define LDBL_MANT_BIT (LDBL_MANT_DIG - LDBL_NORM_BIT)
#define LDBL_EXP_BIT  (LDBL_BIT - LDBL_SIGN_BIT - LDBL_MANT_BIT)
#define LDBL_EXP_BIAS (LDBL_MAX_EXP - 1)

// floating-point type definition
#if LONGDOUBLE_IS_DOUBLE
typedef double long_double;
#else
typedef long double long_double;
#endif

// floating-point structures
typedef union _LONGDOUBLE {
	long_double value;
	struct {
#if !LONGDOUBLE_IS_X86_EXTENDED
#if __BYTE_ORDER == __LITTLE_ENDIAN
		uintmax_t mantissa : LDBL_MANT_BIT;
		uintmax_t exponent : LDBL_EXP_BIT;
		uintmax_t sign     : LDBL_SIGN_BIT;
#else
		uintmax_t sign     : LDBL_SIGN_BIT;
		uintmax_t exponent : LDBL_EXP_BIT;
		uintmax_t mantissa : LDBL_MANT_BIT;
#endif
#elif __BYTE_ORDER == __LITTLE_ENDIAN
		uint64_t mantissa;
		struct {
			uint16_t exponent : LDBL_EXP_BIT;
			uint16_t sign     : LDBL_SIGN_BIT;
		};
#else
		struct {
			uint16_t sign     : LDBL_SIGN_BIT;
			uint16_t exponent : LDBL_EXP_BIT;
		};
		uint64_t mantissa;
#endif
	};
} LONGDOUBLE, NEAR *PLONGDOUBLE, FAR *LPLONGDOUBLE;

// floating-point binary operator macros
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define FLOAT64_SIGN_WORD(x)       *(uintptr_t *)((char *)&(x) + (sizeof(uint64_t) - sizeof(uintptr_t)))
#else
#define FLOAT64_SIGN_WORD(x)       *(uintptr_t *)&(x)
#endif
#define FLOAT64_SIGN_SWORD         *(intptr_t *)&FLOAT64_SIGN_WORD
#define FLOAT64_SIGN_MASK          (uintptr_t)(UINT64_C(0x8000000000000000) >> ((sizeof(uint64_t) - sizeof(uintptr_t)) * CHAR_BIT))
#define FLOAT64_EXP_MASK           (uintptr_t)(UINT64_C(0x7FF0000000000000) >> ((sizeof(uint64_t) - sizeof(uintptr_t)) * CHAR_BIT))
#define FLOAT64_EXP_WORD           FLOAT64_SIGN_WORD
#define FLOAT64_MANT_WORD(x)       *(uint64_t *)&(x)
#define FLOAT64_MANT_MASK          UINT64_C(0x000FFFFFFFFFFFFF)
#define FLOAT64_NORM_MASK          UINT64_C(0x0010000000000000)
#define FLOAT64_GET_SIGN(x)        (FLOAT64_SIGN_WORD(x) >> (INTPTR_BIT - 1))
#define FLOAT64_GET_EXP(x)         ((FLOAT64_EXP_WORD(x) & FLOAT64_EXP_MASK) >> (INTPTR_BIT - 12))
#define FLOAT64_GET_MANT(x)        ((FLOAT64_MANT_WORD(x) & FLOAT64_MANT_MASK) | ((FLOAT64_EXP_WORD(x) & FLOAT64_EXP_MASK) ? FLOAT64_NORM_MASK : 0))
#define FLOAT64_SET_SIGN(x, sign)  (FLOAT64_SIGN_WORD(x) = (FLOAT64_SIGN_WORD(x) & ~FLOAT64_SIGN_MASK) | (((uintptr_t)(sign) << (INTPTR_BIT - 1)) & FLOAT64_SIGN_MASK))
#define FLOAT64_SET_EXP(x, exp)    (FLOAT64_EXP_WORD(x) = (FLOAT64_EXP_WORD(x) & ~FLOAT64_EXP_MASK) | (((uintptr_t)(exp) << (INTPTR_BIT - 12)) & FLOAT64_EXP_MASK))
#define FLOAT64_SET_MANT(x, mant)  (FLOAT64_MANT_WORD(x) = (FLOAT64_MANT_WORD(x) & ~FLOAT64_MANT_MASK) | ((uint64_t)(mant) & FLOAT64_MANT_MASK))

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define FLOAT80_SIGN_WORD(x)       *(uint16_t *)&((uint64_t *)&(x))[1]
#define FLOAT80_MANT_WORD(x)       *(uint64_t *)&(x)
#else
#define FLOAT80_SIGN_WORD(x)       *(uint16_t *)&(x)
#define FLOAT80_MANT_WORD(x)       *(uint64_t *)&((uint16_t *)&(x))[1]
#endif
#define FLOAT80_SIGN_SWORD         *(int16_t *)&FLOAT80_SIGN_WORD
#define FLOAT80_EXP_WORD           FLOAT80_SIGN_WORD
#define FLOAT80_SIGN_MASK          UINT16_C(0x8000)
#define FLOAT80_EXP_MASK           UINT16_C(0x7FFF)
#define FLOAT80_MANT_MASK          UINT64_C(0xFFFFFFFFFFFFFFFF)
#define FLOAT80_GET_SIGN(x)        (FLOAT80_SIGN_WORD(x) >> 15)
#define FLOAT80_GET_EXP(x)         (FLOAT80_EXP_WORD(x) & FLOAT80_EXP_MASK)
#define FLOAT80_GET_MANT           FLOAT80_MANT_WORD
#define FLOAT80_SET_SIGN(x, sign)  (FLOAT80_SIGN_WORD(x) = (FLOAT80_EXP_WORD(x) & FLOAT80_EXP_MASK) | (((uint16_t)(sign) << 15) & FLOAT80_SIGN_MASK))
#define FLOAT80_SET_EXP(x, exp)    (FLOAT80_EXP_WORD(x) = (FLOAT80_EXP_WORD(x) & FLOAT80_SIGN_MASK) | ((uint16_t)(exp) & FLOAT80_EXP_MASK))
#define FLOAT80_SET_MANT(x, mant)  (FLOAT80_MANT_WORD(x) = (uint64_t)(mant))

#ifdef INT128_MAX
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define FLOAT128_SIGN_WORD(x)      *(uintptr_t *)((char *)&(x) + (sizeof(uint128_t) - sizeof(uintptr_t)))
#else
#define FLOAT128_SIGN_WORD(x)      *(uintptr_t *)&(x)
#endif
#define FLOAT128_SIGN_SWORD        *(intptr_t *)&FLOAT128_SIGN_WORD
#define FLOAT128_SIGN_MASK         (uintptr_t)(UINT128_C(0x80000000000000000000000000000000) >> ((sizeof(uint128_t) - sizeof(uintptr_t)) * CHAR_BIT))
#define FLOAT128_EXP_MASK          (uintptr_t)(UINT128_C(0x7FFF0000000000000000000000000000) >> ((sizeof(uint128_t) - sizeof(uintptr_t)) * CHAR_BIT))
#define FLOAT128_EXP_WORD          FLOAT128_SIGN_WORD
#define FLOAT128_MANT_WORD(x)      *(uint128_t *)&(x)
#define FLOAT128_MANT_MASK         UINT128_C(0x0000FFFFFFFFFFFFFFFFFFFFFFFFFFFF)
#define FLOAT128_NORM_MASK         UINT128_C(0x00010000000000000000000000000000)
#define FLOAT128_GET_SIGN(x)       (FLOAT128_SIGN_WORD(x) >> (INTPTR_BIT - 1))
#define FLOAT128_GET_EXP(x)        (FLOAT128_EXP_WORD(x) & FLOAT128_EXP_MASK)
#define FLOAT128_GET_MANT(x)       ((*(uint128_t *)&(x) & FLOAT128_MANT_MASK) | (FLOAT128_GET_EXP(x) ? FLOAT128_NORM_MASK : 0))
#define FLOAT128_SET_SIGN(x, sign) (FLOAT128_SIGN_WORD(x) = (FLOAT128_SIGN_WORD(x) & ~FLOAT128_SIGN_MASK) | (((uintptr_t)(sign) << (INTPTR_BIT - 1)) & FLOAT128_SIGN_MASK))
#define FLOAT128_SET_EXP(x, exp)   (FLOAT128_EXP_WORD(x) = (FLOAT128_EXP_WORD(x) & ~FLOAT128_EXP_MASK) | ((uintptr_t)(exp) & FLOAT80_EXP_MASK))
#define FLOAT128_SET_MANT(x, mant) (*(uint128_t *)&(x) = (*(uint128_t *)&(x) & ~FLOAT128_MANT_MASK) | ((uint128_t)(mant) & FLOAT128_MANT_MASK))
#endif

#define DBL_SIGN_WORD              FLOAT64_SIGN_WORD
#define DBL_SIGN_SWORD             FLOAT64_SIGN_SWORD
#define DBL_EXP_WORD               FLOAT64_EXP_WORD
#define DBL_MANT_WORD              FLOAT64_MANT_WORD
#define DBL_SIGN_MASK              FLOAT64_SIGN_MASK
#define DBL_EXP_MASK               FLOAT64_EXP_MASK
#define DBL_MANT_MASK              FLOAT64_MANT_MASK
#define DBL_GET_SIGN               FLOAT64_GET_SIGN
#define DBL_GET_EXP                FLOAT64_GET_EXP
#define DBL_GET_MANT               FLOAT64_GET_MANT
#define DBL_SET_SIGN               FLOAT64_SET_SIGN
#define DBL_SET_EXP                FLOAT64_SET_EXP
#define DBL_SET_MANT               FLOAT64_SET_MANT

#if LONGDOUBLE_IS_DOUBLE
#define LDBL_SIGN_WORD             FLOAT64_SIGN_WORD
#define LDBL_SIGN_SWORD            FLOAT64_SIGN_SWORD
#define LDBL_EXP_WORD              FLOAT64_EXP_WORD
#define LDBL_MANT_WORD             FLOAT64_MANT_WORD
#define LDBL_SIGN_MASK             FLOAT64_SIGN_MASK
#define LDBL_EXP_MASK              FLOAT64_EXP_MASK
#define LDBL_MANT_MASK             FLOAT64_MANT_MASK
#define LDBL_GET_SIGN              FLOAT64_GET_SIGN
#define LDBL_GET_EXP               FLOAT64_GET_EXP
#define LDBL_GET_MANT              FLOAT64_GET_MANT
#define LDBL_SET_SIGN              FLOAT64_SET_SIGN
#define LDBL_SET_EXP               FLOAT64_SET_EXP
#define LDBL_SET_MANT              FLOAT64_SET_MANT
#elif LONGDOUBLE_IS_X86_EXTENDED
#define LDBL_SIGN_WORD             FLOAT80_SIGN_WORD
#define LDBL_SIGN_SWORD            FLOAT80_SIGN_SWORD
#define LDBL_EXP_WORD              FLOAT80_EXP_WORD
#define LDBL_MANT_WORD             FLOAT80_MANT_WORD
#define LDBL_SIGN_MASK             FLOAT80_SIGN_MASK
#define LDBL_EXP_MASK              FLOAT80_EXP_MASK
#define LDBL_MANT_MASK             FLOAT80_MANT_MASK
#define LDBL_GET_SIGN              FLOAT80_GET_SIGN
#define LDBL_GET_EXP               FLOAT80_GET_EXP
#define LDBL_GET_MANT              FLOAT80_GET_MANT
#define LDBL_SET_SIGN              FLOAT80_SET_SIGN
#define LDBL_SET_EXP               FLOAT80_SET_EXP
#define LDBL_SET_MANT              FLOAT80_SET_MANT
#elif LONGDOUBLE_IS_QUAD
#define LDBL_SIGN_WORD             FLOAT128_SIGN_WORD
#define LDBL_SIGN_SWORD            FLOAT128_SIGN_SWORD
#define LDBL_EXP_WORD              FLOAT128_EXP_WORD
#define LDBL_MANT_WORD             FLOAT128_MANT_WORD
#define LDBL_SIGN_MASK             FLOAT128_SIGN_MASK
#define LDBL_EXP_MASK              FLOAT128_EXP_MASK
#define LDBL_MANT_MASK             FLOAT128_MANT_MASK
#define LDBL_GET_SIGN              FLOAT128_GET_SIGN
#define LDBL_GET_EXP               FLOAT128_GET_EXP
#define LDBL_GET_MANT              FLOAT128_GET_MANT
#define LDBL_SET_SIGN              FLOAT128_SET_SIGN
#define LDBL_SET_EXP               FLOAT128_SET_EXP
#define LDBL_SET_MANT              FLOAT128_SET_MANT
#endif

// floating-point macro function
#undef isfinitel
#define isfinitel(x) ((LDBL_EXP_WORD(x) & LDBL_EXP_MASK) != LDBL_EXP_MASK)
#undef isinfl
#define isinfl(x) (!isfinitel(x) && !(LDBL_MANT_WORD(x) & LDBL_MANT_MASK))
#undef isnanl
#define isnanl(x) (!isfinitel(x) && (LDBL_MANT_WORD(x) & LDBL_MANT_MASK))
#undef signbitl
#define signbitl(x) (LDBL_SIGN_SWORD(x) < 0)

#if LONGDOUBLE_IS_DOUBLE
#undef modfl
#define modfl modf
#undef floorl
#define floorl floor
#undef log10l
#define log10l log10
#undef exp10l
#define exp10l exp10
#endif

// mathematical constants
#ifndef M_LOG10_2
#define M_LOG10_2 0.301029995663981195213738894724	// log10(2), ln(2)/ln(10)
#endif

// mathematical constant value macro funcion
#define ABS(x) ((x) >= 0 ? (x) : -(x))
#define TRUNC(x) (ABS(x) >= 1 ? ABS(x) < 0x0010000000000000 ? (double)(int64_t)(x) : (double)(x) : 0.0)
#define CEIL(x) (TRUNC(x) + ((double)(x) > TRUNC(x)))

// get number of digits, it is constant value macro funcion
#define DEC_DIG(bit) (int32_t)CEIL((bit) * M_LOG10_2)
#define OCT_DIG(bit) (((bit) + (3 - 1)) / 3)
#define HEX_DIG(bit) (((bit) + (4 - 1)) / 4)

// constants
#define UINTMAX_OCT_DIG  OCT_DIG(sizeof(uintmax_t) * CHAR_BIT)
#define MANTISSA_HEX_DIG HEX_DIG(LDBL_MANT_BIT)
#define CVTBUFSIZE       ((LDBL_MAX_10_EXP + 1) + 40)
#define EXPBUFSIZE       (2 + DEC_DIG(LDBL_EXP_BIT - 1) + 1)

// the other macro funcion
#define CONCATIFY(a, b) a##b
#define CONCAT(a, b)    CONCATIFY(a, b)

/* Format flags. */
#define FL_SIGN         0x0001  /* put plus or minus in front */
#define FL_SIGNSP       0x0002  /* put space or minus in front */
#define FL_LEFT         0x0004  /* left justify */
#define FL_LEADZERO     0x0008  /* pad with leading zeros */
#define FL_ALTERNATE    0x0010  /* alternate form requested */
#define FL_QUOTE        0x0020
#define FL_UP           0x0040
#define FL_UNSIGNED     0x0080
#define FL_TYPE_E       0x0100
#define FL_TYPE_G       0x0200
#define FL_TYPE_A       0x0400

/* Conversion flags. */
enum {
	C_DEFAULT,
	C_CHAR,
	C_SHORT,
	C_LONG,
	C_LLONG,
#if !INTMAX_IS_LLONG
	C_INTMAX,
#endif
	C_LDOUBLE,
};

#if INTMAX_IS_LLONG
#define C_INTMAX C_LLONG
#endif

#define C_UNSIGNED (1 << CHAR_BIT)

#if INTPTR_IS_CHAR
#define C_PTRDIFF C_CHAR
#elif INTPTR_IS_SHRT
#define C_PTRDIFF C_SHORT
#elif INTPTR_IS_LONG
#define C_PTRDIFF C_LONG
#elif INTPTR_IS_LLONG
#define C_PTRDIFF C_LLONG
#else
#define C_PTRDIFF C_INTMAX
#endif

#define C_SIZE (C_UNSIGNED | C_PTRDIFF)

#ifdef _WIN32
#define C_WCHAR (C_UNSIGNED | C_SHORT)
#endif

// intrinsic functions
#if defined(_MSC_VER) && _MSC_VER >= 1310
#include <intrin.h>
#pragma intrinsic(_addcarry_u32)
#pragma intrinsic(_subborrow_u32)
#define _add_u32(a, b, out) _addcarry_u32(0, a, b, out)
#define _sub_u32(a, b, out) _subborrow_u32(0, a, b, out)
#else
static inline bool _add_u32(uint32_t a, uint32_t b, uint32_t *out) { return (*out = a + b) < b; }
static inline bool _sub_u32(uint32_t a, uint32_t b, uint32_t *out) { return (*out = a - b) > a; }
#endif

// macro functions
#ifndef ISDIGIT
#define ISDIGIT(c) ((c) >= '0' && (c) <= '9')
#endif

#define OUTCHAR(c)                        \
    if (length != -1 && ++length < count) \
        buffer[length - 1] = c

#define OUTCHAR_OR_BREAK(c, break_process) \
    if (!++length || length >= count) {    \
        --length;                          \
        break_process;                     \
        break;                             \
    } else                                 \
        buffer[length - 1] = c

#define INC_LENGTH() do { if (!++length) --length; } while (0)
#define ADD_LENGTH(x) do { if (_add_u32(length, x, &length)) length = -1; } while (0)
#define SUB_LENGTH(x) do { if (!_sub_u32(length, x, &length)) length = -1; } while (0)

// internal variables
#ifndef _MSC_VER
static const char digitsLarge[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
static const char digitsSmall[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
#else
extern const char digitsLarge[36];
extern const char digitsSmall[36];
#endif
static const TCHAR lpcszNull   [] = TEXT("(null)");
#ifndef _WIN32
static const TCHAR lpcszNil    [] = TEXT("(nil)");
#endif
static const TCHAR lpcszNan    [] = TEXT("nan");
#ifdef _WIN32
static const TCHAR lpcszNanSnan[] = TEXT("nan(snan)");
static const TCHAR lpcszNanInd [] = TEXT("nan(ind)");
#endif
static const TCHAR lpcszInf    [] = TEXT("inf");

// external functions
#ifdef _MSC_VER
#ifdef _UNICODE
size_t __fastcall _ui32to10w(uint32_t value, wchar_t *buffer);
size_t __fastcall _ui64to10w(uint64_t value, wchar_t *buffer);
size_t __fastcall _ui64to16w(uint64_t value, wchar_t *buffer, BOOL upper);
size_t __fastcall _ui64to8w(uint64_t value, wchar_t *buffer);
#define _ui32to10t _ui32to10w
#define _ui64to10t _ui64to10w
#define _ui64to16t _ui64to16w
#define _ui64to8t  _ui64to8w
#else
size_t __fastcall _ui32to10a(uint32_t value, char *buffer);
size_t __fastcall _ui64to10a(uint64_t value, char *buffer);
size_t __fastcall _ui64to16a(uint64_t value, char *buffer, BOOL upper);
size_t __fastcall _ui64to8a(uint64_t value, char *buffer);
#define _ui32to10t _ui32to10a
#define _ui64to10t _ui64to10a
#define _ui64to16t _ui64to16a
#define _ui64to8t  _ui64to8a
#endif
#if defined(_M_IX86) && LONGDOUBLE_IS_DOUBLE
double __cdecl ldexp10(double x, int e);
#endif
#endif

// internal functions
static uint32_t tcsfmt(TCHAR *, uint32_t, uint32_t, const TCHAR *, uint32_t, int32_t, int);
static uint32_t intfmt(TCHAR *, uint32_t, uint32_t, intmax_t, uint32_t, uint32_t, int32_t, int);
static uint32_t fltfmt(TCHAR *, uint32_t, uint32_t, long_double, uint32_t, int32_t, int);

int __cdecl _vsntprintf(TCHAR *buffer, size_t count, const TCHAR *format, va_list argptr)
{
	uint32_t length;
	TCHAR    c;

	/*
	 * C99 says: "If `n' is zero, nothing is written, and `s' may be a null
	 * pointer." (7.19.6.5, 2)  We're forgiving and allow a NULL pointer
	 * even if a count larger than zero was specified.  At least NetBSD's
	 * snprintf(3) does the same, as well as other versions of this file.
	 * (Though some of these versions will write to a non-NULL buffer even
	 * if a count of zero was specified, which violates the standard.)
	 */
	if (!buffer)
		count = 0;
#if SIZE_MAX > UINT32_MAX
	else if (count > UINT32_MAX)
		count = UINT32_MAX;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	#define count (*(uint32_t *)&count)
#else
	#define count (*(uint32_t *)((char *)&count + sizeof(size_t) - sizeof(uint32_t)))
#endif
#endif

	length = 0;
	while (c = *(format++))
	{
		int      flags;
		uint32_t width;
		int32_t  precision;
		int      cflags;
		uint32_t base;

		if (c != '%')
		{
#ifndef _UNICODE
			if (isleadbyte(c))
			{
				OUTCHAR(c);
				if (!(c = *(format++)))
					break;
			}
#endif
			OUTCHAR(c);
			continue;
		}

		// Process flags
		flags = 0;
		for (; ; )
		{
			switch (c = *(format++))
			{
			case '-':
				flags |= FL_LEFT;
				continue;
			case '+':
				flags |= FL_SIGN;
				continue;
			case ' ':
				flags |= FL_SIGNSP;
				continue;
			case '#':
				flags |= FL_ALTERNATE;
				continue;
			case '0':
				flags |= FL_LEADZERO;
				continue;
			case '\'':
				/* SUSv2 flag (not in C99). */
				flags |= FL_QUOTE;
				continue;
			}
			break;
		}

		// Get field width
		if (c != '*')
		{
			width = 0;
			while (ISDIGIT(c))
			{
				if ((int32_t)(width = width * 10 + c - '0') >= 0)
				{
					c = *(format++);
				}
				else
				{
					if (++length && count > length)
						count = length;
					length = -1;
					goto NESTED_BREAK;
				}
			}
		}
		else
		{
			/*
			 * C99 says: "A negative field width argument is
			 * taken as a `-' flag followed by a positive
			 * field width." (7.19.6.1, 5)
			 */
			if ((int32_t)(width = va_arg(argptr, int)) < 0)
			{
				width = -(int32_t)width;
				flags |= FL_LEFT;
			}
			c = *(format++);
		}

		// Get precision
		if (c != '.')
		{
			precision = -1;
		}
		else if ((c = *(format++)) != '*')
		{
			precision = 0;
			while (ISDIGIT(c))
			{
				if ((precision = (uint32_t)precision * 10 + c - '0') >= 0)
				{
					c = *(format++);
				}
				else
				{
					if (++length && count > length)
						count = length;
					length = -1;
					goto NESTED_BREAK;
				}
			}
		}
		else
		{
			/*
			 * C99 says: "A negative precision argument is
			 * taken as if the precision were omitted."
			 * (7.19.6.1, 5)
			 */
			if ((precision = va_arg(argptr, int)) < 0)
				precision = -1;
			c = *(format++);
		}

		// Get the conversion qualifier
		switch (c)
		{
		case 'h':
			if ((c = *(format++)) == 'h')
			{
				/* It's a char. */
				cflags = C_CHAR;
				c = *(format++);
			}
			else
				cflags = C_SHORT;
			break;
		case 'l':
			if ((c = *(format++)) != 'l')
			{
				cflags = C_LONG;
				break;
			}
#if !INTMAX_IS_LLONG
			/* It's a long long. */
			cflags = C_LLONG;
			c = *(format++);
			break;
#endif
		case 'j':
			cflags = C_INTMAX;
			c = *(format++);
			break;
		case 'z':
			cflags = C_SIZE;
			c = *(format++);
			break;
		case 't':
			cflags = C_PTRDIFF;
			c = *(format++);
			break;
		case 'L':
			cflags = C_LDOUBLE;
			c = *(format++);
			break;
#ifdef _WIN32
		case 'I':
			if ((c = *(format++)) == '6')
			{
				if (*format == '4')
				{
					cflags = C_LLONG;
					c = format[1];
					format += 2;
					break;
				}
			}
			else if (c == '3')
			{
				if (*format == '2')
				{
					cflags = C_LONG;
					c = format[1];
					format += 2;
					break;
				}
			}
			cflags = C_PTRDIFF;
			break;
		case 'w':
			cflags = C_WCHAR;
			c = *(format++);
			break;
#endif
		default:
			cflags = C_DEFAULT;
			break;
		}

		switch (c)
		{
		case '\0':
			goto NESTED_BREAK;
		case 'd':
			/* FALLTHROUGH */
		case 'i':
			{
				intmax_t value;

				switch (cflags)
				{
#if !INT_IS_CHAR
				case C_CHAR:
					value = (char)va_arg(argptr, int);
					break;
#endif
#if !INT_IS_SHRT
				case C_SHORT:
					value = (short)va_arg(argptr, int);
					break;
#endif
#if !INT_IS_LONG
				case C_LONG:
					value = va_arg(argptr, long);
					break;
#endif
#if !INT_IS_LLONG
				case C_LLONG:
					value = va_arg(argptr, long_long);
					break;
#endif
#if !INT_IS_INTMAX && !INTMAX_IS_LLONG
				case C_INTMAX:
					value = va_arg(argptr, intmax_t);
					break;
#endif
				case C_SIZE:
					value = va_arg(argptr, size_t);
					break;
				default:
					value = va_arg(argptr, int);
					break;
				}
				length = intfmt(buffer, count, length, value, 10, width, precision, flags);
			}
			break;
		case 'X':
			flags |= FL_UP;
			/* FALLTHROUGH */
		case 'x':
			base = 16;
			goto PUT_UNSIGNED;
		case 'o':
			base = 8;
			goto PUT_UNSIGNED;
		case 'u':
			base = 10;
		PUT_UNSIGNED:
			flags |= FL_UNSIGNED;
			{
				intmax_t value;

				switch ((unsigned char)cflags)
				{
#if !INT_IS_CHAR
				case C_CHAR:
					value = (unsigned char)va_arg(argptr, int);
					break;
#endif
#if !INT_IS_SHRT
				case C_SHORT:
					value = (unsigned short)va_arg(argptr, int);
					break;
#endif
#if !INT_IS_LONG
				case C_LONG:
					value = va_arg(argptr, unsigned long);
					break;
#endif
#if !INT_IS_LLONG
				case C_LLONG:
					value = va_arg(argptr, unsigned_long_long);
					break;
#endif
#if !INT_IS_INTMAX && !INTMAX_IS_LLONG
				case C_INTMAX:
					value = va_arg(argptr, uintmax_t);
					break;
#endif
				default:
					value = va_arg(argptr, unsigned int);
					break;
				}
				length = intfmt(buffer, count, length, value, base, width, precision, flags);
			}
			break;
		case 'F':
			flags |= FL_UP;
			/* FALLTHROUGH */
		case 'f':
			{
				long_double f;

#if !LONGDOUBLE_IS_DOUBLE
				if (cflags == C_LDOUBLE)
					f = va_arg(argptr, long_double);
				else
#endif
					f = va_arg(argptr, double);
				length = fltfmt(buffer, count, length, f, width, precision, flags);
			}
			break;
		case 'E':
			flags |= FL_UP;
			/* FALLTHROUGH */
		case 'e':
			flags |= FL_TYPE_E;
			{
				long_double f;

#if !LONGDOUBLE_IS_DOUBLE
				if (cflags == C_LDOUBLE)
					f = va_arg(argptr, long_double);
				else
#endif
					f = va_arg(argptr, double);
				length = fltfmt(buffer, count, length, f, width, precision, flags);
			}
			break;
		case 'G':
			flags |= FL_UP;
			/* FALLTHROUGH */
		case 'g':
			flags |= FL_TYPE_G;
			{
				long_double f;

#if !LONGDOUBLE_IS_DOUBLE
				if (cflags == C_LDOUBLE)
					f = va_arg(argptr, long_double);
				else
#endif
					f = va_arg(argptr, double);
				length = fltfmt(buffer, count, length, f, width, precision, flags);
			}
			break;
		case 'A':
			flags |= FL_UP;
			/* FALLTHROUGH */
		case 'a':
			flags |= FL_TYPE_A;
			{
				long_double f;

#if !LONGDOUBLE_IS_DOUBLE
				if (cflags == C_LDOUBLE)
					f = va_arg(argptr, long_double);
				else
#endif
					f = va_arg(argptr, double);
				length = fltfmt(buffer, count, length, f, width, precision, flags);
			}
			break;
		case 'c':
#ifdef _WIN32
#ifdef _UNICODE
			if (cflags == C_SHORT || cflags == C_CHAR)
				goto PUT_CHAR;
		PUT_WCHAR:
#else
			if (cflags == C_LONG || cflags == C_WCHAR)
				goto PUT_WCHAR;
		PUT_CHAR:
#endif
#endif
			{
				TCHAR cbuf[2];

				cbuf[0] = (TCHAR)va_arg(argptr, int);
				cbuf[1] = '\0';
				length = tcsfmt(buffer, count, length, cbuf, width, precision, flags);
			}
			break;
#ifdef _WIN32
		case 'C':
#ifdef _UNICODE
			if (cflags == C_LONG || cflags == C_WCHAR)
				goto PUT_WCHAR;
		PUT_CHAR:
			{
				wchar_t cbuf[2];

				cbuf[0] = (char)va_arg(argptr, int);
				cbuf[1] = '\0';
				length = tcsfmt(buffer, count, length, cbuf, width, precision, flags);
			}
#else
			if (cflags == C_SHORT || cflags == C_CHAR)
				goto PUT_CHAR;
		PUT_WCHAR:
			{
				wchar_t w;
				int     i;
				char    cbuf[3];

				w = (wchar_t)va_arg(argptr, int);
				i = WideCharToMultiByte(CP_THREAD_ACP, 0, &w, 1, cbuf, 2, NULL, NULL);
				if (!i)
					break;
				cbuf[i] = '\0';
				length = tcsfmt(buffer, count, length, cbuf, width, precision, flags);
			}
#endif
			break;
#endif
		case 's':
#ifdef _WIN32
#ifdef _UNICODE
			if (cflags == C_SHORT || cflags == C_CHAR)
				goto PUT_STR;
		PUT_WSTR:
#else
			if (cflags == C_LONG || cflags == C_WCHAR)
				goto PUT_WSTR;
		PUT_STR:
#endif
#endif
			length = tcsfmt(buffer, count, length, va_arg(argptr, TCHAR *), width, precision, flags);
			break;
#ifdef _WIN32
		case 'S':
#ifdef _UNICODE
			if (cflags == C_LONG || cflags == C_WCHAR)
				goto PUT_WSTR;
		PUT_STR:
			{
				wchar_t *ws;
				int     i;
				char    *s;
				HANDLE  handle;

				ws = NULL;
				i = 0;
				s = va_arg(argptr, char *);
				if (s)
					if (i = MultiByteToWideChar(CP_THREAD_ACP, 0, s, -1, NULL, 0))
						if (ws = (wchar_t *)HeapAlloc(handle = GetProcessHeap(), 0, ++i * sizeof(wchar_t)))
							i = MultiByteToWideChar(CP_THREAD_ACP, 0, s, -1, ws, i);
				length = tcsfmt(buffer, count, length, i ? ws : NULL, width, precision, flags);
				if (ws)
					HeapFree(handle, 0, ws);
			}
#else
			if (cflags == C_SHORT || cflags == C_CHAR)
				goto PUT_STR;
		PUT_WSTR:
			{
				char    *s;
				int     i;
				wchar_t *ws;
				HANDLE  handle;

				s = NULL;
				i = 0;
				ws = va_arg(argptr, wchar_t *);
				if (ws)
					if (i = WideCharToMultiByte(CP_THREAD_ACP, 0, ws, -1, NULL, 0, NULL, NULL))
						if (s = (char *)HeapAlloc(handle = GetProcessHeap(), 0, ++i))
							i = WideCharToMultiByte(CP_THREAD_ACP, 0, ws, -1, s, i, NULL, NULL);
				length = tcsfmt(buffer, count, length, i ? s : NULL, width, precision, flags);
				if (s)
					HeapFree(handle, 0, s);
			}
#endif
			break;
#endif
		case 'p':
			{
				char *s;

				s = va_arg(argptr, void *);
#ifndef _WIN32
				/*
				 * C99 says: "The value of the pointer is
				 * converted to a sequence of printing
				 * characters, in an implementation-defined
				 * manner." (C99: 7.19.6.1, 8)
				 */
				if (!s)
				{
					/*
					 * We use the glibc format.  BSD prints
					 * "0x0", SysV "0".
					 */
					length = tcsfmt(buffer, count, length, lpcszNil, width, -1, flags);
				}
				else
				{
					/*
					 * We use the BSD/glibc format.  SysV
					 * omits the "0x" prefix (which we emit
					 * using the FL_ALTERNATE flag).
					 */
					flags |= FL_UNSIGNED | FL_UP | FL_ALTERNATE;
					length = intfmt(buffer, count, length, (uintptr_t)s, 16, width, sizeof(void *) * 2, flags);
				}
#else
				flags |= FL_UNSIGNED | FL_UP;
				length = intfmt(buffer, count, length, (uintptr_t)s, 16, width, sizeof(void *) * 2, flags);
#endif
			}
			break;
		case 'n':
			switch ((unsigned char)cflags)
			{
#if !INT_IS_CHAR
			case C_CHAR:
				*va_arg(argptr, char *) = (char)length;
				break;
#endif
#if !INT_IS_SHRT
			case C_SHORT:
				*va_arg(argptr, short *) = (short)length;
				break;
#endif
#if !INT_IS_LONG
			case C_LONG:
				*va_arg(argptr, long *) = (long)length;
				break;
#endif
#if !INT_IS_LLONG
			case C_LLONG:
				*va_arg(argptr, long_long *) = (long_long)length;
				break;
#endif
#if !INT_IS_INTMAX && !INTMAX_IS_LLONG
			case C_INTMAX:
				*va_arg(argptr, intmax_t *) = length;
				break;
#endif
			default:
				*va_arg(argptr, int *) = (int)length;
				break;
			}
			break;
#ifdef _WIN32
		case 'Z':
			switch (cflags)
			{
#ifdef _UNICODE
			case C_SHORT:
			case C_CHAR:
				{
					wchar_t      *ws;
					int          i;
					PANSI_STRING as;
					HANDLE       handle;

					ws = NULL;
					i = 0;
					as = va_arg(argptr, PANSI_STRING);
					if (as && as->Buffer)
						if (ws = (wchar_t *)HeapAlloc(handle = GetProcessHeap(), 0, i = (unsigned int)as->Length + 1))
							i = MultiByteToWideChar(CP_THREAD_ACP, 0, as->Buffer, -1, ws, i);
					length = tcsfmt(buffer, count, length, i ? ws : NULL, width, precision, flags);
					if (ws)
						HeapFree(handle, 0, ws);
				}
				break;
			default:
				{
					PUNICODE_STRING us;

					us = va_arg(argptr, PUNICODE_STRING);
					length = tcsfmt(buffer, count, length, us ? us->Buffer : NULL, width, precision, flags);
				}
				break;
#else
			case C_LONG:
			case C_WCHAR:
				{
					char            *s;
					int             i;
					PUNICODE_STRING us;
					HANDLE          handle;

					s = NULL;
					i = 0;
					us = va_arg(argptr, PUNICODE_STRING);
					if (us && us->Buffer)
						if (s = (char *)HeapAlloc(handle = GetProcessHeap(), 0, i = ((unsigned int)us->Length + 1) * 2))
							i = WideCharToMultiByte(CP_THREAD_ACP, 0, us->Buffer, -1, s, i, NULL, NULL);
					length = tcsfmt(buffer, count, length, i ? s : NULL, width, precision, flags);
					if (s)
						HeapFree(handle, 0, s);
				}
				break;
			default:
				{
					PANSI_STRING as;

					as = va_arg(argptr, PANSI_STRING);
					length = tcsfmt(buffer, count, length, as ? as->Buffer : NULL, width, precision, flags);
				}
				break;
#endif
			}
			break;
#endif
		case '%':
			/* Print a "%" character verbatim. */
			OUTCHAR(c);
			break;
#ifndef _UNICODE
		default:
			/* Skip other characters. */
			if (!isleadbyte(c) || *(format++))
				break;
			else
				goto NESTED_BREAK;
#endif
		}
	}
NESTED_BREAK:
	if (length < count)
	{
		buffer[length] = '\0';
		if ((int32_t)length < 0)
			errno = ERANGE;
	}
	else
	{
		if (count)
			buffer[count - 1] = '\0';
		errno = EOVERFLOW;
	}
	return (int)length;

#if SIZE_MAX > UINT32_MAX
	#undef count
#endif
}

static uint32_t tcsfmt(TCHAR *buffer, uint32_t count, uint32_t length, const TCHAR *src, uint32_t width, int32_t precision, int flags)
{
	uint32_t srclen;
	uint32_t padlen;	/* Amount to pad. */

#ifdef _DEBUG
	assert(width     <= INT32_MAX);
	assert(precision >= -1       );
	assert(precision <= INT32_MAX);
#endif

	/* We're forgiving. */
	if (!src)
		src = lpcszNull;

	srclen = (uint32_t)_tcsnlen(src, (uint32_t)max(precision, -1));
	if (_sub_u32(width, srclen, &padlen))
		padlen = 0;

	/* Leading spaces. */
	if (padlen && !(flags & FL_LEFT))
		do
			OUTCHAR_OR_BREAK(' ', ADD_LENGTH(padlen); padlen = 0);
		while (--padlen);

	if (srclen)
		do
			OUTCHAR_OR_BREAK(*(src++), ADD_LENGTH(srclen));
		while (--srclen);

	/* Trailing spaces. */
	if (padlen)
		do
			OUTCHAR_OR_BREAK(' ', ADD_LENGTH(padlen));
		while (--padlen);

	return length;
}

static inline uint32_t intcvt(uintmax_t value, TCHAR *buffer, uint32_t base, int flags)
{
#if defined(_MSC_VER) && UINTMAX_MAX == UINT64_MAX
	return (uint32_t)(
		base == 10 ? _ui64to10t(value, buffer) :
		base == 16 ? _ui64to16t(value, buffer, flags & FL_UP) :
		_ui64to8t(value, buffer));
#else
	TCHAR *dest;
	TCHAR *p1, *p2;
	TCHAR c1, c2;

#ifdef _DEBUG
	assert(base == 10 || base == 16 || base == 8);
#endif

	dest = buffer;
	if (base == 10)
	{
		do
			*(dest++) = (TCHAR)(value % 10) + '0';
		while (value /= 10);
	}
	else if (base == 16)
	{
		const unsigned char *digits;

		digits = (flags & FL_UP) ? digitsLarge : digitsSmall;
		do
			*(dest++) = (TCHAR)digits[(size_t)value & 0x0F];
		while (value >>= 4);
	}
	else // if (base == 8)
	{
		do
			*(dest++) = ((TCHAR)value & 0x07) + '0';
		while (value >>= 3);
	}
	p1 = buffer;
	p2 = dest - 1;
	while (p1 < p2)
	{
		c1 = *p1;
		c2 = *p2;
		*(p1++) = c2;
		*(p2--) = c1;
	}
	return dest - buffer;
#endif
}

#define GETNUMSEP(digits) \
	(((uint32_t)(digits) - 1) / 3)

static uint32_t intfmt(TCHAR *buffer, uint32_t count, uint32_t length, intmax_t value, uint32_t base, uint32_t width, int32_t precision, int flags)
{
	TCHAR    icvtbuf[UINTMAX_OCT_DIG + 1];
	TCHAR    sign;
	TCHAR    hexprefix;
	uint32_t spadlen;	/* Amount to space pad. */
	uint32_t zpadlen;	/* Amount to zero pad. */
	uint32_t pos;
	uint32_t separators;
	bool     noprecision;
	int32_t  i;

#ifdef _DEBUG
	assert(width     <= INT32_MAX);
	assert(precision >= -1       );
	assert(precision <= INT32_MAX);
#endif

	if (flags & FL_UNSIGNED)
	{
		sign = '\0';
	}
	else if (value >= 0)
	{
		sign =
			!(flags & (FL_SIGN | FL_SIGNSP)) ?
				'\0' :
				(flags & FL_SIGN) ?
					'+' :
					' ';
	}
	else
	{
		sign = '-';
		value = -value;
	}

	pos = intcvt(value, icvtbuf, base, flags);

	hexprefix = '\0';
	noprecision = (precision < 0);
	if (flags & FL_ALTERNATE)
	{
		/*
		 * C99 says: "The result is converted to an `alternative form'.
		 * For `o' conversion, it increases the precision, if and only
		 * if necessary, to force the first digit of the result to be a
		 * zero (if the value and precision are both 0, a single 0 is
		 * printed).  For `x' (or `X') conversion, a nonzero result has
		 * `0x' (or `0X') prefixed to it." (7.19.6.1, 6)
		 */
		switch (base)
		{
		case 8:
			if (value && precision <= (int32_t)pos)
				precision = pos + 1;
			break;
		case 16:
			hexprefix = (flags & FL_UP) ? 'X' : 'x';
			break;
		}
	}

	/* Get the number of group separators we'll print. */
	separators = ((flags & FL_QUOTE) && pos) ? GETNUMSEP(pos) : 0;

	if ((int32_t)(zpadlen = precision - pos - separators) < 0)
		zpadlen = 0;

#ifdef _DEBUG
	assert(width      <= INT32_MAX                );
	assert(separators <= (UINTMAX_OCT_DIG - 1) / 3);
	assert(precision  >= -1                       );
	assert(precision  <= INT32_MAX                );
	assert(pos        <= UINTMAX_OCT_DIG          );
#endif

	if (_sub_u32(width                        , /* Minimum field width. */
	             separators                   + /* Number of separators. */
	             max(precision, (int32_t)pos) + /* Number of integer digits. */
	             (sign ? 1 : 0)               + /* Will we print a sign? */
	             (hexprefix ? 2 : 0)          , /* Will we print a prefix? */
	             &spadlen))
		spadlen = 0;

	/*
	 * C99 says: "If the `0' and `-' flags both appear, the `0' flag is
	 * ignored.  For `d', `i', `o', `u', `x', and `X' conversions, if a
	 * precision is specified, the `0' flag is ignored." (7.19.6.1, 6)
	 */
	if (spadlen && !(flags & FL_LEFT))
	{
		if ((flags & FL_LEADZERO) && noprecision)
		{
			zpadlen += spadlen;
			spadlen = 0;
		}

		/* Leading spaces. */
		else
			do
				OUTCHAR_OR_BREAK(' ', ADD_LENGTH(spadlen); spadlen = 0);
			while (--spadlen);
	}

	/* Sign. */
	if (sign)
		OUTCHAR(sign);

	/* A "0x" or "0X" prefix. */
	if (hexprefix)
	{
		OUTCHAR('0');
		OUTCHAR(hexprefix);
	}

	/* Leading zeros. */
	if (zpadlen)
		do
			OUTCHAR_OR_BREAK('0', ADD_LENGTH(zpadlen));
		while (--zpadlen);

	/* The actual digits. */
	if (pos)
	{
		if (!separators)
		{
			i = -(int32_t)pos;
			do
				OUTCHAR_OR_BREAK(icvtbuf[pos + i], SUB_LENGTH(i));
			while (++i);
		}
		else
		{
			OUTCHAR(icvtbuf[0]);
			i = pos;
			while (--i)
			{
				switch (0)
				{
				default:
					if ((uint32_t)i % 3 == 0)
						OUTCHAR_OR_BREAK(',', INC_LENGTH());
					OUTCHAR_OR_BREAK(icvtbuf[pos - i], );
					continue;
				}
				ADD_LENGTH(i);
				ADD_LENGTH(GETNUMSEP(i));
				break;
			}
		}
	}

	/* Trailing spaces. */
	if (spadlen)
		do
			OUTCHAR_OR_BREAK(' ', ADD_LENGTH(spadlen));
		while (--spadlen);

	return length;
}

#define ECVTBUF(value, ndigits, decpt, cvtbuf) \
	fltcvt(value, ndigits, decpt, cvtbuf, true)

#define FCVTBUF(value, ndigits, decpt, cvtbuf) \
	fltcvt(value, ndigits, decpt, cvtbuf, false)

static uint32_t fltcvt(long_double value, uint32_t ndigits, int32_t *decpt, TCHAR cvtbuf[CVTBUFSIZE], bool eflag)
{
#if !LONGDOUBLE_IS_DOUBLE && (!LONGDOUBLE_IS_X86_EXTENDED || INTMAX_IS_LLONG)
	long_double intpart, fracpart;
	TCHAR       *p1, *p2;
	int32_t     r2;

#ifdef _DEBUG
	assert(!signbitl(value));
	assert(!isnanl(value));
	assert(!isinfl(value));
	assert((int32_t)ndigits >= 0);
#endif

	r2 = 0;
	p1 = cvtbuf;
	value = modfl(value, &intpart);
	if (intpart)
	{
		TCHAR c1, c2;

		do
		{
			fracpart = modfl(intpart / 10, &intpart);
			*(p1++) = (TCHAR)((fracpart + .03) * 10) + '0';
		} while (intpart && p1 < cvtbuf + CVTBUFSIZE - 1);
		r2 = (p1--) - cvtbuf;
		p2 = cvtbuf;
		while (p1 > p2)
		{
			c1 = *p1;
			c2 = *p2;
			*(p1--) = c2;
			*(p2++) = c1;
		}
		p1 = cvtbuf + r2;
	}
	else if (value)
	{
		while ((fracpart = value * 10) < 1)
		{
			value = fracpart;
			r2--;
		}
	}
	p2 = cvtbuf + ndigits;
	if (!eflag)
		p2 += r2;
	if (p2 >= cvtbuf)
	{
		TCHAR *p3;

		p3 = min(p2, cvtbuf + CVTBUFSIZE - 1);
		while (p1 <= p3)
		{
			value *= 10;
			value = modfl(value, &fracpart);
			*(p1++) = (TCHAR)fracpart + '0';
		}
		if (p2 < cvtbuf + CVTBUFSIZE)
		{
			p1 = p2;
			*p2 += 5;
			while (*p2 > '9')
			{
				if (p2 > cvtbuf)
				{
					*p2 = '0';
					++(*(--p2));
				}
				else
				{
					*p2 = '1';
					r2++;
					if (!eflag)
					{
						if (p1 > cvtbuf)
							*p1 = '0';
						p1++;
					}
				}
			}
		}
		else
		{
			p1 = cvtbuf + CVTBUFSIZE - 1;
		}
	}
	else
	{
		r2 = 0;
		p1 = cvtbuf;
	}
	*p1 = '\0';
	*decpt = r2;
	return p1 - cvtbuf;
#else
	uint32_t length;
	int32_t  padding;

#ifdef _DEBUG
	assert(!signbitl(value));
	assert(!isnanl(value));
	assert(!isinfl(value));
	assert((int32_t)ndigits >= 0);
#endif

	if (value)
	{
		int32_t     e, i;
		uint32_t    u;
		long_double x;
		uintmax_t   decimal;

		e = (int32_t)floorl(log10l(value));
		i = (LDBL_DECIMAL_DIG - 1) - e;
#if LONGDOUBLE_IS_DOUBLE && defined(_MSC_VER) && defined(_M_IX86)
		x = ldexp10(value, i);
#elif !LONGDOUBLE_IS_DOUBLE
		x = value;
		if (i > LDBL_MAX_10_EXP)
		{
			x *= CONCAT(1e, LDBL_MAX_10_EXP);
			i -= LDBL_MAX_10_EXP;
		}
		x *= exp10l(i);
#else
		x = value;
		if (i)
		{
			if (i >= 0)
			{
				u = i;
				if (u & 1) x *= 1e+001; if (u >>= 1) {
				if (u & 1) x *= 1e+002; if (u >>= 1) {
				if (u & 1) x *= 1e+004; if (u >>= 1) {
				if (u & 1) x *= 1e+008; if (u >>= 1) {
				if (u & 1) x *= 1e+016; if (u >>= 1) {
				if (u & 1) x *= 1e+032; if (u >>= 1) {
				if (u & 1) x *= 1e+064; if (u >>= 1) {
				if (u & 1) x *= 1e+128; if (u >>= 1) {
				if (u & 1) x *= 1e+256; } } } } } } } }
			}
			else
			{
				u = -i;
				if (u & 1) x *= 1e-001; if (u >>= 1) {
				if (u & 1) x *= 1e-002; if (u >>= 1) {
				if (u & 1) x *= 1e-004; if (u >>= 1) {
				if (u & 1) x *= 1e-008; if (u >>= 1) {
				if (u & 1) x *= 1e-016; if (u >>= 1) {
				if (u & 1) x *= 1e-032; if (u >>= 1) {
				if (u & 1) x *= 1e-064; if (u >>= 1) {
				if (u & 1) x *= 1e-128; if (u >>= 1) {
				if (u & 1) x *= 1e-256; } } } } } } } }
			}
		}
#endif
		decimal = LDBL_GET_MANT(x);
		if ((i = (int32_t)LDBL_GET_EXP(x) - (LDBL_EXP_BIAS + LDBL_MANT_DIG - 1)) < 0)
			decimal >>= -i;
		else
			decimal <<= i;
		if (decimal)
		{
#if !LONGDOUBLE_IS_DOUBLE || defined(_MSC_VER) && defined(_M_IX86)
			if (decimal >= (uintmax_t)CONCAT(1e, LDBL_DECIMAL_DIG))
			{
				e++;
				decimal = (uintmax_t)CONCAT(1e, LDBL_DECIMAL_DIG) / 10;
			}
			else if (decimal < (uintmax_t)CONCAT(1e, LDBL_DECIMAL_DIG) / 10)
			{
				decimal = (uintmax_t)CONCAT(1e, LDBL_DECIMAL_DIG) / 10;
			}
#else
			if (e >= 0 ? decimal >= (uintmax_t)CONCAT(1e, LDBL_DECIMAL_DIG) : decimal >= (uintmax_t)CONCAT(1e, LDBL_DECIMAL_DIG) - 16)
			{
				e++;
				decimal = (uintmax_t)CONCAT(1e, LDBL_DECIMAL_DIG) / 10;
			}
			else if (decimal <= (uintmax_t)CONCAT(1e, LDBL_DECIMAL_DIG) / 10 + 2)
			{
				decimal = (uintmax_t)CONCAT(1e, LDBL_DECIMAL_DIG) / 10;
			}
#endif
			if ((i = (LDBL_DECIMAL_DIG - 1) - ndigits - (!eflag ? e + 1 : 0)) >= 0 && (eflag || modfl(value, &x)))
			{
				if (i <= LDBL_DECIMAL_DIG - 1)
				{
					if (i != LDBL_DECIMAL_DIG - 1)
					{
						static const uintmax_t power[LDBL_DECIMAL_DIG] = {
							1,
							10,
							100,
							1000,
							10000,
							100000,
							1000000,
							10000000,
							100000000,
							1000000000,
							10000000000,
							100000000000,
							1000000000000,
							10000000000000,
							100000000000000,
							1000000000000000,
							10000000000000000,
#if LDBL_DECIMAL_DIG > 17
							100000000000000000,
							1000000000000000000,
							10000000000000000000,
							100000000000000000000,
#if LDBL_DECIMAL_DIG > 21
							1000000000000000000000,
							10000000000000000000000,
							100000000000000000000000,
							1000000000000000000000000,
							10000000000000000000000000,
							100000000000000000000000000,
							1000000000000000000000000000,
							10000000000000000000000000000,
							100000000000000000000000000000,
							1000000000000000000000000000000,
							10000000000000000000000000000000,
							100000000000000000000000000000000,
							1000000000000000000000000000000000,
							10000000000000000000000000000000000,
							100000000000000000000000000000000000,
#endif
#endif
						};

						if (i)
							decimal /= power[i];
						u = (uint32_t)(decimal % 10);
						decimal /= 10;
						if (u >= 5 && ++decimal == power[_countof(power) - 1 - i])
						{
							e++;
							if (eflag)
								decimal = power[_countof(power) - 2 - i];
						}
					}
					else
					{
						if (decimal = decimal >= (uintmax_t)CONCAT(5e, LDBL_DECIMAL_DIG) / 10)
							e++;
					}
				}
				else
				{
					decimal = 0;
				}
			}
		}
#if !defined(_MSC_VER) || !LONGDOUBLE_IS_DOUBLE
		length = intcvt(decimal, cvtbuf, 10, 0);
#else
		length = (uint32_t)_ui64to10t(decimal, cvtbuf);
#endif
		*decpt = e + 1;
	}
	else
	{
		cvtbuf[0] = '0';
		cvtbuf[1] = '\0';
		length = 1;
		*decpt = 0;
	}
	if (padding = (!eflag ? *decpt : 0) + ndigits - length)
	{
		if (padding > 0)
		{
			if ((uint32_t)padding > CVTBUFSIZE - 1 - length)
				padding = CVTBUFSIZE - 1 - length;
			memset(cvtbuf + length, '0', padding);
		}
		if ((int32_t)(length += padding) < 0)
		{
			*decpt -= (int32_t)length;
			length = 0;
		}
		cvtbuf[length] = '\0';
	}
	return length;
#endif
}

static inline uint32_t hexcvt(long_double value, uint32_t precision, TCHAR cvtbuf[CVTBUFSIZE], uint32_t *elen, TCHAR expbuf[EXPBUFSIZE], int flags)
{
	uintmax_t           mantissa;
	int32_t             exponent;
	int32_t             i;
	const unsigned char *digits;
	TCHAR               *p1, *p2;
#ifndef _MSC_VER
	TCHAR               c1, c2;
#endif

#ifdef _DEBUG
	assert(!signbitl(value));
	assert(!isnanl(value));
	assert(!isinfl(value));
	assert((int32_t)precision >= 0);
#endif

	mantissa = LDBL_GET_MANT(value);
	exponent = (int32_t)LDBL_GET_EXP(value) - LDBL_EXP_BIAS;
	if (precision > MANTISSA_HEX_DIG)
		precision = MANTISSA_HEX_DIG;
	if (i = MANTISSA_HEX_DIG - precision)
	{
		mantissa >>= (uint32_t)i * 4 - 4;
		mantissa += 7;
		mantissa >>= 4;
	}
	digits = (flags & FL_UP) ? digitsLarge : digitsSmall;
	if (precision)
	{
		p1 = cvtbuf + 1;
		p2 = p1 + precision;
		do
		{
			*(--p2) = digits[(uint32_t)mantissa & 0x0F];
			mantissa >>= 4;
		} while (p2 != p1);
	}
	*cvtbuf = digits[(uint32_t)mantissa & 0x0F];
	expbuf[0] = (flags & FL_UP) ? 'P' : 'p';
	if (exponent >= 0)
	{
		expbuf[1] = '+';
	}
	else
	{
		exponent = -exponent;
		expbuf[1] = '-';
	}
#ifdef _MSC_VER
	*elen = (uint32_t)_ui32to10t(exponent, expbuf + 2) + 2;
#else
	p2 = p1 = expbuf + 2;
	do
		*(p2++) = (TCHAR)((uint32_t)exponent % 10) + '0';
	while (exponent = (uint32_t)exponent / 10);
	*elen = (p2--) - expbuf;
	while (p1 < p2)
	{
		c1 = *p1;
		c2 = *p2;
		*(p1++) = c2;
		*(p2--) = c1;
	}
#endif
	return precision + 1;
}

static uint32_t fltfmt(TCHAR *buffer, uint32_t count, uint32_t length, long_double value, uint32_t width, int32_t precision, int flags)
{
	TCHAR       cvtbuf[CVTBUFSIZE];
	TCHAR       expbuf[EXPBUFSIZE];	/* "e-12" */
	TCHAR       sign;
	TCHAR       hexprefix;
	uint32_t    cvtlen;
	int32_t     decpt;
	uint32_t    ilen;
	uint32_t    flen;
	uint32_t    elen;
	uint32_t    trailfraczeros;
	uint32_t    separators;
	uint32_t    emitpoint;
	uint32_t    padlen;
	int32_t     i;
	TCHAR       *p;
	const TCHAR *infnan;
	TCHAR       c;

#ifdef _DEBUG
	assert(width     <= INT32_MAX);
	assert(precision >= -1       );
	assert(precision <= INT32_MAX);
#endif

	// Left align means no zero padding
	if (flags & FL_LEFT)
		flags &= ~FL_LEADZERO;

	// Determine padding and sign char
	sign =
		!signbitl(value) ?
			!(flags & (FL_SIGN | FL_SIGNSP)) ?
				'\0' :
				(flags & FL_SIGN) ?
					'+' :
					' ' :
		'-';

	if (!isfinitel(value))
		if (LDBL_MANT_WORD(value) & LDBL_MANT_MASK)
			goto NaN;
		else
			goto INF;

	if (signbitl(value))
		value = -value;

	// Compute the precision value
	if (precision < 0)
		// Default precision: 6
		precision = !(flags & FL_TYPE_A) ? 6 : MANTISSA_HEX_DIG;
	else if (!precision && (flags & FL_TYPE_G))
		// ANSI specified
		precision = 1;

#ifdef _DEBUG
	assert(width     <= INT32_MAX);
	assert(precision >= 0        );
	assert(precision <= INT32_MAX);
#endif

	// Convert floating point number to text
	if (!(flags & FL_TYPE_A))
	{
		if (flags & FL_TYPE_G)
		{
			ECVTBUF(value, precision, &decpt, cvtbuf);
			if (decpt <= -4 || decpt > precision)
			{
				flags |= FL_TYPE_E;
				precision -= 1;
			}
			else
			{
				precision -= decpt;
			}
		}
		if (flags & FL_TYPE_E)
		{
			int32_t exponent;
#ifndef _MSC_VER
			TCHAR   *p1, *p2;
			TCHAR   c1, c2;
#endif

			cvtlen = ECVTBUF(value, precision + 1, &decpt, cvtbuf);

			exponent = !decpt ? !value ? 0 : -1 : decpt - 1;
			decpt = 1;

			expbuf[0] = (flags & FL_UP) ? 'E' : 'e';
			if (exponent >= 0)
			{
				expbuf[1] = '+';
			}
			else
			{
				exponent = -exponent;
				expbuf[1] = '-';
			}
#ifdef _MSC_VER
			elen = (uint32_t)_ui32to10t(exponent, expbuf + 2) + 2;
			if (elen == 3)
			{
				expbuf[3] = expbuf[2];
				expbuf[2] = '0';
				elen++;
			}
#else
			elen = 2;
			do
				expbuf[elen++] = (TCHAR)((uint32_t)exponent % 10) + '0';
			while (exponent = (uint32_t)exponent / 10);

			/*
			 * C99 says: "The exponent always contains at least two digits,
			 * and only as many more digits as necessary to represent the
			 * exponent." (7.19.6.1, 8)
			 */
			if (elen == 3)
				expbuf[elen++] = '0';

			p1 = expbuf + 2;
			p2 = expbuf + elen - 1;
			while (p1 < p2)
			{
				c1 = *p1;
				c2 = *p2;
				*(p1++) = c2;
				*(p2--) = c1;
			}
#endif
		}
		else
		{
			elen = 0;
			cvtlen = FCVTBUF(value, precision, &decpt, cvtbuf);
		}
		if ((flags & FL_TYPE_G) && !(flags & FL_ALTERNATE))
		{
			TCHAR *end;

			p = end = cvtbuf + cvtlen;
			while (*(--p) == '0' && p != cvtbuf);
			if (++p != end)
			{
				int32_t diff;

				diff = end - p;
				cvtlen -= diff;
				if ((precision -= diff) < 0)
					precision = 0;
			}
		}
		ilen = max(decpt, 1);
		flen = cvtlen - decpt;
		hexprefix = '\0';
	}
	else
	{
		cvtlen = hexcvt(value, precision, cvtbuf, &elen, expbuf, flags);
		ilen = decpt = 1;
		flen = cvtlen - 1;
		hexprefix = (flags & FL_UP) ? 'X' : 'x';
	}
	if ((int32_t)(trailfraczeros = precision - flen) < 0)
		trailfraczeros = 0;

	/*
	 * Print a decimal point if either the fractional part is non-zero
	 * and/or the "#" flag was specified.
	 */
	emitpoint = precision || (flags & FL_ALTERNATE);

	/* Get the number of group separators we'll print. */
	separators = ((flags & FL_QUOTE) && !(flags & FL_TYPE_A) && ilen) ? GETNUMSEP(ilen) : 0;

#ifdef _DEBUG
	assert(width      <= INT32_MAX                            );
	assert(decpt      >= DEC_DIG(LDBL_MIN_EXP - LDBL_MANT_DIG));
	assert(decpt      <= LDBL_MAX_10_EXP + 1                  );
	assert(ilen       >= 1                                    );
	assert(ilen       <= LDBL_MAX_10_EXP + 1                  );
	assert(elen       <= EXPBUFSIZE - 1                       );
	assert(precision  >= 0                                    );
	assert(precision  <= INT32_MAX                            );
	assert(separators <= LDBL_MAX_10_EXP / 3                  );
	assert(emitpoint  <= 1                                    );
#endif

	if (_sub_u32(width              ,   /* Minimum field width. */
	             ilen               +   /* Number of integer digits. */
	             elen               +   /* Number of exponent characters. */
	             precision          +   /* Number of fractional digits. */
	             separators         +   /* Number of group separators. */
	             emitpoint          +   /* Will we print a decimal point? */
	             (sign ? 1 : 0)     +   /* Will we print a sign character? */
	             (hexprefix ? 2 : 0),   /* Will we print a prefix? */
	             &padlen))
		padlen = 0;

	do  /* do { ... } while (0); */
	{
		/*
		 * C99 says: "If the `0' and `-' flags both appear, the `0' flag is
		 * ignored." (7.19.6.1, 6)
		 */
		if (padlen && !(flags & FL_LEFT))
		{
			if (flags & FL_LEADZERO)
			{
				/* Sign. */
				if (sign)
					OUTCHAR(sign);

				/* A "0x" or "0X" prefix. */
				if (hexprefix)
				{
					OUTCHAR('0');
					OUTCHAR(hexprefix);
				}

				/* Leading zeros. */
				do
					OUTCHAR_OR_BREAK('0', ADD_LENGTH(padlen); padlen = 0);
				while (--padlen);

				break;
			}
			else
			{
				/* Leading spaces. */
				do
					OUTCHAR_OR_BREAK(' ', ADD_LENGTH(padlen); padlen = 0);
				while (--padlen);
			}
		}

		/* Sign. */
		if (sign)
			OUTCHAR(sign);

		/* A "0x" or "0X" prefix. */
		if (hexprefix)
		{
			OUTCHAR('0');
			OUTCHAR(hexprefix);
		}

	} while (0);

	/* Integer part. */
	if (decpt > 0)
	{
		if (!separators)
		{
			i = -decpt;
			do
				OUTCHAR_OR_BREAK(cvtbuf[decpt + i], SUB_LENGTH(i));
			while (++i);
		}
		else
		{
			OUTCHAR(cvtbuf[0]);
			i = decpt;
			while (--i)
			{
				switch (0)
				{
				default:
					if ((uint32_t)i % 3 == 0)
						OUTCHAR_OR_BREAK(',', INC_LENGTH());
					OUTCHAR_OR_BREAK(cvtbuf[decpt - i], );
					continue;
				}
				ADD_LENGTH(i);
				ADD_LENGTH(GETNUMSEP(i));
				break;
			}
		}
	}
	else
	{
		OUTCHAR('0');
	}

	/* Decimal point. */
	if (emitpoint)
		OUTCHAR('.');

	/* The remaining fractional part. */
	if (decpt < 0)
		do
			OUTCHAR_OR_BREAK('0', SUB_LENGTH(decpt); decpt = 0);
		while (++decpt);
	if (i = decpt - cvtlen)
		do
			OUTCHAR_OR_BREAK(cvtbuf[cvtlen + i], SUB_LENGTH(i));
		while (++i);

	/* Following fractional part zeros. */
	if (trailfraczeros)
		do
			OUTCHAR_OR_BREAK('0', ADD_LENGTH(trailfraczeros));
		while (--trailfraczeros);

	/* Exponent. */
	if (i = -(int32_t)elen)
		do
			OUTCHAR_OR_BREAK(expbuf[elen + i], SUB_LENGTH(i));
		while (++i);

	/* Trailing spaces. */
	if (padlen)
		do
			OUTCHAR_OR_BREAK(' ', ADD_LENGTH(padlen));
		while (--padlen);

	return length;

NaN:
#if !defined(_WIN32) || (!LONGDOUBLE_IS_DOUBLE && !LONGDOUBLE_IS_X86_EXTENDED && !LONGDOUBLE_IS_QUAD)
	infnan = lpcszNan;
#elif LONGDOUBLE_IS_DOUBLE
	if (!(*(uint64_t *)&value & 0x0008000000000000))
		infnan = lpcszNanSnan;
	else if (*(uint64_t *)&value != 0xFFF8000000000000)
		infnan = lpcszNan;
	else
		infnan = lpcszNanInd;
#elif LONGDOUBLE_IS_X86_EXTENDED
	if (!(LDBL_MANT_WORD(value) & 0x4000000000000000))
		infnan = lpcszNanSnan;
	else if (LDBL_MANT_WORD(value) != 0xC000000000000000 || LDBL_SIGN_WORD(value) != 0xFFFF)
		infnan = lpcszNan;
	else
		infnan = lpcszNanInd;
#elif LONGDOUBLE_IS_QUAD
	if (!(*(uint128_t *)&value & 0x00008000000000000000000000000000))
		infnan = lpcszNanSnan;
	else if (*(uint128_t *)&value != 0xFFFF8000000000000000000000000000)
		infnan = lpcszNan;
	else
		infnan = lpcszNanInd;
#endif
	goto INF_NaN;

INF:
	infnan = lpcszInf;

INF_NaN:
	p = cvtbuf;
	if (sign)
		*(p++) = sign;
	if (flags & FL_UP)
		while (c = *(infnan++))
			*(p++) = (c >= 'a' && c <= 'z') ? c - ('a' - 'A') : c;
	else
		while (c = *(infnan++))
			*(p++) = c;
	return tcsfmt(buffer, count, length, cvtbuf, width, p - cvtbuf, flags);
}
