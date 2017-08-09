#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include "bcb6_errno.h"

__declspec(naked) int __cdecl bcb6_snprintf(char *buffer, size_t count, const char *format, ...)
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
		mov     ecx, dword ptr [esp + 16 + 8]
		add     esp, 16
		cmp     eax, ecx
		jb      L1
		call    dword ptr [_bcb6_errno]
		mov     dword ptr [eax], BCB6_ERANGE
		mov     eax, -1
	L1:
		ret
	}
}
