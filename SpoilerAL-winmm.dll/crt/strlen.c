#include <stddef.h>

#pragma function(strlen)

#ifndef _M_IX86
size_t __cdecl strlen(const char *string)
{
	size_t length;

	length = -1;
	string++;
	while (string[length++]);
	return length;
}
#else
extern const char ymmconst_maskbit[64];
#define maskbit (ymmconst_maskbit + 16)

static size_t __cdecl strlenAVX2(const char *string);
static size_t __cdecl strlenSSE42(const char *string);
static size_t __cdecl strlenSSE2(const char *string);
static size_t __cdecl strlen386(const char *string);
static size_t __cdecl strlenCPUDispatch(const char *string);

static size_t(__cdecl * strlenDispatch)(const char *string) = strlenCPUDispatch;

__declspec(naked) size_t __cdecl strlen(const char *string)
{
	__asm
	{
		jmp     dword ptr [strlenDispatch]                  // Go to appropriate version, depending on instruction set
	}
}

// AVX2 version
__declspec(naked) static size_t __cdecl strlenAVX2(const char *string)
{
	__asm
	{
		#define string (esp + 4)

		mov     ecx, dword ptr [string]                     // get pointer to string
		vpxor   ymm1, ymm1, ymm1                            // set to zero
		or      edx, -1                                     // fill mask bits
		mov     eax, ecx                                    // copy pointer
		and     ecx, 31                                     // get lower 8 bits indicate misalignment
		jz      loop_entry                                  // jump if aligned
		shl     edx, cl                                     // shift out false bits
		sub     eax, ecx                                    // align pointer by 32
		jmp     loop_entry

		// Main loop, search 32 bytes at a time
		align   16
	loop_begin:
		add     eax, 32                                     // increment pointer by 32
		or      edx, -1                                     // fill mask bits
	loop_entry:
		vpcmpeqb ymm0, ymm1, ymmword ptr [eax]              // compare 32 bytes with zero
		vpmovmskb ecx, ymm0                                 // get one bit for each byte result
		and     ecx, edx                                    // mask result
		jz      loop_begin                                  // loop if not found

		// Zero-byte found. Compute string length
		bsf     ecx, ecx                                    // get first bit index of result
		mov     edx, dword ptr [string]                     // get pointer to string
		add     eax, ecx                                    // add byte index
		sub     eax, edx                                    // subtract start address
		vzeroupper
		ret

		#undef string
	}
}

// SSE4.2 version
__declspec(naked) static size_t __cdecl strlenSSE42(const char *string)
{
	__asm
	{
		#define string (esp + 4)

		mov     ecx, dword ptr [string]                     // get pointer to string
		pxor    xmm0, xmm0                                  // set to zero
		mov     eax, -16
		lea     edx, [ecx + 16]
		and     ecx, 15
		jz      loop_begin
		sub     eax, ecx
		movdqa  xmm1, xmmword ptr [edx + eax]               // read 16 bytes from string
		movdqu  xmm2, xmmword ptr [maskbit + eax + 32]      // load the non target bits mask
		por     xmm1, xmm2                                  // fill the non target bits to 1
		pcmpistri xmm0, xmm1, 00001000B                     // find null. returns index in ecx
		jmp     loop_increment

		align   16
	loop_begin:
		pcmpistri xmm0, xmmword ptr [edx + eax], 00001000B  // find null. returns index in ecx
	loop_increment:
		lea     eax, [eax + 16]                             // increment pointer by 16
		jnz     loop_begin                                  // next 16 bytes

		add     eax, ecx
		ret

		#undef string
	}
}

// SSE2 version
__declspec(naked) static size_t __cdecl strlenSSE2(const char *string)
{
	__asm
	{
		#define string (esp + 4)

		mov     ecx, dword ptr [string]                     // get pointer to string
		pxor    xmm1, xmm1                                  // set to zero
		or      edx, -1                                     // fill mask bits
		mov     eax, ecx                                    // copy pointer
		and     ecx, 15                                     // get lower 4 bits indicate misalignment
		jz      loop_entry                                  // jump if aligned
		shl     edx, cl                                     // shift out false bits
		sub     eax, ecx                                    // align pointer by 16
		jmp     loop_entry

		// Main loop, search 16 bytes at a time
		align   16
	loop_begin:
		add     eax, 16                                     // increment pointer by 16
		or      edx, -1                                     // fill mask bits
	loop_entry:
		movdqa  xmm0, xmmword ptr [eax]                     // read 16 bytes aligned
		pcmpeqb xmm0, xmm1                                  // compare 16 bytes with zero
		pmovmskb ecx, xmm0                                  // get one bit for each byte result
		and     ecx, edx                                    // mask result
		jz      loop_begin                                  // loop if not found

		// Zero-byte found. Compute string length
		bsf     ecx, ecx                                    // get first bit index of result
		mov     edx, dword ptr [string]                     // get pointer to string
		add     eax, ecx                                    // add byte index
		sub     eax, edx                                    // subtract start address
		ret

		#undef string
	}
}

