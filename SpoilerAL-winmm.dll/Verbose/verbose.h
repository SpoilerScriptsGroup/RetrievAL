#ifndef _VRB_H_
#define _VRB_H_

#if _MSC_VER > 1000
#pragma once
#endif

#define ENABLE_VERBOSE 0
#define VERBOSE_LOG    "winmm.log"

typedef enum {
	VRB_DEBUG,
	VRB_INFO,
	VRB_WARN,
	VRB_ERROR,
} VRB_LEVEL;

#if ENABLE_VERBOSE

#include <windows.h>

EXTERN_C void init_verbose(HMODULE hModule);
EXTERN_C void __cdecl verbose_output(const char *format, ...);
#define verbose(level, format, ...)                                        \
do                                                                         \
    if (level >= VRB_DEBUG)                                                \
        verbose_output("%s:%d: " format, __FILE__, __LINE__, __VA_ARGS__); \
while (0)
#else
#define init_verbose(hModule)
#define verbose(level, format, ...)
#endif

#endif	// _VRB_H_
