#include <stdlib.h>

#pragma warning(disable:4028)

size_t __fastcall _ui32to10w(unsigned __int32 value, wchar_t *buffer);

#ifndef _M_IX86
wchar_t * __cdecl _ltow(long value, wchar_t *string, int radix)
{
	wchar_t *p;

	if (radix != 10)
		return _ultow(value, string, radix);
	p = string;
	if (value < 0)
	{
		*(p++) = L'-';
		value = -value;
	}
	_ui32to10w(value, p);
	return string;
}
#else
__declspec(naked) wchar_t * __cdecl _ltow(long value, wchar_t *string, int radix)
{
	__asm
	{
		cmp     dword ptr [esp + 12], 10
		je      L1
		jmp     _ultow
	L1:
		mov     ecx, dword ptr [esp + 4]
		mov     edx, dword ptr [esp + 8]
		test    ecx, ecx
		jns     L2
		neg     ecx
		mov     word ptr [edx], '-'
		add     edx, 2
	L2:
		call    _ui32to10w
		mov     eax, dword ptr [esp + 8]
		ret
	}
}
#endif
