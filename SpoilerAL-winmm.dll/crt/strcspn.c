#include <stddef.h>

#ifndef _M_IX86
#include <intrin.h>
#pragma intrinsic(_bittestandset, _bittest)

size_t __cdecl strcspn(const char *string, const char *control)
{
	unsigned long map[0x100 / 32] = { 1 };
	unsigned char c;
	size_t        index;

	while (c = *(control++))
		_bittestandset(map, c);
	index = -1;
	string++;
	do
		c = string[index++];
	while (!_bittest(map, c));
	return index;
}
#else
__declspec(naked) size_t __cdecl strcspn(const char *string, const char *control)
{
	__asm
	{
		#define string  (esp + 4)
		#define control (esp + 8)

		mov     edx, dword ptr [string]                 // edx = string
		mov     eax, dword ptr [control]                // eax = control
		xor     ecx, ecx                                // create and zero out char bit map
		push    0                                       // 256
		push    ecx                                     // 224
		push    ecx                                     // 192
		push    ecx                                     // 160
		push    ecx                                     // 128
		push    ecx                                     //  96
		push    ecx                                     //  64
		push    1                                       //  32
		jmp     listinit

		#define map     (esp)

		// Set control char bits in map
		align   16
	listnext:
		bts     dword ptr [map], ecx                    // init char bit map
	listinit:
		mov     cl, byte ptr [eax]
		inc     eax
		test    cl, cl
		jnz     listnext

		mov     eax, edx                                // eax = string
		inc     edx                                     // edx = string + 1

		// Loop through comparing source string with control bits
		align   16
	dstnext:
		mov     cl, byte ptr [eax]
		inc     eax
		bt      dword ptr [map], ecx
		jnc     dstnext                                 // did not find char, continue

		// Return code
		add     esp, 32
		sub     eax, edx
		ret                                             // __cdecl return

		#undef string
		#undef control
		#undef map
	}
}
#endif
