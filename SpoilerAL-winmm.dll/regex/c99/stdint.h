#ifndef _STDINT_H_
#define _STDINT_H_

#include <stddef.h>

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

#ifndef SIZE_MAX
#ifdef _WIN64
#define SIZE_MAX _UI64_MAX
#else
#define SIZE_MAX UINT_MAX
#endif
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
#define INTPTR_MIN  (-INTPTR_MAX - 1)
#define INTPTR_MAX  (UINTPTR_MAX >> 1)
#define UINT8_MAX   _UI8_MAX
#define UINT16_MAX  _UI16_MAX
#define UINT32_MAX  _UI32_MAX
#define UINT64_MAX  _UI64_MAX
#define UINTMAX_MAX _UI64_MAX
#define UINTPTR_MAX SIZE_MAX

#define INT8_C(x)    (x)
#define INT16_C(x)   (x)
#define INT32_C(x)   (x)
#define INT64_C(x)   (x ## I64)
#define UINT8_C(x)   (x)
#define UINT16_C(x)  (x)
#define UINT32_C(x)  (x ## U)
#define UINT64_C(x)  (x ## UI64)
#define INTMAX_C(x)  INT64_C(x)
#define UINTMAX_C(x) UINT64_C(x)

#endif
