#ifndef _VERBOSE_H_
#define _VERBOSE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#define ENABLE_VERBOSE 0
#define VERBOSE_LOG    "winmm.log"

typedef enum {
	VERBOSE_DEBUG,
	VERBOSE_INFO,
	VERBOSE_WARN,
	VERBOSE_ERROR,
} VERBOSE_LEVEL;

#if ENABLE_VERBOSE

#include <windows.h>

EXTERN_C void init_verbose(HMODULE hModule);
EXTERN_C void __cdecl verbose_output(VERBOSE_LEVEL level, const char *format, ...);
#define VERBOSE_STRINGIFY(n) #n
#define VERBOSE_TOSTRING(n) VERBOSE_STRINGIFY(n)
#define verbose(level, format, ...) \
	if ((level) >= VERBOSE_DEBUG) \
		output_verbose(level, __FILE__ ":" VERBOSE_TOSTRING(__LINE__) ": " format, __VA_ARGS__)

#else
#define init_verbose(hModule)
#define verbose(level, format, ...)
#endif

#endif	// _VERBOSE_H_
