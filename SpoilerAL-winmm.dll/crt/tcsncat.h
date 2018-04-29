#include <string.h>
#include <tchar.h>

#ifndef _M_IX86
TCHAR * __cdecl _tcsncat(TCHAR *string1, const TCHAR *string2, size_t count)
{
	TCHAR  *dest;
	size_t length;

	count = _tcsnlen(string2, count);
	dest = string1 + _tcslen(string1);
	dest[count] = '\0';
	memcpy(dest, string2, count * sizeof(TCHAR));
	return string1;
}
#else
#pragma function(_tcslen, memcpy)
__declspec(naked) TCHAR * __cdecl _tcsncat(TCHAR *string1, const TCHAR *string2, size_t count)
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
#ifdef _UNICODE
		add     eax, eax
#endif
		mov     ecx, dword ptr [string1 + 8]
		mov     dword ptr [esp + 4], eax
		mov     dword ptr [esp], ecx
		call    _tcslen
		mov     ecx, dword ptr [string1 + 8]
		mov     edx, dword ptr [string2 + 8]
#ifdef _UNICODE
		lea     eax, [ecx + eax * 2]
#else
		add     eax, ecx
#endif
		mov     ecx, dword ptr [esp + 4]
		mov     dword ptr [esp], edx
		push    eax
		mov     tchar_ptr [eax + ecx], '\0'
		call    memcpy
		mov     eax, dword ptr [string1 + 12]
		add     esp, 12
		ret

		#undef tchar_ptr
		#undef string1
		#undef string2
		#undef count
	}
}
#endif
