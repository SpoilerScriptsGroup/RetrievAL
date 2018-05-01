#include <string.h>
#include <tchar.h>

#ifndef _M_IX86
TCHAR * __cdecl _tcslcat(TCHAR *string1, const TCHAR *string2, size_t count)
{
	size_t length;

	length = _tcsnlen(string1, count);
	if (count > length)
	{
		TCHAR *dest;

		count -= length;
		(dest = string1 + length)[count = _tcsnlen(string2, count - 1)] = '\0';
		memcpy(dest, string2, count * sizeof(TCHAR));
	}
	return string1;
}
#else
#pragma function(_tcslen, memcpy)
__declspec(naked) TCHAR * __cdecl _tcslcat(TCHAR *string1, const TCHAR *string2, size_t count)
{
	__asm
	{
#ifdef _UNICODE
		#define tchar_ptr word ptr
#else
		#define tchar_ptr byte ptr
#endif

		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		mov     ecx, dword ptr [count]
		mov     eax, dword ptr [string2]
		push    ecx
		push    eax
		call    _tcsnlen
		mov     ecx, dword ptr [count + 8]
		mov     edx, dword ptr [string2 + 8]
		sub     ecx, eax
		jbe     L1
		dec     ecx
		mov     dword ptr [esp + 4], eax
		mov     dword ptr [esp], ecx
		push    edx
		call    _tcsnlen
		mov     ecx, dword ptr [esp + 8]
		mov     edx, dword ptr [string1 + 12]
#ifdef _UNICODE
		add     ecx, ecx
		add     eax, eax
#endif
		add     edx, ecx
		mov     ecx, dword ptr [string2 + 12]
		mov     dword ptr [esp + 8], eax
		mov     dword ptr [esp + 4], ecx
		mov     dword ptr [esp], edx
		mov     tchar_ptr [eax + edx], '\0'
		call    memcpy
		mov     eax, dword ptr [string1 + 12]
		add     esp, 12
		ret

		align   16
	L1:
		mov     eax, dword ptr [string1 + 8]
		add     esp, 8
		ret

		#undef tchar_ptr
		#undef string1
		#undef string2
		#undef count
	}
}
#endif
