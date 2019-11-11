#include <string.h>
#include <tchar.h>

#ifndef _M_IX86
TCHAR * __cdecl _tcsncat(TCHAR *dest, const TCHAR *src, size_t count)
{
	if (count = _tcsnlen(src, count))
		*((TCHAR *)memcpy(dest + _tcslen(dest), src, count * sizeof(TCHAR)) + count) = '\0';
	return dest;
}
#else
#pragma function(_tcslen, memcpy)
__declspec(naked) TCHAR * __cdecl _tcsncat(TCHAR *dest, const TCHAR *src, size_t count)
{
	__asm
	{
		#define dest  (esp + 4)
		#define src   (esp + 8)
		#define count (esp + 12)

		mov     ecx, dword ptr [count]                  // ecx = count
		mov     eax, dword ptr [src]                    // eax = src
		push    ecx
		push    eax
		call    _tcsnlen
		mov     ecx, dword ptr [dest + 8]               // ecx = dest
#ifdef _UNICODE
		add     eax, eax
#else
		test    eax, eax
#endif
		mov     dword ptr [esp + 4], ebx                // preserve ebx
		jz      L1                                      // jump if _tcsnlen(src, count) == 0
		mov     dword ptr [esp], ecx
		mov     ebx, eax                                // ebx = _tcsnlen(src, count)
		call    _tcslen
		mov     ecx, dword ptr [dest + 8]               // ecx = dest
		mov     edx, dword ptr [src + 8]                // edx = src
#ifdef _UNICODE
		lea     eax, [ecx + eax * 2]                    // eax = dest + _tcslen(dest)
#else
		add     eax, ecx
#endif
		mov     dword ptr [esp], ebx
		push    edx
		push    eax
		call    memcpy
#ifdef _UNICODE
		mov     word ptr [eax + ebx], '\0'              // null terminator
#else
		mov     byte ptr [eax + ebx], '\0'
#endif
		add     esp, 12
		mov     eax, dword ptr [dest + 4]               // return dest
		pop     ebx                                     // restore ebx
		ret

		align   16
	L1:
		mov     eax, ecx
		add     esp, 8
		ret

		#undef dest
		#undef src
		#undef count
	}
}
#endif