// 80386 version
__declspec(naked) static size_t __cdecl strlen386(const char *string)
{
	__asm
	{
		#define string (esp + 4)

		mov     ecx, dword ptr [string]                     // get pointer to string
		mov     eax, ecx                                    // copy pointer
		and     ecx, 3                                      // lower 2 bits of address, check alignment
		jz      loop_begin                                  // string is aligned by 4. Go to loop
		dec     ecx
		jz      modulo1
		dec     ecx
		jz      modulo2

		// unaligned (3 == string % 4)
		mov     cl, byte ptr [eax]                          // read 1 bytes of string
		inc     eax                                         // add pointer by 1 (align pointer by 4)
		test    cl, cl                                      // compare to zero
		jnz     loop_begin                                  // no zero bytes, enter loop
		xor     eax, eax                                    // return 0
		ret

		// unaligned (2 == string % 4)
		align   16
	modulo2:
		mov     ecx, dword ptr [eax - 2]                    // read from nearest preceding boundary
		add     eax, 2                                      // add pointer by 2 (align pointer by 4)
		lea     edx, [ecx - 01010000H]                      // subtract 1 from upper 2 byte
		xor     ecx, -1                                     // invert all bytes
		and     edx, 80800000H                              // mask two sign bits
		and     ecx, edx                                    // and these two
		jz      loop_begin                                  // no zero bytes, enter loop
		jmp     found_at_high_word                          // zero-byte found

		// unaligned (1 == string % 4)
		align   16
	modulo1:
		mov     ecx, dword ptr [eax - 1]                    // read from nearest preceding boundary
		add     eax, 3                                      // add pointer by 3 (align pointer by 4)
		lea     edx, [ecx - 01010100H]                      // subtract 1 from upper 3 byte
		xor     ecx, -1                                     // invert all bytes
		and     edx, 80808000H                              // mask three sign bits
		and     ecx, edx                                    // and these two
		jnz     found                                       // zero-byte found

		// Main loop, read 4 bytes aligned
		align   16
	loop_begin:
		mov     ecx, dword ptr [eax]                        // read 4 bytes of string
		add     eax, 4                                      // increment pointer by 4
		lea     edx, [ecx - 01010101H]                      // subtract 1 from each byte
		xor     ecx, -1                                     // invert all bytes
		and     edx, 80808080H                              // mask all sign bits
		and     ecx, edx                                    // and these two
		jz      loop_begin                                  // no zero bytes, continue loop

		// subtract start address
	found:
		test    cx, cx
		jz      found_at_high_word
		shl     ecx, 24
		mov     edx, dword ptr [string]
		sub     eax, 3
		add     ecx, ecx
		sbb     eax, edx
		ret

		align   16
	found_at_high_word:
		shl     ecx, 8
		mov     edx, dword ptr [string]
		dec     eax
		add     ecx, ecx
		sbb     eax, edx
		ret

		#undef string
	}
}

__declspec(naked) static size_t __cdecl strlenCPUDispatch(const char *string)
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	__asm
	{
		mov     eax, dword ptr [__isa_available]
		cmp     eax, __ISA_AVAILABLE_AVX2
		jb      L1
		mov     dword ptr [strlenDispatch], offset strlenAVX2
		jmp     strlenAVX2

	L1:
		cmp     eax, __ISA_AVAILABLE_SSE2
		jbe     L2
		mov     dword ptr [strlenDispatch], offset strlenSSE42
		jmp     strlenSSE42

	L2:
		mov     dword ptr [strlenDispatch], offset strlenSSE2
		jb      L3
		jmp     strlenSSE2

	L3:
		mov     dword ptr [strlenDispatch], offset strlen386
		jmp     strlen386
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
	#undef __ISA_AVAILABLE_AVX
	#undef __ISA_AVAILABLE_ENFSTRG
	#undef __ISA_AVAILABLE_AVX2
}
#endif
