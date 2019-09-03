#include <stddef.h>

#pragma function(strlen)

extern int __cdecl InstructionSet();

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

// SSE2 version
__declspec(naked) static size_t __cdecl strlenSSE2(const char *string)
{
	__asm
	{
		mov      eax,  dword ptr [esp + 4]                  // get pointer to string
		mov      ecx,  eax                                  // copy pointer
		pxor     xmm0, xmm0                                 // set to zero
		and      ecx,  0FH                                  // lower 4 bits indicate misalignment
		and      eax,  -10H                                 // align pointer by 16
		movdqa   xmm1, xmmword ptr [eax]                    // read from nearest preceding boundary
		pcmpeqb  xmm1, xmm0                                 // compare 16 bytes with zero
		pmovmskb edx,  xmm1                                 // get one bit for each byte result
		shr      edx,  cl                                   // shift out false bits
		shl      edx,  cl                                   // shift back again
		bsf      edx,  edx                                  // find first 1-bit
		jnz      A200                                       // found

		// Main loop, search 16 bytes at a time
	A100:
		add      eax,  10H                                  // increment pointer by 16
		movdqa   xmm1, xmmword ptr [eax]                    // read 16 bytes aligned
		pcmpeqb  xmm1, xmm0                                 // compare 16 bytes with zero
		pmovmskb edx,  xmm1                                 // get one bit for each byte result
		bsf      edx,  edx                                  // find first 1-bit
		// (moving the bsf out of the loop and using test here would be faster for long strings on old processors,
		//  but we are assuming that most strings are short, and newer processors have higher priority)
		jz       A100                                       // loop if not found

	A200:
		// Zero-byte found. Compute string length
		sub      eax,  dword ptr [esp + 4]                  // subtract start address
		add      eax,  edx                                  // add byte index
		ret
	}
}

// 80386 version
__declspec(naked) static size_t __cdecl strlen386(const char *string)
{
#if 0
	__asm
	{
		push    ebx
		mov     ecx, dword ptr [esp + 8]                    // get pointer to string
		mov     eax, ecx                                    // copy pointer
		and     ecx, 3                                      // lower 2 bits of address, check alignment
		jz      L2                                          // string is aligned by 4. Go to loop
		and     eax, -4                                     // align pointer by 4
		mov     ebx, dword ptr [eax]                        // read from nearest preceding boundary
		shl     ecx, 3                                      // mul by 8 = displacement in bits
		mov     edx, -1
		shl     edx, cl                                     // make byte mask
		not     edx                                         // mask = 0FFH for false bytes
		or      ebx, edx                                    // mask out false bytes

		// check first four bytes for zero
		lea     ecx, [ebx - 01010101H]                      // subtract 1 from each byte
		not     ebx                                         // invert all bytes
		and     ecx, ebx                                    // and these two
		and     ecx, 80808080H                              // test all sign bits
		jnz     L3                                          // zero-byte found

		// Main loop, read 4 bytes aligned
	L1:
		add     eax, 4                                      // increment pointer by 4
	L2:
		mov     ebx, dword ptr [eax]                        // read 4 bytes of string
		lea     ecx, [ebx - 01010101H]                      // subtract 1 from each byte
		not     ebx                                         // invert all bytes
		and     ecx, ebx                                    // and these two
		and     ecx, 80808080H                              // test all sign bits
		jz      L1                                          // no zero bytes, continue loop

	L3:
		bsf     ecx, ecx                                    // find right-most 1-bit
		shr     ecx, 3                                      // divide by 8 = byte index
		sub     eax, dword ptr [esp + 8]                    // subtract start address
		add     eax, ecx                                    // add index to byte
		pop     ebx
		ret
	}
#else
	__asm
	{
		#define string (esp + 4)

		mov     ecx, dword ptr [string]                     // get pointer to string
		mov     eax, ecx                                    // copy pointer
		and     ecx, 3                                      // lower 2 bits of address, check alignment
		jz      L3                                          // string is aligned by 4. Go to loop
		dec     ecx
		jz      L2
		dec     ecx
		jz      L1

		// unaligned (3 == string % 4)
		mov     cl, byte ptr [eax]                          // read 1 bytes of string
		inc     eax                                         // increment pointer by 4 (align pointer by 4)
		test    cl, cl                                      // compare to zero
		jnz     L3                                          // no zero bytes, enter loop
		xor     eax, eax                                    // return null
		ret

		// unaligned (2 == string % 4)
		align   16
	L1:
		mov     ecx, dword ptr [eax - 2]                    // read from nearest preceding boundary
		add     eax, 2                                      // increment pointer by 4 (align pointer by 4)
		lea     edx, [ecx - 01010000H]                      // subtract 1 from 2 byte
		xor     ecx, -1                                     // invert all bytes
		and     edx, 80800000H                              // mask 2 sign bits
		and     ecx, edx                                    // and these two
		jz      L3                                          // no zero bytes, enter loop
		jmp     L5                                          // zero-byte found

		// unaligned (1 == string % 4)
		align   16
	L2:
		mov     ecx, dword ptr [eax - 1]                    // read from nearest preceding boundary
		add     eax, 3                                      // increment pointer by 4 (align pointer by 4)
		lea     edx, [ecx - 01010100H]                      // subtract 1 from 3 byte
		xor     ecx, -1                                     // invert all bytes
		and     edx, 80808000H                              // mask 3 sign bits
		and     ecx, edx                                    // and these two
		jnz     L4                                          // zero-byte found

		// Main loop, read 4 bytes aligned
		align   16
	L3:
		mov     ecx, dword ptr [eax]                        // read 4 bytes of string
		add     eax, 4                                      // increment pointer by 4
		lea     edx, [ecx - 01010101H]                      // subtract 1 from each byte
		xor     ecx, -1                                     // invert all bytes
		and     edx, 80808080H                              // mask all sign bits
		and     ecx, edx                                    // and these two
		jz      L3                                          // no zero bytes, continue loop

		// subtract start address
	L4:
		test    cx, cx
		jz      L5
		sub     eax, 3
		jmp     L6
	L5:
		shr     ecx, 16
		dec     eax
	L6:
		shr     ecx, 8
		mov     edx, dword ptr [string]
		sbb     eax, edx
		ret

		#undef string
	}
#endif
}

// CPU dispatching for strlen. This is executed only once
__declspec(naked) static size_t __cdecl strlenCPUDispatch(const char *string)
{
	__asm
	{
		pushad
		call    InstructionSet
		// Point to generic version of strlen
		mov     dword ptr [strlenDispatch], offset strlen386
		cmp     eax, 4                                      // check SSE2
		jb      M100
		// SSE2 supported
		// Point to SSE2 version of strlen
		mov     dword ptr [strlenDispatch], offset strlenSSE2

	M100:
		popad
		// Continue in appropriate version of strlen
		jmp     dword ptr [strlenDispatch]
	}
}
