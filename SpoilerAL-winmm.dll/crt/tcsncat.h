#include <string.h>
#include <tchar.h>

#ifndef _M_IX86
TCHAR * __cdecl _tcsncat(TCHAR *dest, const TCHAR *src, size_t count)
{
	if (count = _tcsnlen(src, count))
	{
		TCHAR *p;

		memcpy(p = dest + _tcslen(dest), src, count * sizeof(TCHAR));
		p[count] = '\0';
	}
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

		mov     ecx, dword ptr [count]
		mov     eax, dword ptr [src]
		push    ecx
		push    eax
		call    _tcsnlen
		mov     ecx, dword ptr [dest + 8]
		add     esp, 8
#ifdef _UNICODE
		add     eax, eax
#else
		test    eax, eax
#endif
		jz      L1
		push    eax
		push    ecx
		call    _tcslen
		mov     ecx, dword ptr [dest + 8]
		mov     edx, dword ptr [src + 8]
#ifdef _UNICODE
		lea     eax, [ecx + eax * 2]
#else
		add     eax, ecx
#endif
		mov     ecx, dword ptr [esp]
		add     ecx, eax
		push    edx
		mov     dword ptr [esp + 8], ecx
		push    eax
		call    memcpy
		mov     ecx, dword ptr [esp + 12]
		mov     eax, dword ptr [dest + 16]
		add     esp, 16
#ifdef _UNICODE
		mov     word ptr [ecx], '\0'
#else
		mov     byte ptr [ecx], '\0'
#endif
		ret

		align   16
	L1:
		mov     eax, ecx
		ret

		#undef dest
		#undef src
		#undef count
	}
}
#endif
