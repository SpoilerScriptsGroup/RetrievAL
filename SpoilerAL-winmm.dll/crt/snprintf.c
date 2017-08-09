#define _CRT_SECURE_NO_WARNINGS
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include <stdarg.h>

#pragma warning(disable:4028)

#ifndef _M_IX86
int __cdecl _snprintf(char *buffer, size_t count, const char *format, ...)
{
	int     result;
	va_list argptr;

	va_start(argptr, format);
	result = _vsnprintf(buffer, count, format, argptr);
	va_end(argptr);
	return result;
}
#else
__declspec(naked) int __cdecl _snprintf(char *buffer, size_t count, const char *format, ...)
{
	__asm
	{
		mov     ecx, dword ptr [esp + 12]
		lea     eax, [esp + 16]
		push    eax
		push    ecx
		mov     eax, dword ptr [esp + 8 + 8]
		mov     ecx, dword ptr [esp + 8 + 4]
		push    eax
		push    ecx
		call    _vsnprintf
		add     esp, 16
		ret
	}
}
#endif
