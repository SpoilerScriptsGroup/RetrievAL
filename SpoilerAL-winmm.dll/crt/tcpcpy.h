#include <tchar.h>

#ifdef _UNICODE
#define _tcpcpy wcpcpy
#else
#define _tcpcpy stpcpy
#endif

#ifndef _M_IX86
TCHAR * __cdecl _tcpcpy(char *string1, const char *string2)
{
	size_t length = _tcslen(src);
	return (TCHAR *)memcpy(dest, src, (length + 1) * sizeof(TCHAR)) + length;
}
#else
#pragma function(_tcslen, memcpy)
__declspec(naked) char * __cdecl _tcpcpy(char *string1, const char *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		mov     eax, dword ptr [string2]                // src
		sub     esp, 8
		push    eax
		push    eax
		call    _tcslen                                 // length of src
		mov     ecx, dword ptr [string1 + 16]           // dest
#ifdef _UNICODE
		lea     eax, [eax + eax + 2]                    // include terminating zero in length
#else
		inc     eax
#endif
		mov     dword ptr [esp], ecx
#ifdef _UNICODE
		lea     ecx, [ecx + eax - 2]
#else
		lea     ecx, [ecx + eax - 1]
#endif
		mov     dword ptr [esp + 8], eax
		mov     dword ptr [esp + 12], ecx
		call    memcpy                                  // copy
		mov     eax, dword ptr [esp + 12]
		add     esp, 16                                 // clean up stack
		ret

		#undef string1
		#undef string2
	}
}
#endif
