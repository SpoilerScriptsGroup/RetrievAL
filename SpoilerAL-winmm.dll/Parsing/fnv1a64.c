#include "fnv1a.h"

#ifdef _M_IX86
#ifdef __BORLANDC__
#pragma warn -8070
#endif
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
		#define size     ebp
		#define hash_lo  (esp + 4)
		#define hash_hi  (esp + 8)
		#define lo       eax
		#define hi       edx
		#define r1       ebx
		#define r1b      bl
		#define r2       esi
		#define r3       edi

#ifndef FNV1A_USE_COMBINE
		push    ebx                             // 00000000 _ 53
		push    ebp                             // 00000001 _ 55
		push    esi                             // 00000002 _ 56
		push    edi                             // 00000003 _ 57
		xor     r1, r1                          // 00000004 _ 33. DB
		mov     size, edx                       // 00000006 _ 8B. EA
		mov     lo, basis_lo                    // 00000008 _ B8, 84222325
		mov     hi, basis_hi                    // 0000000D _ BA, CBF29CE4
		test    size, size                      // 00000012 _ 85. ED
		jnz     L1                              // 00000014 _ 75, 0A
		pop     edi                             // 00000016 _ 5F
		pop     esi                             // 00000017 _ 5E
		pop     ebp                             // 00000018 _ 5D
		pop     ebx                             // 00000019 _ 5B
		ret                                     // 0000001A _ C3
#else
		push    ebx                             // 00000000 _ 53
		push    ebp                             // 00000001 _ 55
		push    esi                             // 00000002 _ 56
		push    edi                             // 00000003 _ 57
		xor     r1, r1                          // 00000004 _ 33. DB
		mov     size, edx                       // 00000006 _ 8B. EA
		mov     lo, dword ptr [hash_lo + 16]    // 00000008 _ 8B. 44 24, 14
		mov     hi, dword ptr [hash_hi + 16]    // 0000000C _ 8B. 54 24, 18
		test    size, size                      // 00000010 _ 85. ED
		jnz     L1                              // 00000012 _ 75, 0C
		pop     edi                             // 00000014 _ 5F
		pop     esi                             // 00000015 _ 5E
		pop     ebp                             // 00000016 _ 5D
		pop     ebx                             // 00000017 _ 5B
		ret                                     // 00000018 _ C3
		int     3                               // 00000019 _ CC
		int     3                               // 0000001A _ CC
#endif
		int     3                               // 0000001B _ CC
		int     3                               // 0000001C _ CC
		int     3                               // 0000001D _ CC
		int     3                               // 0000001E _ CC
		int     3                               // 0000001F _ CC
	L1:
		mov     r1b, byte ptr [data]            // 00000020 _ 8A. 19
		mov     r2, hi                          // 00000022 _ 8B. F2
		xor     lo, r1                          // 00000024 _ 33. C3
		inc     data                            // 00000026 _ 41
		mov     r3, lo                          // 00000027 _ 8B. F8
		add     lo, lo                          // 00000029 _ 03. C0
		adc     hi, hi                          // 0000002B _ 13. D2
		add     lo, r3                          // 0000002D _ 03. C7
		adc     hi, r2                          // 0000002F _ 13. D6
		mov     r1, lo                          // 00000031 _ 8B. D8
		shr     r1, 28                          // 00000033 _ C1. EB, 1C
		mov     r2, hi                          // 00000036 _ 8B. F2
		shl     r2, 4                           // 00000038 _ C1. E6, 04
		or      r2, r1                          // 0000003B _ 0B. F3
		mov     r1, lo                          // 0000003D _ 8B. D8
		shl     r1, 4                           // 0000003F _ C1. E3, 04
		add     hi, r2                          // 00000042 _ 03. D6
		shl     r3, 8                           // 00000044 _ C1. E7, 08
		add     lo, r1                          // 00000047 _ 03. C3
		adc     hi, r3                          // 00000049 _ 13. D7
		mov     r3, r1                          // 0000004B _ 8B. FB
		shr     r3, 29                          // 0000004D _ C1. EF, 1D
		shl     r1, 3                           // 00000050 _ C1. E3, 03
		add     hi, r3                          // 00000053 _ 03. D7
		shl     r2, 3                           // 00000055 _ C1. E6, 03
		add     lo, r1                          // 00000058 _ 03. C3
		adc     hi, r2                          // 0000005A _ 13. D6
		xor     r1, r1                          // 0000005C _ 33. DB
		dec     size                            // 0000005E _ 4D
		jnz     L1                              // 0000005F _ 75, BF
		pop     edi                             // 00000061 _ 5F
		pop     esi                             // 00000062 _ 5E
		pop     ebp                             // 00000063 _ 5D
		pop     ebx                             // 00000064 _ 5B
#ifndef FNV1A_USE_COMBINE
		ret                                     // 00000065 _ C3
#else
		ret     8                               // 00000065 _ C2, 0008
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
