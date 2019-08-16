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

		mov     eax, dword ptr [count]                      // eax = count
		mov     ecx, dword ptr [buf]                        // ecx = buffer
		test    eax, eax                                    // check if count=0
		jz      retnull                                     // if count=0, leave
		movd    xmm1, byte ptr [c]                          // xmm1 = search char
		punpcklbw xmm1, xmm1
		pshuflw xmm1, xmm1, 0
		movlhps xmm1, xmm1
		push    ebx                                         // preserve ebx
		lea     ebx, [ecx + eax]                            // ebx = end of buffer
		and     ecx, 15
		dec     eax
		mov     edx, -1
		add     eax, ecx
		shl     edx, cl                                     // edx = bit mask
		xor     eax, -1                                     // eax = -(count + (buffer % 16))
		movdqa  xmm0, xmmword ptr [ebx + eax]
		pcmpeqb xmm0, xmm1
		pmovmskb ecx, xmm0
		and     ecx, edx
		jnz     found

		align   16
	main_loop:
		add     eax, 16
		jc      not_found
		movdqa  xmm0, xmmword ptr [ebx + eax]
		pcmpeqb xmm0, xmm1
		pmovmskb ecx, xmm0
		test    ecx, ecx
		jz      main_loop
	found:
		bsf     ecx, ecx
		add     eax, ecx
		jc      not_found
		add     eax, ebx
		pop     ebx                                         // restore ebx
	retnull:
		ret

		align   16
	not_found:
		xor     eax, eax
		pop     ebx                                         // restore ebx
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

		mov     eax, dword ptr [count]                      // eax = count
		mov     edx, dword ptr [buf]                        // edx = buffer
		test    eax, eax                                    // check if count=0
		jz      retnull                                     // if count=0, leave
		push    ebx                                         // preserve ebx
		xor     ebx, ebx
		mov     bl, byte ptr [c + 4]                        // bl = search char
		jmp     misaligned_loop_entry

		align   16
	misaligned_loop:                                        // simple byte loop until buffer is aligned
		mov     cl, byte ptr [edx]
		inc     edx
		cmp     cl, bl
		je      found
		dec     eax                                         // counter--
		jz      retnull
	misaligned_loop_entry:
		test    edx, 3                                      // already aligned ?
		jnz     misaligned_loop

		cmp     eax, 4
		jae     main_loop_start

		align   16
	tail_loop:                                              // 0 < eax < 4
		mov     cl, byte ptr [edx]
		inc     edx
		cmp     cl, bl
		je      found
		dec     eax
		jnz     tail_loop
		pop     ebx                                         // restore ebx
	retnull:
		ret                                                 // __cdecl return

		align   16
	found:
		lea     eax, [edx - 1]
		pop     ebx                                         // restore ebx
		ret                                                 // __cdecl return

		align   16
	main_loop_start:
		                                                    // set all 4 bytes of ebx to [value]
		push    esi                                         // preserve esi
		mov     ecx, ebx                                    // ecx=0/0/0/char
		shl     ebx, 8                                      // ebx=0/0/char/0
		add     edx, eax
		or      ebx, ecx                                    // ebx=0/0/char/char
		xor     eax, -1
		mov     ecx, ebx                                    // ecx=0/0/char/char
		dec     eax
		shl     ebx, 16                                     // ebx=char/char/0/0
		or      ebx, ecx                                    // ebx = all 4 bytes = [search char]

		align   16
	main_loop:
		mov     ecx, dword ptr [edx + eax]                  // read 4 bytes
		xor     ecx, ebx                                    // ebx is byte\byte\byte\byte
		mov     esi, 7EFEFEFFH
		add     esi, ecx
		xor     ecx, -1
		xor     ecx, esi
		test    ecx, 81010100H
		jz      chr_is_not_found
		test    ecx, 01010100H
		jnz     byte_0_to_2
		test    esi, 80000000H
		jz      byte_3
	chr_is_not_found:
		add     eax, 4
		jnc     main_loop
		xor     eax, eax
		jmp     epilogue

		align   16
	byte_0_to_2:
		add     eax, edx
		test    ecx, 00000100H
		jnz     epilogue
		inc     eax
		test    ecx, 00010000H
		jnz     epilogue
		inc     eax
		jmp     epilogue

		align   16
	byte_3:
		lea     eax, [edx + eax + 3]
	epilogue:
		pop     esi                                         // restore esi
		pop     ebx                                         // restore ebx
		ret                                                 // __cdecl return

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
