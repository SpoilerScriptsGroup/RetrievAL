#include "fnv1a.h"

#ifndef _M_IX86

#ifdef __BORLANDC__
uint32_t __msfastcall fnv1a32(const void *data, size_t size)
{
	uint32_t ret;
	FNV1A32(ret, data, size);
	return ret;
}
#endif	// __BORLANDC__

#else	// _M_IX86

#ifdef __BORLANDC__
#pragma warn -8070
#endif

__declspec(naked) uint32_t __msfastcall fnv1a32(const void *data, size_t size)
{
	__asm
	{
		#define basis 0x811C9DC5
		#define prime 0x01000193
		#define data  ecx
		#define size  edx
		#define hash  eax
		#define r1    ebx
		#define r2    esi

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
		movzx   r1, byte ptr [data]     // 00000010 _ 0F B6. 19
		inc     data                    // 00000013 _ 41
		xor     hash, r1                // 00000014 _ 31. D8
		mov     r1, hash                // 00000016 _ 89. C3
		lea     hash, [hash + hash * 2] // 00000018 _ 8D. 04 40
		shl     r1, 4                   // 0000001B _ C1. E3, 04
		mov     r2, hash                // 0000001E _ 89. C6
		shl     r2, 7                   // 00000020 _ C1. E6, 07
		add     hash, r1                // 00000023 _ 01. D8
		shl     r1, 24 - 4              // 00000025 _ C1. E3, 14
		add     hash, r2                // 00000028 _ 01. F0
		add     hash, r1                // 0000002A _ 01. D8
		dec     size                    // 0000002C _ 4A
		jnz     L1                      // 0000002D _ 75, E1
		pop     esi                     // 0000002F _ 5E
		pop     ebx                     // 00000030 _ 5B
		ret                             // 00000031 _ C3

		#undef basis
		#undef prime
		#undef data
		#undef size
		#undef hash
		#undef r1
		#undef r2
	}
}
#endif	// _M_IX86

#ifdef __BORLANDC__
uint64_t __msreturn __fastcall fnv1a64(const void *data, size_t size)
{
	uint64_t ret;
	FNV1A64(ret, data, size);
	return ret;
}
#endif	// __BORLANDC__
