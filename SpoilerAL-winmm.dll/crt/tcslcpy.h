#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#ifndef _M_IX86
size_t __cdecl _tcslcpy(TCHAR *dest, const TCHAR *src, size_t count)
{
	size_t length;

	length = _tcslen(src);
	if (count)
	{
		if (count = min(count - 1, length))
			memcpy(dest, src, count * sizeof(TCHAR));
		dest[count] = '\0';
	}
	return length;
}
#else
#pragma function(_tcslen, memcpy)
__declspec(naked) size_t __cdecl _tcslcpy(TCHAR *dest, const TCHAR *src, size_t count)
{
	__asm
	{
#ifdef _UNICODE
		#define tchar_ptr word ptr
#else
		#define tchar_ptr byte ptr
#endif

		#define dest  (esp + 4)
		#define src   (esp + 8)
		#define count (esp + 12)

		push    esi
		push    edi
		mov     esi, dword ptr [src + 8]
		mov     edi, dword ptr [dest + 8]
		push    esi
		call    _tcslen
		mov     ecx, dword ptr [count + 12]
		pop     edx
		sub     ecx, 1
		jb      L2
		push    eax
		cmp     eax, ecx
		cmova   eax, ecx
#ifdef _UNICODE
		add     eax, eax
		jbe     L1
#else
		test    eax, eax
		jz      L1
#endif
		push    eax
		push    esi
		push    edi
		add     edi, eax
		call    memcpy
		add     esp, 12
	L1:
		mov     tchar_ptr [edi], '\0'
		pop     eax
	L2:
		pop     edi
		pop     esi
		ret

		#undef tchar_ptr
		#undef dest
		#undef src
		#undef count
	}
}
#endif
