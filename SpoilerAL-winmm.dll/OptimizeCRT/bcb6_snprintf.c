#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include "bcb6_errno.h"

__declspec(naked) int __cdecl bcb6_snprintf(char *buffer, size_t count, const char *format, ...)
{
#ifndef _DEBUG
	int __fastcall __vsnprintf(char *buffer, size_t count, const char *format, va_list argptr, const va_list endarg);
#endif

	__asm
	{
		#define buffer  (esp + 4)
		#define count   (esp + 8)
		#define format  (esp + 12)
		#define va_args (esp + 16)

		mov     eax, dword ptr [format]
		lea     ecx, [va_args]
#ifndef _DEBUG
		push    0
		push    ecx
		mov     ecx, dword ptr [buffer + 8]
		mov     edx, dword ptr [count + 8]
		push    eax
		call    __vsnprintf
		cmp     eax, dword ptr [count]
#else
		push    ecx
		push    eax
		mov     ecx, dword ptr [count + 8]
		mov     eax, dword ptr [buffer + 8]
		push    ecx
		push    eax
		call    _vsnprintf
		mov     ecx, dword ptr [count + 16]
		add     esp, 16
		cmp     eax, ecx
#endif
		jb      L1
		call    dword ptr [_bcb6_errno]
		mov     dword ptr [eax], BCB6_ERANGE
		mov     eax, -1
	L1:
		ret

		#undef buffer
		#undef count
		#undef format
		#undef va_args
	}
}
