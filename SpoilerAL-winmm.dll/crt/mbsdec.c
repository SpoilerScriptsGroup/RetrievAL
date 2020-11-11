#include <windows.h>

#ifndef _M_IX86
unsigned char * __cdecl _mbsdec(const unsigned char *start, const unsigned char *current)
{
	const unsigned char *p;

	p = NULL;
	if (start < current)
		while ((start += IsDBCSLeadByteEx(CP_THREAD_ACP, *(p = start)) ? 2 : 1) < current);
	return (unsigned char *)p;
}
#else
__declspec(naked) unsigned char * __cdecl _mbsdec(const unsigned char *start, const unsigned char *current)
{
	__asm
	{
		#define start   (esp + 4)
		#define current (esp + 8)

		push    ebx                                     // 00000000 _ 53
		push    esi                                     // 00000001 _ 56
		push    edi                                     // 00000002 _ 57
		mov     ebx, -2                                 // 00000003 _ BB, FFFFFFFE
		mov     esi, dword ptr [start + 12]             // 00000008 _ 8B. 74 24, 10
		mov     edi, dword ptr [current + 12]           // 0000000C _ 8B. 7C 24, 14
		sub     esi, edi                                // 00000010 _ 2B. F7
		jb      L1                                      // 00000012 _ 72, 08
		xor     eax, eax                                // 00000014 _ 33. C0
		pop     edi                                     // 00000016 _ 5F
		pop     esi                                     // 00000017 _ 5E
		pop     ebx                                     // 00000018 _ 5B
		ret                                             // 00000019 _ C3

		align   4                                       // 0000001A _ 66, 90
	L1:
		xor     ecx, ecx                                // 0000001C _ 33. C9
		add     ebx, edi                                // 0000001E _ 03. DF

		align   16
	L2:
		mov     cl, byte ptr [esi + edi]                // 00000020 _ 8A. 0C 3E
		add     esi, edi                                // 00000023 _ 03. F7
		push    ecx                                     // 00000025 _ 51
		push    CP_THREAD_ACP                           // 00000026 _ 6A, 03
		call    IsDBCSLeadByteEx                        // 00000028 _ FF. 15, ????????(d)
		xor     ecx, ecx                                // 0000002E _ 33. C9
		cmp     eax, 1                                  // 00000030 _ 83. F8, 01
		sbb     esi, ebx                                // 00000033 _ 1B. F3
		jb      L2                                      // 00000035 _ 72, E9

		cmp     eax, 1                                  // 00000037 _ 83. F8, 01
		pop     edi                                     // 0000003A _ 5F
		adc     ebx, esi                                // 0000003B _ 13. DE
		pop     esi                                     // 0000003D _ 5E
		mov     eax, ebx                                // 0000003E _ 8B. C3
		pop     ebx                                     // 00000040 _ 5B
		ret                                             // 00000041 _ C3

		#undef start
		#undef current
	}
}
#endif
