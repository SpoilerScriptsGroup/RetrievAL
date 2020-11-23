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

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Per-module <stdio.h> configuration.
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#define _CRT_INTERNAL_PRINTF_LEGACY_VSPRINTF_NULL_TERMINATION   (UINT64_C(1) << 0)
#define _CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR         (UINT64_C(1) << 1)
#define _CRT_INTERNAL_PRINTF_LEGACY_WIDE_SPECIFIERS             (UINT64_C(1) << 2)
#define _CRT_INTERNAL_PRINTF_LEGACY_MSVCRT_COMPATIBILITY        (UINT64_C(1) << 3)
#define _CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS       (UINT64_C(1) << 4)

#endif	// _CORECRT_STDIO_CONFIG_H_
