#include <memory.h>
#ifndef _M_IX86

void * __cdecl _memrchr(const void *buffer, int c, size_t count)
{
	while (count--)
		if (((char *)buffer)[count] == (char)c)
			return (char *)buffer + count;
	return NULL;
}
#else
#include <xmmintrin.h>

void * __cdecl memrchrSSE2(const void *buffer, int c, size_t count);
void * __vectorcall internal_memrchrSSE2(const void *buffer, __m128 c, size_t count);
void * __cdecl memrchr386(const void *buffer, int c, size_t count);
void * __fastcall internal_memrchr386(const void *buffer, unsigned long c, size_t count);
static void * __cdecl memrchrCPUDispatch(const void *buffer, int c, size_t count);

static void *(__cdecl *memrchrDispatch)(const void *buffer, int c, size_t count) = memrchrCPUDispatch;

__declspec(naked) void * __cdecl _memrchr(const void *buffer, int c, size_t count)
{
	__asm
	{
		jmp     dword ptr [memrchrDispatch]
	}
}

__declspec(naked) void * __cdecl memrchrSSE2(const void *buffer, int c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                      // edx = count
		mov     ecx, dword ptr [buffer]                     // ecx = buffer
		test    edx, edx                                    // check if count == 0
		jz      retnull                                     // if count == 0, leave
		movd    xmm0, dword ptr [c]                         // xmm0 = search char
		punpcklbw xmm0, xmm0
		pshuflw xmm0, xmm0, 0
		movlhps xmm0, xmm0
		jmp     internal_memrchrSSE2

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) void * __vectorcall internal_memrchrSSE2(const void *buffer, __m128 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      xmm0
		#define count  edx

		push    esi                                         // preserve esi
		lea     esi, [ecx + edx - 1]                        // esi = last byte of buffer
		push    edi                                         // preserve edi
		and     esi, -16                                    // esi = last xmmword of buffer
		add     ecx, edx                                    // ecx = end of buffer
		sub     esi, edx                                    // esi = last xmmword of buffer - count
		and     ecx, 15
		jz      loop_begin
		movdqa  xmm1, xmmword ptr [esi + edx]
		pcmpeqb xmm1, xmm0
		pmovmskb eax, xmm1
		mov     edi, 7FFFH
		xor     ecx, 15
		shr     edi, cl
		and     eax, edi
		jnz     has_char_at_last_xmmword
		sub     esi, ecx
		xor     ecx, 15
		sub     edx, ecx
		jbe     retnull
		dec     esi

		align   16
	loop_begin:
		movdqa  xmm1, xmmword ptr [esi + edx]
		pcmpeqb xmm1, xmm0
		pmovmskb eax, xmm1
		test    eax, eax
		jnz     has_char
		sub     edx, 16
		ja      loop_begin
	retnull:
		xor     eax, eax
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		ret

		align   16
	has_char_at_last_xmmword:
		xor     ecx, 15
		or      edi, -1
		sub     ecx, edx
		ja      mask_first_xmmword
		jmp     found

		align   16
	has_char:
		cmp     edx, 16
		jae     found
		mov     ecx, esi
		or      edi, -1
		and     ecx, 15
		jz      found
	mask_first_xmmword:
		shl     edi, cl
		and     eax, edi
		jz      retnull
	found:
		bsr     eax, eax
		add     edx, esi
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		add     eax, edx
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) void * __cdecl memrchr386(const void *buffer, int c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                      // edx = count
		xor     eax, eax
		test    edx, edx                                    // check if count == 0
		jz      retnull                                     // if count == 0, leave
		                                                    // set all 4 bytes of edx to [value]
		mov     al, byte ptr [c]                            // al = search char
		mov     ecx, dword ptr [buffer]                     // ecx = buffer
		push    edx
		mov     edx, eax                                    // edx = 0/0/0/c
		shl     eax, 8                                      // eax = 0/0/c/0
		or      eax, edx                                    // eax = 0/0/c/c
		mov     edx, eax                                    // edx = 0/0/c/c
		shl     eax, 16                                     // eax = c/c/0/0
		or      edx, eax                                    // edx = all 4 bytes = [search char]
		call    internal_memrchr386
	retnull:
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) void * __fastcall internal_memrchr386(const void *buffer, unsigned long c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      edx
		#define count  (esp + 4)

		push    ebx                                         // preserve ebx
		push    esi                                         // preserve esi
		push    edi                                         // preserve edi
		mov     eax, dword ptr [count + 12]                 // eax = count
		mov     esi, ecx                                    // esi = buffer
		add     ecx, eax                                    // ecx = end of buffer
		dec     esi                                         // esi = buffer - 1
		and     ecx, 3
		jz      loop_entry
		dec     ecx
		jz      modulo1
		dec     ecx
		jz      modulo2
		cmp     byte ptr [esi + eax], dl
		je      found
		dec     eax
		jz      epilogue
	modulo2:
		cmp     byte ptr [esi + eax], dl
		je      found
		dec     eax
		jz      epilogue
	modulo1:
		cmp     byte ptr [esi + eax], dl
		je      found
		dec     eax
		jnz     loop_entry
		jmp     epilogue

		align   16
	loop_begin:
		sub     eax, 4
		jbe     retnull
	loop_entry:
		mov     ebx, dword ptr [esi + eax - 3]              // read 4 bytes
		mov     edi, 7EFEFEFFH
		mov     ecx, ebx
		xor     ebx, edx                                    // edx is byte\byte\byte\byte
		add     edi, ebx
		xor     ebx, -1
		xor     ebx, edi
		and     ebx, 81010100H
		jz      loop_begin
		and     ebx, 01010100H
		jnz     has_char
		test    edi, edi
		js      loop_begin
		jmp     found

		align   16
	has_char:
		bswap   ecx
		cmp     cl, dl
		je      found
		cmp     ch, dl
		je      byte_2
		shr     ecx, 16
		cmp     cl, dl
		je      byte_1
		sub     eax, 3
		ja      found
	retnull:
		xor     eax, eax
		jmp     epilogue

		align   16
	byte_1:
		dec     eax
		jz      epilogue
	byte_2:
		dec     eax
		jz      epilogue
	found:
		add     eax, esi
	epilogue:
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		pop     ebx                                         // restore ebx
		ret     4

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
