#include "fnv1a.h"

#ifdef _M_IX86
#ifdef __BORLANDC__
#pragma warn -8070
#endif
#ifndef FNV1A_USE_COMBINE
__declspec(naked) uint32_t __msfastcall fnv1a32(const void *data, size_t size)
#else
__declspec(naked) uint32_t __msfastcall fnv1a32combine(const void *data, size_t size, uint32_t hash)
#endif
{
	__asm
	{
		#define basis 0x811C9DC5
		#define prime 0x01000193
		#define data  ecx
		#define size  edx
		#define hash  (esp + 4)
		#define r0    eax
		#define r1    ebx
		#define r1b   bl
		#define r2    esi
		#define r3    edi
		#define r4    ebp

#ifndef FNV1A_USE_COMBINE
		push    ebx                             // 00000000 _ 53
		push    ebp                             // 00000001 _ 55
		push    esi                             // 00000002 _ 56
		push    edi                             // 00000003 _ 57
		mov     r0, basis                       // 00000004 _ B8, 811C9DC5
		xor     r1, r1                          // 00000009 _ 33. DB
		xor     r2, r2                          // 0000000B _ 33. F6
		xor     r3, r3                          // 0000000D _ 33. FF
		test    size, size                      // 0000000F _ 85. D2
		jnz     L1                              // 00000011 _ 75, 0D
		pop     edi                             // 00000013 _ 5F
		pop     esi                             // 00000014 _ 5E
		pop     ebp                             // 00000015 _ 5D
		pop     ebx                             // 00000016 _ 5B
		ret                                     // 00000017 _ C3
		int     3                               // 00000018 _ CC
#else
		push    ebx                             // 00000000 _ 53
		push    ebp                             // 00000001 _ 55
		push    esi                             // 00000002 _ 56
		push    edi                             // 00000003 _ 57
		xor     r1, r1                          // 00000004 _ 33. DB
		xor     r2, r2                          // 00000006 _ 33. F6
		xor     r3, r3                          // 00000008 _ 33. FF
		mov     r0, dword ptr [hash + 16]       // 0000000A _ 8B. 44 24, 14
		test    size, size                      // 0000000E _ 85. D2
		jnz     L1                              // 00000010 _ 75, 0E
		pop     edi                             // 00000012 _ 5F
		pop     esi                             // 00000013 _ 5E
		pop     ebp                             // 00000014 _ 5D
		pop     ebx                             // 00000015 _ 5B
		ret     4                               // 00000016 _ C2, 0004
#endif
		int     3                               // 00000019 _ CC
		int     3                               // 0000001A _ CC
		int     3                               // 0000001B _ CC
		int     3                               // 0000001C _ CC
		int     3                               // 0000001D _ CC
		int     3                               // 0000001E _ CC
		int     3                               // 0000001F _ CC
	L1:
		mov     r1b, byte ptr [data]            // 00000020 _ 8A. 19
		lea     r0, [r0 + r2 * 4]               // 00000022 _ 8D. 04 B0
		xor     r0, r1                          // 00000025 _ 33. C3
		inc     data                            // 00000027 _ 41
		mov     r4, r0                          // 00000028 _ 8B. E8
		lea     r2, [r0 + r0 * 4]               // 0000002A _ 8D. 34 80
		shl     r0, 24                          // 0000002D _ C1. E0, 18
		lea     r2, [r2 + r4 * 8]               // 00000030 _ 8D. 34 EE
		sub     r0, r2                          // 00000033 _ 2B. C6
		lea     r2, [r3 + r2 * 8]               // 00000035 _ 8D. 34 F7
		dec     size                            // 00000038 _ 4A
		jnz     L1                              // 00000039 _ 75, E5
		lea     r0, [r0 + r2 * 4]               // 0000003B _ 8D. 04 B0
		pop     edi                             // 0000003E _ 5F
		pop     esi                             // 0000003F _ 5E
		pop     ebp                             // 00000040 _ 5D
		pop     ebx                             // 00000041 _ 5B
#ifndef FNV1A_USE_COMBINE
		ret                                     // 00000042 _ C3
#else
		ret     4                               // 00000042 _ C2, 0004
#endif

		#undef basis
		#undef prime
		#undef data
		#undef size
		#undef hash
		#undef r0
		#undef r1
		#undef r1b
		#undef r2
		#undef r3
		#undef r4
	}
}
#endif
