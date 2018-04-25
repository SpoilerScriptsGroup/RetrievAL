#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include "bcb6_errno.h"

__declspec(naked) int __cdecl bcb6_snprintf(char *buffer, size_t count, const char *format, ...)
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
		call    _vsnprintf
		mov     ecx, dword ptr [count + 16]
		add     esp, 16
		cmp     eax, ecx
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
