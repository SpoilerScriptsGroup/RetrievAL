#include <stdlib.h>

#pragma warning(disable:4028)

size_t __fastcall _ui32to10a(unsigned __int32 value, char *buffer);

#ifndef _M_IX86
char * __cdecl _ltoa(long value, char *string, int radix)
{
	char *p;

	if (radix != 10)
		return _ultoa(value, string, radix);
	p = string;
	if (value < 0)
	{
		*(p++) = '-';
		value = -value;
	}
	_ui32to10a(value, p);
	return string;
}
#else
__declspec(naked) char * __cdecl _ltoa(long value, char *string, int radix)
{
	__asm
	{
		cmp     dword ptr [esp + 12], 10
		je      L1
		jmp     _ultoa
	L1:
		mov     ecx, dword ptr [esp + 4]
		mov     edx, dword ptr [esp + 8]
		test    ecx, ecx
		jns     L2
		neg     ecx
		mov     byte ptr [edx], '-'
		inc     edx
	L2:
		call    _ui32to10a
		mov     eax, dword ptr [esp + 8]
		ret
	}
}
#endif
