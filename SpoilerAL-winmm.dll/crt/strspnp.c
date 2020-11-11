#ifndef _M_IX86
#include <intrin.h>
#pragma intrinsic(_bittestandset, _bittest)

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
__declspec(naked) char * __cdecl _strspnp(const char *string, const char *control)
{
	__asm
	{
		#define string  (esp + 4)
		#define control (esp + 8)

		mov     eax, dword ptr [string]                     // eax = string
		mov     edx, dword ptr [control]                    // edx = control
		xor     ecx, ecx                                    // create and zero out char bit map
		push    0                                           // 256
		push    ecx                                         // 224
		push    ecx                                         // 192
		push    ecx                                         // 160
		push    ecx                                         // 128
		push    ecx                                         //  96
		push    ecx                                         //  64
		push    ecx                                         //  32
		jmp     listinit

		#define map     (esp)

		// Set control char bits in map
		align   16
	listnext:
		bts     dword ptr [map], ecx                        // init char bit map
	listinit:
		mov     cl, byte ptr [edx]
		inc     edx
		test    cl, cl
		jnz     listnext

		// Loop through comparing source string with control bits
		align   16
	dstnext:
		mov     cl, byte ptr [eax]
		inc     eax
		bt      dword ptr [map], ecx
		jc      dstnext                                     // found char, continue

		// Return code
		dec     eax
		add     ecx, -1
		sbb     ecx, ecx
		add     esp, 32
		and     eax, ecx
		ret                                                 // __cdecl return

		#undef string
		#undef control
		#undef map
	}
}
#endif
