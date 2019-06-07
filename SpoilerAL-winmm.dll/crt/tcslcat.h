#include <string.h>
#include <tchar.h>

#ifndef _M_IX86
size_t __cdecl _tcslcat(TCHAR *dest, const TCHAR *src, size_t count)
{
	size_t length;

	length = _tcsnlen(dest, count);
	if (count > length)
	{
		size_t maxlen;

		maxlen = count - length;
		count = _tcsnlen(src, maxlen);
		dest += length;
		length += count;
		count -= count == maxlen;
		dest[count] = '\0';
		memcpy(dest, src, count * sizeof(TCHAR));
	}
	else if (length = count)
	{
		dest[length - 1] = '\0';
	}
	return length;
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
		mov     ecx, dword ptr [dest + 8]
		mov     ebx, dword ptr [count + 8]
		push    ebx
		push    ecx
		call    _tcsnlen
		sub     ebx, eax
		jbe     L1
		mov     ecx, dword ptr [src + 8]
		mov     esi, eax
		push    ebx
		push    ecx
		call    _tcsnlen
		add     esp, 8
		add     esi, eax
		xor     ecx, ecx
		cmp     eax, ebx
		sete    cl
#ifdef _UNICODE
		add     eax, eax
		add     ecx, ecx
#endif
		sub     eax, ecx
		mov     ecx, dword ptr [dest + 8]
		mov     edx, dword ptr [src + 8]
		add     ecx, esi
		push    eax
		push    edx
		push    ecx
		mov     tchar_ptr [eax + ecx], '\0'
		call    memcpy
		add     esp, 12
		mov     eax, esi
		pop     esi
		pop     ebx
		ret

		align   16
	L1:
		add     eax, ebx
		mov     ecx, dword ptr [dest]
		test    eax, eax
		jz      L2
#ifdef _UNICODE
		mov     tchar_ptr [ecx + eax * 2 - 2], '\0'
#else
		mov     tchar_ptr [ecx + eax - 1], '\0'
#endif
	L2:
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
