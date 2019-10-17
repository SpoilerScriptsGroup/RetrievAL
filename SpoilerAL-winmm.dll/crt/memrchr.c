#include <memory.h>

#ifndef _M_IX86
void * __cdecl _memrchr(const void *buffer, int c, size_t count)
{
	while (count--)
		if (((char *)buffer)[count] == (char)c)
			return = (char *)buffer + count;
	return NULL;
}
#else
static void * __cdecl memrchrSSE2(const void *buffer, int c, size_t count);
static void * __cdecl memrchr386(const void *buffer, int c, size_t count);
static void * __cdecl memrchrCPUDispatch(const void *buffer, int c, size_t count);

static void *(__cdecl * memrchrDispatch)(const void *buffer, int c, size_t count) = memrchrCPUDispatch;

__declspec(naked) void * __cdecl _memrchr(const void *buffer, int c, size_t count)
{
	__asm
	{
		jmp     dword ptr [memrchrDispatch]
	}
}

__declspec(naked) static void * __cdecl memrchrSSE2(const void *buffer, int c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     eax, dword ptr [count]                  // eax = count
		mov     ecx, dword ptr [buffer]                 // ecx = buffer
		test    eax, eax                                // check if count=0
		jz      count_equal_zero                        // if count=0, leave
		push    ebx                                     // preserve ebx
		lea     ebx, [ecx + eax - 1]                    // ebx = last byte of buffer
		push    esi                                     // preserve esi
		and     ebx, -16                                // ebx = last xmmword of buffer
		add     ecx, eax                                // ecx = end of buffer
		sub     ebx, eax                                // ebx = last xmmword of buffer - count
		movd    xmm1, dword ptr [c + 8]                 // xmm1 = search char
		punpcklbw xmm1, xmm1
		pshuflw xmm1, xmm1, 0
		movlhps xmm1, xmm1
		and     ecx, 15
		jz      loop_begin
		movdqa  xmm0, xmmword ptr [ebx + eax]
		pcmpeqb xmm0, xmm1
		pmovmskb edx, xmm0
		mov     esi, 7FFFH
		xor     ecx, 15
		shr     esi, cl
		and     edx, esi
		jnz     found
		sub     ebx, ecx
		xor     ecx, 15
		sub     eax, ecx
		jb      retnull
		dec     ebx

		align   16
	loop_begin:
		movdqa  xmm0, xmmword ptr [ebx + eax]
		pcmpeqb xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jnz     has_chr
		sub     eax, 16
		jae     loop_begin
	retnull:
		xor     eax, eax
		pop     esi                                     // restore esi
		pop     ebx                                     // restore ebx
	count_equal_zero:
		ret

		align   16
	has_chr:
		cmp     eax, 16
		jae     found
		mov     ecx, ebx
		or      esi, -1
		and     ecx, 15
		shl     esi, cl
		and     edx, esi
		jz      retnull
	found:
		bsr     edx, edx
		add     eax, ebx
		pop     esi                                     // restore esi
		pop     ebx                                     // restore ebx
		add     eax, edx
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static void * __cdecl memrchr386(const void *buffer, int c, size_t count)
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
		or      eax, ebx                                // u eax = 0/0/c/c
		push    edi                                     // v preserve edi
		mov     ebx, eax                                // u ebx = 0/0/c/c
		add     ecx, edx                                // v ecx = end of buffer
		shl     eax, 16                                 // u eax = c/c/0/0
		sub     ecx, 4                                  // v ecx = end of buffer - 4
		or      ebx, eax                                // u ebx = all 4 bytes = [search char]
		mov     eax, ecx                                // v eax = end of buffer - 4
		and     ecx, 3
		jz      loop_entry
		dec     ecx
		jz      modulo1
		dec     ecx
		jz      modulo2
		mov     cl, byte ptr [eax + 3]
		dec     eax
		cmp     cl, bl
		je      found
		dec     edx                                     // counter--
		jz      retnull
	modulo2:
		mov     cl, byte ptr [eax + 3]
		dec     eax
		cmp     cl, bl
		je      found
		dec     edx                                     // counter--
		jz      retnull
	modulo1:
		mov     cl, byte ptr [eax + 3]
		dec     eax
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
		mov     esi, dword ptr [eax]                    // read 4 bytes
		sub     eax, 4
		mov     ecx, esi
		xor     esi, ebx                                // ebx is byte\byte\byte\byte
		lea     edi, [esi + 7EFEFEFFH]
		xor     esi, -1
		xor     esi, edi
		and     esi, 81010100H
		jz      loop_begin
		and     esi, 01010100H
		jnz     has_chr
		test    edi, edi
		js      loop_begin
	byte_3:
		add     eax, 7
		jmp     epilogue

		align   16
	has_chr:
		bswap   ecx
		cmp     cl, bl
		je      byte_3
		cmp     ch, bl
		je      byte_2
		shr     ecx, 16
		cmp     cl, bl
		je      byte_1
		cmp     edx, 3
		jbe     retnull
	found:
		add     eax, 4
		jmp     epilogue

	byte_2:
		dec     edx
		jz      retnull
		add     eax, 6
		jmp     epilogue

	byte_1:
		cmp     edx, 2
		jbe     retnull
		add     eax, 5
		jmp     epilogue

		align   16
	retnull:
		xor     eax, eax
	epilogue:
		pop     edi                                     // restore edi
		pop     esi                                     // restore esi
		pop     ebx                                     // restore ebx
	count_equal_zero:
		ret                                             // __cdecl return

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static void * __cdecl memrchrCPUDispatch(const void *buffer, int c, size_t count)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [memrchrDispatch], offset memrchr386
		jmp     memrchr386
	L1:
		mov     dword ptr [memrchrDispatch], offset memrchrSSE2
		jmp     memrchrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
