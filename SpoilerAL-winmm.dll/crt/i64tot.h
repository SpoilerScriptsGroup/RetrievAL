#include <stdlib.h>
#include <tchar.h>

#pragma warning(disable:4028 4414)

#ifdef _UNICODE
size_t __fastcall _ui64to10w(unsigned __int64 value, wchar_t *buffer);
#define _ui64to10t _ui64to10w
#else
size_t __fastcall _ui64to10a(unsigned __int64 value, char *buffer);
#define _ui64to10t _ui64to10a
#endif

#ifndef _M_IX86
wchar_t * __cdecl _i64tot(__int64 value, TCHAR *string, int radix)
{
	TCHAR *p;

	if (radix != 10)
		return _ui64tot(value, string, radix);
	p = string;
	if (value < 0)
	{
		*(p++) = '-';
		value = -value;
	}
	_ui64to10t(value, p);
	return string;
}
#else
__declspec(naked) TCHAR * __cdecl _i64tot(__int64 value, TCHAR *string, int radix)
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
		mov     ecx, dword ptr [esp + 12]
		mov     eax, dword ptr [esp + 16]
		cmp     eax, 10
		jne     _ui64tot
		mov     eax, dword ptr [esp + 4]
		mov     edx, dword ptr [esp + 8]
		test    edx, edx
		jns     L1
		mov     tchar ptr [ecx], '-'
		xor     edx, -1
		neg     eax
		sbb     edx, -1
		inc_tchar(ecx)
	L1:
		push    edx
		push    eax
		call    _ui64to10t
		mov     eax, dword ptr [esp + 12]
		ret
	}

	#undef tchar
	#undef inc_tchar
}
#endif
