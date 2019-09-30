#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#ifndef _M_IX86
size_t __cdecl _tcslcat(TCHAR *dest, const TCHAR *src, size_t count)
{
	size_t destLength, srcLength;

	destLength = _tcslen(dest);
	srcLength = _tcslen(src);
	if (count > destLength + 1)
	{
		(dest += destLength)[count = min(count - (destLength + 1), srcLength)] = '\0';
		if (count *= sizeof(TCHAR))
			memcpy(dest, src, count);
	}
	return destLength + srcLength;
}
#else
#pragma function(_tcslen, memcpy)
__declspec(naked) size_t __cdecl _tcslcat(TCHAR *dest, const TCHAR *src, size_t count)
{
#ifdef _UNICODE
	#define tchar_ptr    word ptr
	#define sizeof_tchar 2
#else
	#define tchar_ptr    byte ptr
	#define sizeof_tchar 1
#endif

	__asm
	{
		#define dest  (esp + 4)
		#define src   (esp + 8)
		#define count (esp + 12)

		push    esi
		push    edi
		mov     edi, dword ptr [dest + 8]
		mov     esi, dword ptr [src + 8]
		push    ebx
		push    edi
		call    _tcslen
		mov     dword ptr [esp], esi
		lea     ebx, [eax + 1]
		call    _tcslen
		mov     ecx, dword ptr [count + 16]
		lea     edi, [edi + ebx * sizeof_tchar - sizeof_tchar]
		sub     ecx, ebx
		jbe     L3
		cmp     ecx, eax
		ja      L1
		mov     tchar_ptr [edi + ecx * sizeof_tchar], '\0'
		jmp     L2
	L1:
		mov     tchar_ptr [edi + eax * sizeof_tchar], '\0'
		mov     ecx, eax
	L2:
#ifdef _UNICODE
		add     ecx, ecx
#else
		test    ecx, ecx
#endif
		jz      L3
		add     ebx, eax
		mov     dword ptr [esp], ecx
		push    esi
		push    edi
		call    memcpy
		add     esp, 12
		mov     eax, ebx
		pop     ebx
		pop     edi
		pop     esi
		dec     eax
		ret

		align   16
	L3:
		add     eax, ebx
		pop     ecx
		pop     ebx
		pop     edi
		pop     esi
		dec     eax
		ret

		#undef dest
		#undef src
		#undef count
	}

	#undef tchar_ptr
	#undef sizeof_tchar
}
#endif
