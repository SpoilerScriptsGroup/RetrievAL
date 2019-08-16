#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#ifndef _M_IX86
size_t __cdecl _tcslcat(TCHAR *dest, const TCHAR *src, size_t count)
{
	size_t destLength, srcLength;

	destLength = _tcslen(dest);
	srcLength = _tcslen(src);
#ifdef _UNICODE
	if (count > destLength && (ptrdiff_t)(count = min(count - destLength - 1, srcLength)) > 0)
#else
	if (count > destLength && (count = min(count - destLength - 1, srcLength)))
#endif
	{
		dest += destLength;
		dest[count] = '\0';
		memcpy(dest, src, count * sizeof(TCHAR));
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
		add     esp, 4
		sub     ecx, ebx
		jbe     L1
		cmp     ecx, eax
		lea     ecx, [ecx - 1]
		cmova   ecx, eax
#ifdef _UNICODE
		add     ecx, ecx
		jbe     L1
		push    eax
		lea     edi, [edi + ebx * 2]
#else
		test    ecx, ecx
		jz      L1
		add     edi, ebx
		push    eax
#endif
		push    ecx
		push    esi
		push    edi
		mov     tchar_ptr [edi + ecx], '\0'
		call    memcpy
		mov     eax, dword ptr [esp + 12]
		add     esp, 16
	L1:
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
