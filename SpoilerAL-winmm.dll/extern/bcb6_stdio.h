#pragma once

#include <windows.h>

EXTERN_C int __cdecl bcb6_snprintf(char *buffer, size_t count, const char *format, ...);
EXTERN_C int __cdecl bcb6_vsnprintf(char *buffer, size_t count, const char *format, va_list argptr);

