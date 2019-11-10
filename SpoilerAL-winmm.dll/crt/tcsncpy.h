#include <string.h>
#include <tchar.h>

#ifndef _M_IX86
TCHAR * __cdecl _tcsncpy(TCHAR *dest, const TCHAR *src, size_t count)
{
	if (count)
	{
		size_t length;

		memcpy(dest, src, (length = _tcsnlen(src, count)) * sizeof(TCHAR));
		memset(dest + length, 0, (count - length) * sizeof(TCHAR));
	}
	return dest;
}
#else
#pragma function(memcpy, memset)
__declspec(naked) TCHAR * __cdecl _tcsncpy(TCHAR *dest, const TCHAR *src, size_t count)
{
	__asm
	{
		#define dest  (esp + 4)
		#define src   (esp + 8)
		#define count (esp + 12)

		mov     eax, dword ptr [count]
		mov     ecx, dword ptr [src]
		test    eax, eax
		jz      L1
		push    eax
		push    ecx
		call    _tcsnlen
		mov     ecx, dword ptr [count + 8]
		mov     edx, dword ptr [dest + 8]
		sub     ecx, eax
		sub     esp, 4
#ifdef _UNICODE
		add     ecx, ecx
		add     eax, eax
#endif
		mov     dword ptr [esp + 8], ecx
		push    eax
		add     eax, edx
		mov     ecx, dword ptr [src + 16]
		mov     dword ptr [esp + 8], 0
		mov     dword ptr [esp + 4], eax
		push    ecx
		push    edx
		call    memcpy
		add     esp, 12
		call    memset
		mov     eax, dword ptr [dest + 12]
		add     esp, 12
		ret

		align   16
	L1:
		mov     eax, dword ptr [dest]
		ret

		#undef dest
		#undef src
		#undef count
	}
}
#endif
