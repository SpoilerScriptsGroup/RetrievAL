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

		mov     ecx, dword ptr [count]                  // ecx = count
		mov     eax, dword ptr [src]                    // eax = src
		test    ecx, ecx
		jz      L1                                      // jump if count == 0
		push    ecx
		push    eax
		call    _tcsnlen
		mov     ecx, dword ptr [count + 8]              // ecx = count
		mov     edx, dword ptr [dest + 8]               // edx = dest
		sub     ecx, eax                                // ecx = count - length
		sub     esp, 4
#ifdef _UNICODE
		add     ecx, ecx
		add     eax, eax
#endif
		mov     dword ptr [esp + 8], ecx                // store memset 3rd param
		push    eax                                     // store memcpy 3rd param
		add     eax, edx                                // eax = dest + length
		mov     ecx, dword ptr [src + 16]               // ecx = src
		mov     dword ptr [esp + 8], 0                  // store memset 2nd param
		mov     dword ptr [esp + 4], eax                // store memset 1st param
		push    ecx                                     // store memcpy 2nd param
		push    edx                                     // store memcpy 1st param
		call    memcpy
		add     esp, 12
		call    memset
		mov     eax, dword ptr [dest + 12]              // return dest
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
