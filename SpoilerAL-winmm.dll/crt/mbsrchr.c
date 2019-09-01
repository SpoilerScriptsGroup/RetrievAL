#include <windows.h>
#include <mbstring.h>

#pragma warning(disable:4414)

#ifndef _M_IX86
unsigned char * __cdecl _mbsrchr(const unsigned char *string, unsigned int c)
{
	const unsigned char *p;
	size_t              n;

	if (!c)
		return (unsigned char *)string + strlen((char *)string);
	p = NULL;
	if (c <= 0xFFFF)
		for (n = c <= 0xFF ? 1 : 2; string = _mbschr(string, c); string += n)
			p = string;
	return (unsigned char *)p;
}
#else
#pragma function(strlen)

__declspec(naked) unsigned char * __cdecl _mbsrchr(const unsigned char *string, unsigned int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		push    ebx
		push    esi
		mov     ebx, dword ptr [c + 8]
		mov     esi, dword ptr [string + 8]
		test    ebx, ebx
		jnz     L1
		push    esi
		call    strlen
		pop     ecx
		add     eax, esi
		jmp     L5

		align   16
	L1:
		push    edi
		xor     edi, edi
		cmp     ebx, 0FFFFH
		ja      L4
		push    ebx
		push    esi
		mov     esi, 2
		cmp     ebx, 100H
		sbb     esi, 0
		jmp     L3

		align   16
	L2:
		mov     edi, eax
		add     eax, esi
		mov     dword ptr [esp    ], eax
		mov     dword ptr [esp + 4], ebx
	L3:
		call    _mbschr
		test    eax, eax
		jnz     L2
		add     esp, 8
	L4:
		mov     eax, edi
		pop     edi
	L5:
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}
#endif
