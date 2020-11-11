#include "noinline_wchar.h"
#include <tchar.h>

#ifdef _UNICODE
#define _tmemset wmemset
typedef wint_t int_t;
#else
#pragma function(strlen, memset)
#define _tmemset memset
typedef int int_t;
#endif

#ifndef _M_IX86
TCHAR * __cdecl _tcsset(TCHAR *string, int_t c)
{
	return _tmemset(string, c, _tcslen(string));
}
#else
__declspec(naked) TCHAR * __cdecl _tcsset(TCHAR *string, int_t c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		push    dword ptr [string]
		call    _tcslen
		mov     edx, dword ptr [c + 4]
		mov     ecx, dword ptr [string + 4]
		mov     dword ptr [esp], eax
		push    edx
		push    ecx
		call    _tmemset
		add     esp, 12
		ret

		#undef string
		#undef c
	}
}
#endif
