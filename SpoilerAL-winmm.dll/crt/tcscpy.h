#include <tchar.h>

#pragma function(_tcscpy)

#ifndef _M_IX86
TCHAR * __cdecl _tcscpy(TCHAR *string1, const TCHAR *string2)
{
	return (TCHAR *)memcpy(string1, string2, (_tcslen(string2) + 1) * sizeof(TCHAR));
}
#else
#pragma function(_tcslen, memcpy)
__declspec(naked) TCHAR * __cdecl _tcscpy(TCHAR *string1, const TCHAR *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		mov     eax, dword ptr [string2]                    // src
		sub     esp, 4
		push    eax
		push    eax
		call    _tcslen                                     // length of src
		mov     ecx, dword ptr [string1 + 12]               // dest
#ifdef _UNICODE
		lea     eax, [eax + eax + 2]                        // include terminating zero in length
#else
		inc     eax
#endif
		mov     dword ptr [esp], ecx
		mov     dword ptr [esp + 8], eax
		call    memcpy                                      // copy
		add     esp, 12                                     // clean up stack
		ret

		#undef string1
		#undef string2
	}
}
#endif
