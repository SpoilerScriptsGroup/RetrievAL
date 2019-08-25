#include <string.h>
#include <tchar.h>

#ifndef _M_IX86
TCHAR * __cdecl _tcsncpy(TCHAR *dest, const TCHAR *src, size_t count)
{
	if (count)
	{
		size_t length;

		length = _tcsnlen(src, count);
		length += length < count;
		memcpy(dest, src, length * sizeof(TCHAR));
	}
	return dest;
}
#else
#pragma function(memcpy)
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
		add     esp, 8
		cmp     eax, ecx
		adc     eax, 0
#ifdef _UNICODE
		add     eax, eax
#endif
		mov     dword ptr [count], eax
		jmp     memcpy

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
