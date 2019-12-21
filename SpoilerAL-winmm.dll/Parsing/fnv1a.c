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
		#define basis  0x811C9DC5
		#define prime  0x01000193
		#define data   ecx
		#define size   edx
		#define hash   (esp + 4)
		#define r0     eax
		#define r1     ebx
		#define r1b    bl
		#define r2     esi

#ifndef FNV1A_USE_COMBINE
		push    ebx                             // 00000000 _ 53
		push    esi                             // 00000001 _ 56
		mov     r0, basis                       // 00000002 _ B8, 811C9DC5
		xor     r1, r1                          // 00000007 _ 33. DB
		test    size, size                      // 00000009 _ 85. D2
		jnz     L2                              // 0000000B _ 75, 03
		pop     esi                             // 0000000D _ 5E
		pop     ebx                             // 0000000E _ 5B
		ret                                     // 0000000F _ C3
#else
		mov     r0, dword ptr [hash]            // 00000000 _ 8B. 44 24, 04
		test    size, size                      // 00000004 _ 85. D2
		push    ebx                             // 00000006 _ 53
		jnz     L1                              // 00000007 _ 75, 04
		pop     ebx                             // 00000009 _ 5B
		ret     4                               // 0000000A _ C2, 0004
	L1:
		push    esi                             // 0000000D _ 56
		xor     r1, r1                          // 0000000E _ 33. DB
#endif
	L2:
		mov     r1b, byte ptr [data]            // 00000010 _ 8A. 19
		inc     data                            // 00000012 _ 41
		xor     r0, r1                          // 00000013 _ 33. C3
		mov     r1, r0                          // 00000015 _ 8B. D8
		lea     r2, [r0 + r0 * 4]               // 00000017 _ 8D. 34 80
		shl     r0, 24                          // 0000001A _ C1. E0, 18
		lea     r2, [r2 + r1 * 8]               // 0000001D _ 8D. 34 DE
		sub     r0, r2                          // 00000020 _ 2B. C6
		lea     r2, [r2 * 4]                    // 00000022 _ 8D. 34 B5, 00000000
		xor     r1, r1                          // 00000029 _ 33. DB
		lea     r0, [r0 + r2 * 8]               // 0000002B _ 8D. 04 F0
		dec     size                            // 0000002E _ 4A
		jnz     L2                              // 0000002F _ 75, DF
		pop     esi                             // 00000031 _ 5E
		pop     ebx                             // 00000032 _ 5B
#ifndef FNV1A_USE_COMBINE
		ret                                     // 00000033 _ C3
#else
		ret     4                               // 00000033 _ C2, 0004
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
	}
}

#ifndef FNV1A_USE_COMBINE
__declspec(naked) uint64_t __msreturn __msfastcall fnv1a64(const void *data, size_t size)
#else
__declspec(naked) uint64_t __msreturn __msfastcall fnv1a64combine(const void *data, size_t size, uint64_t hash)
#endif
{
	__asm
	{
		#define basis_lo 0x84222325
		#define basis_hi 0xCBF29CE4
		#define prime_lo 0x000001B3
		#define prime_hi 0x00000100
		#define data     ecx
		#define size     edi
		#define hash_lo  (esp + 4)
		#define hash_hi  (esp + 8)
		#define lo       eax
		#define hi       edx
		#define r1       ebx
		#define r1b      bl
		#define r2       ebp
		#define r3       esi

#ifndef FNV1A_USE_COMBINE
		push    ebx                             // 00000000 _ 53
		push    ebp                             // 00000001 _ 55
		push    esi                             // 00000002 _ 56
		push    edi                             // 00000003 _ 57
		xor     r1, r1                          // 00000004 _ 33. DB
		mov     size, edx                       // 00000006 _ 8B. FA
		mov     lo, basis_lo                    // 00000008 _ B8, 84222325
		mov     hi, basis_hi                    // 0000000D _ BA, CBF29CE4
		test    size, size                      // 00000012 _ 85. FF
		jnz     L2                              // 00000014 _ 75, 0A
		pop     edi                             // 00000016 _ 5F
		pop     esi                             // 00000017 _ 5E
		pop     ebp                             // 00000018 _ 5D
		pop     ebx                             // 00000019 _ 5B
		ret                                     // 0000001A _ C3
		int     3                               // 0000001B _ CC
		int     3                               // 0000001C _ CC
		int     3                               // 0000001D _ CC
		int     3                               // 0000001E _ CC
		int     3                               // 0000001F _ CC
#else
		test    edx, edx                        // 00000000 _ 85. D2
		jnz     L1                              // 00000002 _ 75, 0C
		mov     lo, dword ptr [hash_lo]         // 00000004 _ 8B. 44 24, 04
		mov     hi, dword ptr [hash_hi]         // 00000008 _ 8B. 54 24, 08
		ret     8                               // 0000000C _ C2, 0008
		int     3                               // 0000000F _ CC
	L1:
		push    ebx                             // 00000010 _ 53
		push    ebp                             // 00000011 _ 55
		push    esi                             // 00000012 _ 56
		push    edi                             // 00000013 _ 57
		xor     r1, r1                          // 00000014 _ 33. DB
		mov     size, edx                       // 00000016 _ 8B. FA
		mov     lo, dword ptr [hash_lo + 16]    // 00000018 _ 8B. 44 24, 14
		mov     hi, dword ptr [hash_hi + 16]    // 0000001C _ 8B. 54 24, 18
#endif
	L2:
		mov     r1b, byte ptr [data]            // 00000020 _ 8A. 19
		inc     data                            // 00000022 _ 41
		xor     lo, r1                          // 00000023 _ 33. C3
		mov     r2, hi                          // 00000025 _ 8B. EA
		mov     r3, lo                          // 00000027 _ 8B. F0
		add     lo, lo                          // 00000029 _ 03. C0
		adc     hi, hi                          // 0000002B _ 13. D2
		add     lo, r3                          // 0000002D _ 03. C6
		adc     hi, r2                          // 0000002F _ 13. D5
		mov     r1, lo                          // 00000031 _ 8B. D8
		shl     r1, 4                           // 00000033 _ C1. E3, 04
		mov     r2, hi                          // 00000036 _ 8B. EA
		shld    r2, lo, 4                       // 00000038 _ 0F A4. C5, 04
		shl     r3, 8                           // 0000003C _ C1. E6, 08
		add     lo, r1                          // 0000003F _ 03. C3
		adc     hi, r2                          // 00000041 _ 13. D5
		shld    r2, r1, 3                       // 00000043 _ 0F A4. DD, 03
		shl     r1, 3                           // 00000047 _ C1. E3, 03
		add     hi, r3                          // 0000004A _ 03. D6
		add     lo, r1                          // 0000004C _ 03. C3
		adc     hi, r2                          // 0000004E _ 13. D5
		xor     r1, r1                          // 00000050 _ 33. DB
		dec     size                            // 00000052 _ 4F
		jnz     L2                              // 00000053 _ 75, CB
		pop     edi                             // 00000055 _ 5F
		pop     esi                             // 00000056 _ 5E
		pop     ebp                             // 00000057 _ 5D
		pop     ebx                             // 00000058 _ 5B
#ifndef FNV1A_USE_COMBINE
		ret                                     // 00000059 _ C3
#else
		ret     8                               // 00000059 _ C2, 0008
#endif

		#undef basis_lo
		#undef basis_hi
		#undef prime_lo
		#undef prime_hi
		#undef data
		#undef size
		#undef hash_lo
		#undef hash_hi
		#undef lo
		#undef hi
		#undef r1
		#undef r1b
		#undef r2
		#undef r3
	}
}
#endif
