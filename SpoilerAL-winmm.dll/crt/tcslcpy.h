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
		dest[count = min(count - 1, length)] = '\0';
		if (count *= sizeof(TCHAR))
			memcpy(dest, src, count);
	}
	return length;
}
#else
#pragma function(_tcslen, memcpy)
__declspec(naked) size_t __cdecl _tcslcpy(TCHAR *dest, const TCHAR *src, size_t count)
{
#ifdef _UNICODE
	#define tchar_ptr word ptr
#else
	#define tchar_ptr byte ptr
#endif

	__asm
	{
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
		jb      L3
		cmp     ecx, eax
		ja      L1
		mov     tchar_ptr [edi + ecx * size TCHAR], '\0'
		jmp     L2

	L1:
		mov     tchar_ptr [edi + eax * size TCHAR], '\0'
		mov     ecx, eax
	L2:
#ifdef _UNICODE
		add     ecx, ecx
#else
		test    ecx, ecx
#endif
		jz      L3
		push    ecx
		push    esi
		push    edi
		mov     esi, eax
		call    memcpy
		add     esp, 12
		mov     eax, esi
	L3:
		pop     edi
		pop     esi
		ret

		#undef dest
		#undef src
		#undef count
	}

	#undef tchar_ptr
}
#endif
