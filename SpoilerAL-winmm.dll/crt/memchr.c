#include <memory.h>

#pragma function(memchr)

#ifndef _M_IX86
void * __cdecl memchr(const void *buffer, int c, size_t count)
{
	while (count--)
		if (*(((char *)buffer)++) == (char)c)
			return (char *)buffer - 1;
	return NULL;
}
#else
static void * __cdecl memchrSSE2(const void *buffer, int c, size_t count);
static void * __cdecl memchr386(const void *buffer, int c, size_t count);
static void * __cdecl memchrCPUDispatch(const void *buffer, int c, size_t count);

static void *(__cdecl * memchrDispatch)(const void *buffer, int c, size_t count) = memchrCPUDispatch;

__declspec(naked) void * __cdecl memchr(const void *buffer, int c, size_t count)
{
	__asm
	{
		jmp     dword ptr [memchrDispatch]
	}
}

__declspec(naked) static void * __cdecl memchrSSE2(const void *buffer, int c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     eax, dword ptr [count]                  // eax = count
		mov     ecx, dword ptr [buffer]                 // ecx = buffer
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
		jz      loop_begin
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
	loop_begin:
		movdqa  xmm0, xmmword ptr [ebx + eax]
		pcmpeqb xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jnz     found
		add     eax, 16
		jnc     loop_begin
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

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static void * __cdecl memchr386(const void *buffer, int c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                  // edx = count
		xor     eax, eax
		test    edx, edx                                // check if count=0
		jz      count_equal_zero                        // if count=0, leave
		mov     al, byte ptr [c]                        // u al = search char
		mov     ecx, dword ptr [buffer]                 // v ecx = buffer
		                                                // set all 4 bytes of ebx to [value]
		push    ebx                                     // u preserve ebx
		mov     ebx, eax                                // v ebx = 0/0/0/c
		shl     eax, 8                                  // u eax = 0/0/c/0
		push    esi                                     // v preserve esi
		mov     esi, eax                                // u esi = 0/0/c/0
		or      eax, ebx                                // v eax = 0/0/c/c
		shl     eax, 16                                 // u eax = c/c/0/0
		or      ebx, esi                                // v ebx = 0/0/c/c
		or      ebx, eax                                // u ebx = all 4 bytes = [search char]
		mov     eax, ecx                                // v eax = buffer
		and     ecx, 3
		jz      loop_entry
		xor     ecx, 3
		jz      modulo3
		dec     ecx
		jz      modulo2
		mov     cl, byte ptr [eax]
		inc     eax
		cmp     cl, bl
		je      found
		dec     edx                                     // counter--
		jz      retnull
	modulo2:
		mov     cl, byte ptr [eax]
		inc     eax
		cmp     cl, bl
		je      found
		dec     edx                                     // counter--
		jz      retnull
	modulo3:
		mov     cl, byte ptr [eax]
		inc     eax
		cmp     cl, bl
		je      found
		dec     edx                                     // counter--
		jnz     loop_entry
		jmp     retnull

		align   16
	loop_begin:
		sub     edx, 4
		jbe     retnull
	loop_entry:
		mov     ecx, dword ptr [eax]                    // read 4 bytes
		add     eax, 4
		xor     ecx, ebx                                // ebx is byte\byte\byte\byte
		mov     esi, 7EFEFEFFH
		add     esi, ecx
		xor     ecx, -1
		xor     ecx, esi
		and     ecx, 81010100H
		jz      loop_begin
		and     ecx, 01010100H
		jnz     byte_0_to_2
		test    esi, esi
		js      loop_begin
		cmp     edx, 3
		jbe     retnull
	found:
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
	count_equal_zero:
		ret                                             // __cdecl return

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static void * __cdecl memchrCPUDispatch(const void *buffer, int c, size_t count)
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
