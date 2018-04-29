#include <string.h>
#include <tchar.h>

#ifndef _M_IX86
TCHAR * __cdecl _tcsncpy(TCHAR *string1, const TCHAR *string2, size_t count)
{
	size_t size;

	size = (_tcsnlen(string2, count) + 1) * sizeof(TCHAR);
	count *= sizeof(TCHAR);
	if (count > size)
		memset((char *)string2 + size, '\0', count - size);
	else
		size = count;
	return (TCHAR *)memcpy(string1, string2, size);
}
#else
#pragma function(memset, memcpy)
__declspec(naked) TCHAR * __cdecl _tcsncpy(TCHAR *string1, const TCHAR *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		mov     ecx, dword ptr [count]
		mov     eax, dword ptr [string2]
		push    ecx
		push    eax
		call    _tcsnlen
		mov     ecx, dword ptr [count + 8]
#ifdef _UNICODE
		lea     eax, [eax * 2 + 2]
		add     ecx, ecx
#else
		inc     eax
#endif
		sub     ecx, eax
		jbe     L1
		mov     edx, dword ptr [string1 + 8]
		mov     dword ptr [count + 8], eax
		mov     dword ptr [esp + 4], ecx
		add     eax, edx
		mov     dword ptr [esp], '\0'
		push    eax
		call    memset
		add     esp, 12
		jmp     memcpy

		align   16
	L1:
		add     esp, 8
		add     eax, ecx
		mov     dword ptr [count], eax
		jmp     memcpy

		#undef string1
		#undef string2
		#undef count
	}
}
#endif
