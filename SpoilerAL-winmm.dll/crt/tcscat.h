#include <tchar.h>

#pragma function(_tcscat)

#ifndef _M_IX86
TCHAR * __cdecl _tcscat(TCHAR *string1, const TCHAR *string2)
{
	memcpy(string1 + _tcslen(string1), string2, (_tcslen(string2) + 1) * sizeof(TCHAR));
	return string1;
}
#else
#pragma function(_tcslen, memcpy)
__declspec(naked) TCHAR * __cdecl _tcscat(TCHAR *string1, const TCHAR *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    esi
		push    edi
		mov     esi, dword ptr [string2 + 8]            // src
		mov     edi, dword ptr [string1 + 8]            // dest
		push    esi
		push    edi
		call    _tcslen                                 // length of dest
		pop     ecx
#ifdef _UNICODE
		lea     edi, [edi + eax * 2]                    // dest + strlen(dest)
#else
		add     edi, eax
#endif
		call    _tcslen                                 // length of src
		push    esi
#ifdef _UNICODE
		lea     eax, [eax * 2 + 2]                      // include terminating zero in length
#else
		inc     eax
#endif
		mov     dword ptr [esp + 4], eax
		push    edi
		call    memcpy                                  // copy
		mov     eax, dword ptr [string1 + 20]           // return dest
		add     esp, 12                                 // clean up stack
		pop     edi
		pop     esi
		ret

		#undef string1
		#undef string2
	}
}
#endif
