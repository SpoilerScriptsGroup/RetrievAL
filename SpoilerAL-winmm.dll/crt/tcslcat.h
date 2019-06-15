#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#ifndef _M_IX86
size_t __cdecl _tcslcat(TCHAR *dest, const TCHAR *src, size_t count)
{
	size_t destLength, srcLength;

	destLength = _tcslen(dest);
	srcLength = _tcslen(src);
	if (count > destLength)
	{
		count -= destLength;
		dest += destLength;
		if (count = min(count - 1, srcLength))
			memcpy(dest, src, count * sizeof(TCHAR));
		dest[count] = '\0';
	}
	return destLength + srcLength;
}
#else
#pragma function(_tcslen, memcpy)
__declspec(naked) size_t __cdecl _tcslcat(TCHAR *dest, const TCHAR *src, size_t count)
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

		push    ebx
		push    esi
		push    edi
		mov     edi, dword ptr [dest + 12]
		mov     esi, dword ptr [src + 12]
		push    edi
		call    _tcslen
		mov     dword ptr [esp], esi
		mov     ebx, eax
		call    _tcslen
		mov     ecx, dword ptr [count + 16]
		pop     edx
		sub     ecx, ebx
		jbe     L2
		dec     ecx
#ifdef _UNICODE
		lea     edi, [edi + ebx * 2]
#else
		add     edi, ebx
#endif
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
		add     eax, ebx
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef tchar_ptr
		#undef dest
		#undef src
		#undef count
	}
}
#endif
