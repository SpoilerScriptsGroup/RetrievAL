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
EXTERN_C void __cdecl verbose_output(const char *buffer);
EXTERN_C void __cdecl verbose_format(const char *format, ...);
#define VERBOSE_STRINGIFY(n) #n
#define VERBOSE_TOSTRING(n) VERBOSE_STRINGIFY(n)
#define verbose(level, format, ...)                                    \
do                                                                     \
    if (level >= VRB_DEBUG)                                            \
    {                                                                  \
        verbose_output(__FILE__ ": " VERBOSE_TOSTRING(__LINE__) ": "); \
        verbose_format(format, __VA_ARGS__);                           \
        verbose_output("\r\n");                                        \
    }                                                                  \
while (0)
#else
#define init_verbose(hModule)
#define verbose(level, format, ...)
#endif

#endif	// _VRB_H_
