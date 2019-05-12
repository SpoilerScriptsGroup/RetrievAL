#include <stdlib.h>
#include <tchar.h>

#pragma warning(disable:4028 4414)

#ifdef _UNICODE
size_t __fastcall _ui32to10w(unsigned __int32 value, wchar_t *buffer);
#define _ui32to10t _ui32to10w
#else
size_t __fastcall _ui32to10a(unsigned __int32 value, char *buffer);
#define _ui32to10t _ui32to10a
#endif

#ifndef _M_IX86
wchar_t * __cdecl _ltot(long value, TCHAR *string, int radix)
{
	TCHAR *p;

	if (radix != 10)
		return _ultot(value, string, radix);
	p = string;
	if (value < 0)
	{
		*(p++) = '-';
		value = -value;
	}
	_ui32to10t(value, p);
	return string;
}
#else
__declspec(naked) TCHAR * __cdecl _ltot(long value, TCHAR *string, int radix)
{
	#ifdef _UNICODE
	#define tchar        word
	#define inc_tchar(r) add r, 2
	#else
	#define tchar        byte
	#define inc_tchar(r) inc r
	#endif

	__asm
	{
		cmp     dword ptr [esp + 12], 10
		jne     _ultot
		mov     ecx, dword ptr [esp + 4]
		mov     edx, dword ptr [esp + 8]
		test    ecx, ecx
		jns     L1
		neg     ecx
		mov     tchar ptr [edx], '-'
		inc_tchar(edx)
	L1:
		call    _ui32to10t
		mov     eax, dword ptr [esp + 8]
		ret
	}

	#undef tchar
	#undef inc_tchar
}
#endif
