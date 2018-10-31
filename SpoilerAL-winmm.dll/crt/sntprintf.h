#define _CRT_SECURE_NO_WARNINGS
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include <stdarg.h>
#include <tchar.h>

#pragma warning(disable:4028)

#ifndef _M_IX86
int __cdecl _sntprintf(TCHAR *buffer, size_t count, const TCHAR *format, ...)
{
	int     result;
	va_list argptr;

	va_start(argptr, format);
	result = _vsntprintf(buffer, count, format, argptr);
	va_end(argptr);
	return result;
}
#else
__declspec(naked) int __cdecl _sntprintf(TCHAR *buffer, size_t count, const TCHAR *format, ...)
{
	__asm
	{
		#define buffer  (esp + 4)
		#define count   (esp + 8)
		#define format  (esp + 12)
		#define va_args (esp + 16)

		mov     ecx, dword ptr [format]
		lea     eax, [va_args]
		push    eax
		push    ecx
		mov     eax, dword ptr [count + 8]
		mov     ecx, dword ptr [buffer + 8]
		push    eax
		push    ecx
		call    _vsntprintf
		add     esp, 16
		ret

		#undef buffer
		#undef count
		#undef format
		#undef va_args
	}
}
#endif
