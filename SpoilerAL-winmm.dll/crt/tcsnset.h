#include "noinline_wchar.h"
#include <tchar.h>

#ifdef _UNICODE
#define _tmemset wmemset
typedef wint_t int_t;
#else
#pragma function(memset)
#define _tmemset memset
typedef int int_t;
#endif

#ifndef _M_IX86
TCHAR * __cdecl _tcsnset(TCHAR *string, int_t c, size_t count)
{
	return _tmemset(string, c, _tcsnlen(string, count));
}
#else
__declspec(naked) TCHAR * __cdecl _tcsnset(TCHAR *string, int_t c, size_t count)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]
		mov     ecx, dword ptr [string]
		push    edx
		push    ecx
		call    _tcsnlen
		mov     edx, dword ptr [c + 8]
		mov     ecx, dword ptr [string + 8]
		mov     dword ptr [esp + 4], eax
		mov     dword ptr [esp], edx
		push    ecx
		call    _tmemset
		add     esp, 12
		ret

		#undef string
		#undef c
		#undef count
	}
}
#endif
