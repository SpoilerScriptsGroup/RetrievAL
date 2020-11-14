#include <stddef.h>
#ifndef _M_IX86
#include <intrin.h>
#pragma intrinsic(_bittestandset, _bittest)

#ifndef _DEBUG
size_t __cdecl strspn(const char *string, const char *control)
{
	unsigned long map[0x100 / 32] = { 0 };
	unsigned char c, *p;

	while (c = *(control++))
		_bittestandset(map, c);
	p = string++;
	do
		c = *(p++);
	while (_bittest(map, c));
	return p - string;
}

#endif
char * __cdecl _strspnp(const char *string, const char *control)
{
	unsigned long map[0x100 / 32] = { 0 };
	unsigned char c;

	while (c = *(control++))
		_bittestandset(map, c);
	do
		c = *(string++);
	while (_bittest(map, c));
	return c ? (char *)string - 1 : NULL;
}
#else

static unsigned __int64 __fastcall internal_strspnp(const char *string, const char *control);

#ifndef _DEBUG
__declspec(naked) size_t __cdecl strspn(const char *string, const char *control)
{
	__asm
	{
		#define string  (esp + 4)
		#define control (esp + 8)

		mov     ecx, dword ptr [string]                     // ecx = string
		mov     edx, dword ptr [control]                    // edx = control
		call    internal_strspnp
		mov     eax, edx
		ret

		#undef string
		#undef control
	}
}

#endif
__declspec(naked) char * __cdecl _strspnp(const char *string, const char *control)
{
	__asm
	{
		#define string  (esp + 4)
		#define control (esp + 8)

		mov     ecx, dword ptr [string]                     // ecx = string
		mov     edx, dword ptr [control]                    // edx = control
		jmp     internal_strspnp

		#undef string
		#undef control
	}
}

__declspec(naked) static unsigned __int64 __fastcall internal_strspnp(const char *string, const char *control)
{
	__asm
	{
		#define string  ecx
		#define control edx

		xor     eax, eax                                    // create and zero out char bit map
		push    0                                           // 256
		push    eax                                         // 224
		push    eax                                         // 192
		push    eax                                         // 160
		push    eax                                         // 128
		push    eax                                         //  96
		push    eax                                         //  64
		push    eax                                         //  32
		jmp     init

		#define map     (esp)

		// Set control char bits in map
		align   16
	initnext:
		bts     dword ptr [map], eax                        // init char bit map
	init:
		mov     al, byte ptr [edx]
		inc     edx
		test    al, al
		jnz     initnext

		mov     edx, ecx                                    // edx = string

		// Loop through comparing source string with control bits
		align   16
	dstnext:
		mov     al, byte ptr [edx]
		inc     edx
		bt      dword ptr [map], eax
		jc      dstnext                                     // found char, continue

		// Return code
		dec     edx
		add     eax, -1
		sbb     eax, eax
		add     esp, 32
		and     eax, edx
		sub     edx, ecx
		ret                                                 // __cdecl return

		#undef string
		#undef control
		#undef map
	}
}
#endif
