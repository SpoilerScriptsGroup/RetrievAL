#include <windows.h>

#ifndef _M_IX86
unsigned char * __cdecl _mbsinc(const unsigned char *current)
{
	if (IsDBCSLeadByteEx(CP_THREAD_ACP, *(current++)) && *current)
		current++;
	return (unsigned char *)current;
}
#else
__declspec(naked) unsigned char * __cdecl _mbsinc(const unsigned char *current)
{
	__asm
	{
		#define current (esp + 4)

		mov     ecx, dword ptr [current]                // 00000000 _ 8B. 4C 24, 04
		xor     eax, eax                                // 00000004 _ 33. C0
		mov     al, byte ptr [ecx]                      // 00000006 _ 8A. 01
		push    eax                                     // 00000008 _ 50
		push    CP_THREAD_ACP                           // 00000009 _ 6A, 03
		call    IsDBCSLeadByteEx                        // 0000000B _ FF. 15, ????????(d)
		mov     ecx, eax                                // 00000011 _ 8B. C8
		mov     eax, dword ptr [current]                // 00000013 _ 8B. 44 24, 04
		test    ecx, ecx                                // 00000017 _ 85. C9
		jnz     L1                                      // 00000019 _ 75, 05
		inc     eax                                     // 0000001B _ 40
		jmp     L2                                      // 0000001C _ EB, 0D

		align   16                                      // 0000001E _ 66, 90
	L1:
		mov     cl, byte ptr [eax + 1]                  // 00000020 _ 8A. 48, 01
		add     eax, 2                                  // 00000023 _ 83. C0, 02
		test    cl, cl                                  // 00000026 _ 84. C9
		jnz     L2                                      // 00000028 _ 75, 01
		dec     eax                                     // 0000002A _ 48
	L2
		ret                                             // 0000002B _ C3

		#undef current
	}
}
#endif
