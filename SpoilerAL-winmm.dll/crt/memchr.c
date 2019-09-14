#include <memory.h>

#pragma function(memchr)

#ifndef _M_IX86
void * __cdecl memchr(const void *buf, int c, size_t count)
{
	while (count--)
		if (*(((char *)buf)++) == (char)c)
			return (char *)buf - 1;
	return NULL;
}
#else
static void * __cdecl memchrSSE2(const void *buf, int c, size_t count);
static void * __cdecl memchr386(const void *buf, int c, size_t count);
static void * __cdecl memchrCPUDispatch(const void *buf, int c, size_t count);

static void *(__cdecl * memchrDispatch)(const void *buf, int c, size_t count) = memchrCPUDispatch;

__declspec(naked) void * __cdecl memchr(const void *buf, int c, size_t count)
{
	__asm
	{
		jmp     dword ptr [memchrDispatch]
	}
}

__declspec(naked) static void * __cdecl memchrSSE2(const void *buf, int c, size_t count)
{
	__asm
	{
		#define buf   (esp + 4)
		#define c     (esp + 8)
		#define count (esp + 12)

		mov     eax, dword ptr [count]                  // eax = count
		mov     ecx, dword ptr [buf]                    // ecx = buffer
		push    ebx                                     // preserve ebx
		lea     ebx, [ecx + eax]                        // ebx = end of buffer
		sub     eax, 1                                  // check if count=0
		jb      retnull                                 // if count=0, leave
		mov     edx, -16
		xor     eax, -1                                 // eax = -count
		and     edx, ecx
		align   16                                      // padding 5 byte
		movd    xmm1, dword ptr [c + 4]                 // xmm1 = search char
		punpcklbw xmm1, xmm1
		pshuflw xmm1, xmm1, 0
		movlhps xmm1, xmm1
		and     ecx, 15
		jz      main_loop
		movdqa  xmm0, xmmword ptr [edx]
		pcmpeqb xmm0, xmm1
		pmovmskb edx, xmm0
		shr     edx, cl
		test    edx, edx
		jnz     found
		sub     ecx, 16
		sub     eax, ecx
		jae     retnull

		align   16                                      // already aligned
	main_loop:
		movdqa  xmm0, xmmword ptr [ebx + eax]
		pcmpeqb xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jnz     found
		add     eax, 16
		jnc     main_loop
	retnull:
		xor     eax, eax
		pop     ebx                                     // restore ebx
		ret

		align   16
	found:
		bsf     edx, edx
		add     eax, edx
		jc      retnull
		add     eax, ebx
		pop     ebx                                     // restore ebx
		ret

		#undef buf
		#undef c
		#undef count
	}
}

__declspec(naked) static void * __cdecl memchr386(const void *buf, int c, size_t count)
{
	__asm
	{
		#define buf   (esp + 4)
		#define c     (esp + 8)
		#define count (esp + 12)

		mov     edx, dword ptr [count]                  // edx = count
		mov     eax, dword ptr [buf]                    // eax = buffer
		test    edx, edx                                // check if count=0
		jz      retnull_pop0                            // if count=0, leave
		push    ebx                                     // preserve ebx
		xor     ebx, ebx
		mov     bl, byte ptr [c + 4]                    // bl = search char
		jmp     misaligned_loop_entry

		align   16
	misaligned_loop:                                    // simple byte loop until buffer is aligned
		mov     cl, byte ptr [eax]
		inc     eax
		cmp     cl, bl
		je      found
		dec     edx                                     // counter--
		jz      retnull_pop1
	misaligned_loop_entry:
		test    eax, 3                                  // already aligned ?
		jnz     misaligned_loop

		cmp     edx, 4
		jae     main_loop_start

		align   16
	tail_loop:                                          // 0 < edx < 4
		mov     cl, byte ptr [eax]
		inc     eax
		cmp     cl, bl
		je      found
		dec     edx
		jnz     tail_loop
	retnull_pop1:
		pop     ebx                                     // restore ebx
	retnull_pop0:
		xor     eax, eax
		ret                                             // __cdecl return

		align   16
	found:
		dec     eax
		pop     ebx                                     // restore ebx
		ret                                             // __cdecl return

		align   16
	main_loop_start:
		                                                // set all 4 bytes of ebx to [value]
		push    esi                                     // u  preserve esi
		mov     ecx, ebx                                // v  ecx=0/0/0/c
		shl     ebx, 8                                  // np ebx=0/0/c/0
		mov     esi, ebx                                // u  esi=0/0/c/0
		or      ebx, ecx                                // v  ebx=0/0/c/c
		shl     ebx, 16                                 // u  ebx=c/c/0/0
		or      ecx, esi                                // v  ecx=0/0/c/c
		or      ebx, ecx                                // u  ebx = all 4 bytes = [search char]
		jmp     main_loop_entry

		align   16
	main_loop:
		sub     edx, 4
		jbe     retnull
	main_loop_entry:
		mov     ecx, dword ptr [eax]                    // read 4 bytes
		add     eax, 4
		xor     ecx, ebx                                // ebx is byte\byte\byte\byte
		mov     esi, 7EFEFEFFH
		add     esi, ecx
		xor     ecx, -1
		xor     ecx, esi
		and     ecx, 81010100H
		jz      main_loop
		and     ecx, 01010100H
		jnz     byte_0_to_2
		test    esi, esi
		js      main_loop
		cmp     edx, 3
		jbe     retnull
		dec     eax
		jmp     epilogue

		align   16
	byte_0_to_2:
		test    ch, ch
		jnz     byte_0
		and     ecx, 00010000H
		jnz     byte_1
		cmp     edx, 2
		jbe     retnull
		sub     eax, 2
		jmp     epilogue

	byte_0:
		sub     eax, 4
		jmp     epilogue

	byte_1:
		dec     edx
		jz      retnull
		sub     eax, 3
		jmp     epilogue

		align   16
	retnull:
		xor     eax, eax
	epilogue:
		pop     esi                                     // restore esi
		pop     ebx                                     // restore ebx
		ret                                             // __cdecl return

		#undef buf
		#undef c
		#undef count
	}
}

__declspec(naked) static void * __cdecl memchrCPUDispatch(const void *buf, int c, size_t count)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [memchrDispatch], offset memchr386
		jmp     memchr386
	L1:
		mov     dword ptr [memchrDispatch], offset memchrSSE2
		jmp     memchrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
