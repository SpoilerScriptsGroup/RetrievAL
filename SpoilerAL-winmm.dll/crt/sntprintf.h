#define _CRT_SECURE_NO_WARNINGS
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include <stdarg.h>
#include <tchar.h>
#ifdef _UNICODE
#define internal_vsntprintf internal_vsnwprintf
#else
#define internal_vsntprintf internal_vsnprintf
#endif

#pragma warning(disable:4028)

#ifndef _M_IX86
int __cdecl _sntprintf(TCHAR *buffer, size_t count, const TCHAR *format, ...)
{
	int __fastcall internal_vsntprintf(TCHAR *buffer, size_t count, const TCHAR *format, va_list argptr, const va_list endarg);

	int     result;
	va_list argptr;

	va_start(argptr, format);
	result = internal_vsntprintf(buffer, count, format, argptr, NULL);
	va_end(argptr);
	return result;
}
#else
__declspec(naked) int __cdecl _sntprintf(TCHAR *buffer, size_t count, const TCHAR *format, ...)
{
	int __fastcall internal_vsntprintf(TCHAR *buffer, size_t count, const TCHAR *format, va_list argptr, const va_list endarg);

	__asm
	{
		#define buffer  (esp + 4)
		#define count   (esp + 8)
		#define format  (esp + 12)
		#define va_args (esp + 16)

		mov     ecx, dword ptr [esp]
		lea     eax, [va_args]
		mov     dword ptr [esp], 0
		push    eax
		mov     edx, dword ptr [count + 4]
		mov     eax, dword ptr [format + 4]
		push    eax
		push    ecx
		mov     ecx, dword ptr [buffer + 12]
		jmp     internal_vsntprintf

		#undef buffer
		#undef count
		#undef format
		#undef va_args
	}
}
#endif
