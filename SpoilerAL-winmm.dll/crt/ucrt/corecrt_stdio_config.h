//
// corecrt_stdio_config.h
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// Per-module <stdio.h> configuration.
//
#ifndef _CORECRT_STDIO_CONFIG_H_
#define _CORECRT_STDIO_CONFIG_H_

#if _MSC_VER > 1000
#pragma once
#endif

#if !defined(_MSC_VER) || _MSC_VER >= 1600
#include <stdint.h>
#else
typedef unsigned __int64 uint64_t;
#endif

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Per-module <stdio.h> configuration.
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#define _CRT_INTERNAL_PRINTF_LEGACY_VSPRINTF_NULL_TERMINATION   ((uint64_t)1 << 0)
#define _CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR         ((uint64_t)1 << 1)
#define _CRT_INTERNAL_PRINTF_LEGACY_WIDE_SPECIFIERS             ((uint64_t)1 << 2)
#define _CRT_INTERNAL_PRINTF_LEGACY_MSVCRT_COMPATIBILITY        ((uint64_t)1 << 3)
#define _CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS       ((uint64_t)1 << 4)

#endif	// _CORECRT_STDIO_CONFIG_H_
