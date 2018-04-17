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
#endif

#ifdef __BORLANDC__
uint64_t __msreturn __fastcall fnv1a64(const void *data, size_t size)
{
	uint64_t ret;
	FNV1A64(ret, data, size);
	return ret;
}
#endif
