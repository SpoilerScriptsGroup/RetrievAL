#include <string.h>
#include <tchar.h>

#ifndef _M_IX86
TCHAR * __cdecl _tcsncat(TCHAR *dest, const TCHAR *src, size_t count)
{
	if (count)
	{
		size_t length;

		length = _tcsnlen(src, count);
		length += length < count;
		memcpy(dest + _tcslen(dest), src, length * sizeof(TCHAR));
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

		mov     eax, dword ptr [count]
		mov     ecx, dword ptr [src]
		test    eax, eax
		jz      L2
		push    eax
		push    ecx
		call    _tcsnlen
		mov     edx, dword ptr [count + 8]
		add     esp, 8
		xor     ecx, ecx
		cmp     eax, edx
		setb    cl
#ifdef _UNICODE
		add     eax, eax
		add     ecx, ecx
#endif
		add     eax, ecx
		mov     ecx, dword ptr [dest]
		push    eax
		push    ecx
		call    _tcslen
		mov     ecx, dword ptr [dest + 8]
		add     esp, 8
		mov     edx, dword ptr [src]
#ifdef _UNICODE
		lea     ecx, [ecx + eax * 2]
#else
		add     ecx, eax
#endif
		push    edx
		push    ecx
		call    memcpy
		mov     eax, dword ptr [dest + 12]
		add     esp, 12
		ret

		align   16
	L2:
		mov     eax, dword ptr [dest]
		ret

		#undef dest
		#undef src
		#undef count
	}
}
#endif
