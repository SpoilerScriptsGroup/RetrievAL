#include <tchar.h>

#ifndef _M_IX86
TCHAR * __cdecl _tcsncat(TCHAR *string1, const TCHAR *string2, size_t count)
{
	if (count = _tcsnlen(string2, count))
		*((TCHAR *)memcpy(string1 + _tcslen(string1), string2, count * sizeof(TCHAR)) + count) = '\0';
	return string1;
}
#else
#pragma function(_tcslen, memcpy)
__declspec(naked) TCHAR * __cdecl _tcsncat(TCHAR *string1, const TCHAR *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		mov     ecx, dword ptr [count]                  // ecx = count
		mov     eax, dword ptr [string2]                // eax = src
		push    ecx
		push    eax
		call    _tcsnlen
		mov     ecx, dword ptr [string1 + 8]            // ecx = dest
#ifdef _UNICODE
		add     eax, eax
#else
		test    eax, eax
#endif
		mov     dword ptr [esp + 4], ebx                // preserve ebx
		jz      L1                                      // jump if _tcsnlen(string2, count) * sizeof(TCHAR) == 0
		mov     dword ptr [esp], ecx
		mov     ebx, eax                                // ebx = _tcsnlen(string2, count) * sizeof(TCHAR)
		call    _tcslen
		mov     ecx, dword ptr [string1 + 8]            // ecx = dest
		mov     edx, dword ptr [string2 + 8]            // edx = src
#ifdef _UNICODE
		mov     dword ptr [esp], ebx
		lea     eax, [ecx + eax * 2]                    // eax = string1 + _tcslen(string1)
#else
		add     eax, ecx
		mov     dword ptr [esp], ebx
#endif
		push    edx
		push    eax
		call    memcpy
#ifdef _UNICODE
		mov     word ptr [eax + ebx], '\0'              // null terminator
#else
		mov     byte ptr [eax + ebx], '\0'
#endif
		add     esp, 12
		mov     eax, dword ptr [string1 + 4]            // return dest
		pop     ebx                                     // restore ebx
		ret

		align   16
	L1:
		add     esp, 8
		mov     eax, ecx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}
#endif
