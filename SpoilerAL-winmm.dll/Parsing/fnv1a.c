#include "fnv1a.h"

#ifdef _M_IX86
#ifdef __BORLANDC__
#pragma warn -8070
#endif
__declspec(naked) uint32_t __msfastcall fnv1a32(const void *data, size_t size)
{
	__asm
	{
		#define basis  0x811C9DC5
		#define prime  0x01000193
		#define data   ecx
		#define size   edx
		#define hash   eax
		#define hash_b al
		#define r1     ebx
		#define r2     esi

		push    ebx                     // 00000000 _ 53
		push    esi                     // 00000001 _ 56
		mov     hash, basis             // 00000002 _ B8, 811C9DC5
		test    size, size              // 00000007 _ 85. D2
		jnz     L1                      // 00000009 _ 75, 05
		pop     esi                     // 0000000B _ 5E
		pop     ebx                     // 0000000C _ 5B
		ret                             // 0000000D _ C3
		int     3                       // 0000000E _ CC
		int     3                       // 0000000F _ CC
	L1:
		xor     hash_b, byte ptr [data] // 00000010 _ 32. 01
		inc     data                    // 00000012 _ 41
		lea     r1, [hash + hash * 2]   // 00000013 _ 8D. 1C 40
		shl     hash, 4                 // 00000016 _ C1. E0, 04
		mov     r2, r1                  // 00000019 _ 8B. F3
		shl     r2, 7                   // 0000001B _ C1. E6, 07
		add     r1, hash                // 0000001E _ 03. D8
		shl     hash, 24 - 4            // 00000020 _ C1. E0, 14
		add     r1, r2                  // 00000023 _ 03. DE
		add     hash, r1                // 00000025 _ 03. C3
		dec     size                    // 00000027 _ 4A
		jnz     L1                      // 00000028 _ 75, E6
		pop     esi                     // 0000002A _ 5E
		pop     ebx                     // 0000002B _ 5B
		ret                             // 0000002C _ C3

		#undef basis
		#undef prime
		#undef data
		#undef size
		#undef hash
		#undef hash_b
		#undef r1
		#undef r2
	}
}

__declspec(naked) uint64_t __msreturn __msfastcall fnv1a64(const void *data, size_t size)
{
	__asm
	{
		#define basis_lo 0x84222325
		#define basis_hi 0xCBF29CE4
		#define prime_lo 0x000001B3
		#define prime_hi 0x00000100
		#define data     ebx
		#define size     ebp
		#define lo       eax
		#define lo_b     al
		#define hi       edx
		#define r1       ecx
		#define r2       esi
		#define r3       edi

		push    ebx                     // 00000000 _ 53
		push    ebp                     // 00000001 _ 55
		push    esi                     // 00000002 _ 56
		push    edi                     // 00000003 _ 57
		mov     data, ecx               // 00000004 _ 8B. D9
		mov     size, edx               // 00000006 _ 8B. EA
		mov     lo, basis_lo            // 00000008 _ B8, 84222325
		mov     hi, basis_hi            // 0000000D _ BA, CBF29CE4
		test    size, size              // 00000012 _ 85. ED
		jnz     L1                      // 00000014 _ 75, 0A
		pop     edi                     // 00000016 _ 5F
		pop     esi                     // 00000017 _ 5E
		pop     ebp                     // 00000018 _ 5D
		pop     ebx                     // 00000019 _ 5B
		ret                             // 0000001A _ C3
		int     3                       // 0000001B _ CC
		int     3                       // 0000001C _ CC
		int     3                       // 0000001D _ CC
		int     3                       // 0000001E _ CC
		int     3                       // 0000001F _ CC
	L1:
		xor     lo_b, byte ptr [data]   // 00000020 _ 32. 03
		inc     data                    // 00000022 _ 43
		mov     r1, lo                  // 00000023 _ 8B. C8
		mov     r2, hi                  // 00000025 _ 8B. F2
		mov     r3, lo                  // 00000027 _ 8B. F8
		add     lo, lo                  // 00000029 _ 03. C0
		adc     hi, hi                  // 0000002B _ 13. D2
		add     lo, r1                  // 0000002D _ 03. C1
		adc     hi, r2                  // 0000002F _ 13. D6
		mov     r1, lo                  // 00000031 _ 8B. C8
		shl     r1, 4                   // 00000033 _ C1. E1, 04
		mov     r2, hi                  // 00000036 _ 8B. F2
		shld    r2, lo, 4               // 00000038 _ 0F A4. C6, 04
		shl     r3, 8                   // 0000003C _ C1. E7, 08
		add     lo, r1                  // 0000003F _ 03. C1
		adc     hi, r2                  // 00000041 _ 13. D6
		shld    r2, r1, 3               // 00000043 _ 0F A4. CE, 03
		shl     r1, 3                   // 00000047 _ C1. E1, 03
		add     hi, r3                  // 0000004A _ 03. D7
		add     lo, r1                  // 0000004C _ 03. C1
		adc     hi, r2                  // 0000004E _ 13. D6
		dec     size                    // 00000050 _ 4D
		jnz     L1                      // 00000051 _ 75, CD
		pop     edi                     // 00000053 _ 5F
		pop     esi                     // 00000054 _ 5E
		pop     ebp                     // 00000055 _ 5D
		pop     ebx                     // 00000056 _ 5B
		ret                             // 00000057 _ C3

		#undef basis_lo
		#undef basis_hi
		#undef prime_lo
		#undef prime_hi
		#undef data
		#undef size
		#undef lo
		#undef lo_b
		#undef hi
		#undef r1
		#undef r2
		#undef r3
	}
}
#endif
