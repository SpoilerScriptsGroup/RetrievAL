#include <string.h>
#include <tchar.h>

#ifndef _M_IX86
size_t __cdecl _tcslcpy(TCHAR *dest, const TCHAR *src, size_t count)
{
	size_t length;

	if (length = count)
	{
		length = _tcsnlen(src, count);
		count = length - (length == count);
		dest[count] = '\0';
		memcpy(dest, src, count * sizeof(TCHAR));
	}
	return length;
}
#else
#pragma function(memcpy)
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

		mov     eax, dword ptr [count]
		mov     ecx, dword ptr [src]
		test    eax, eax
		jz      L1
		push    eax
		push    ecx
		call    _tcsnlen
		mov     edx, dword ptr [count + 8]
		add     esp, 8
		xor     ecx, ecx
		cmp     eax, edx
		sete    cl
		push    eax
		sub     eax, ecx
#ifdef _UNICODE
		add     eax, eax
#endif
		mov     edx, dword ptr [src + 4]
		mov     ecx, dword ptr [dest + 4]
		push    eax
		push    edx
		push    ecx
		mov     tchar_ptr [eax + ecx], '\0'
		call    memcpy
		mov     eax, dword ptr [esp + 12]
		add     esp, 16
	L1:
		ret

		#undef tchar_ptr
		#undef dest
		#undef src
		#undef count
	}
}
#endif
