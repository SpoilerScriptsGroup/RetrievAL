#pragma warning(disable:4164)
#pragma function(memchr)

#include <memory.h>
#ifndef _M_IX86

void * __cdecl memchr(const void *buffer, int c, size_t count)
{
	while (count--)
		if (*(((char *)buffer)++) == (char)c)
			return (char *)buffer - 1;
	return NULL;
}
#else
#include <xmmintrin.h>

void * __cdecl memchrSSE2(const void *buffer, int c, size_t count);
void * __vectorcall internal_memchrSSE2(const void *buffer, __m128 c, size_t count);
void * __cdecl memchr386(const void *buffer, int c, size_t count);
void * __fastcall internal_memchr386(const void *buffer, unsigned long c, size_t count);
#ifndef _DEBUG
static void * __cdecl memchrCPUDispatch(const void *buffer, int c, size_t count);

static void *(__cdecl *memchrDispatch)(const void *buffer, int c, size_t count) = memchrCPUDispatch;

__declspec(naked) void * __cdecl memchr(const void *buffer, int c, size_t count)
{
	__asm
	{
		jmp     dword ptr [memchrDispatch]
	}
}
#endif

__declspec(naked) void * __cdecl memchrSSE2(const void *buffer, int c, size_t count)
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
		jmp     internal_memchrSSE2

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) void * __vectorcall internal_memchrSSE2(const void *buffer, __m128 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      xmm0
		#define count  edx

		push    esi                                         // preserve esi
		lea     esi, [ecx + edx]                            // esi = end of buffer
		mov     eax, -16
		xor     edx, -1
		and     eax, ecx
		add     edx, 1                                      // edx = -count
		and     ecx, 15
		jz      loop_begin
		movdqa  xmm1, xmmword ptr [eax]
		pcmpeqb xmm1, xmm0
		pmovmskb eax, xmm1
		shr     eax, cl
		test    eax, eax
		jnz     found
		sub     ecx, 16
		nop
		sub     edx, ecx
		jae     retnull

		align   16                                          // already aligned
	loop_begin:
		movdqa  xmm1, xmmword ptr [esi + edx]
		pcmpeqb xmm1, xmm0
		pmovmskb eax, xmm1
		test    eax, eax
		jnz     found
		add     edx, 16
		jnc     loop_begin
	retnull:
		xor     eax, eax
		pop     esi                                         // restore esi
		ret

		align   16
	found:
		bsf     eax, eax
		add     eax, edx
		jc      retnull
		add     eax, esi
		pop     esi                                         // restore esi
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) void * __cdecl memchr386(const void *buffer, int c, size_t count)
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
		call    internal_memchr386
	retnull:
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) void * __fastcall internal_memchr386(const void *buffer, unsigned long c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      edx
		#define count  (esp + 4)

		push    esi                                         // preserve esi
		push    edi                                         // preserve edi
		mov     eax, dword ptr [count + 8]
		mov     esi, eax
		xor     eax, -1
		add     esi, ecx                                    // esi = end of buffer
		inc     eax                                         // eax = -count
		and     ecx, 3
		jz      loop_entry
		xor     ecx, 3
		jz      modulo3
		dec     ecx
		jz      modulo2
		cmp     byte ptr [esi + eax], dl
		je      found
		inc     eax
		jz      epilogue
	modulo2:
		cmp     byte ptr [esi + eax], dl
		je      found
		inc     eax
		jz      epilogue
	modulo3:
		cmp     byte ptr [esi + eax], dl
		je      found
		inc     eax
		jnz     loop_entry
		jmp     epilogue

		align   16
	loop_begin:
		add     eax, 4
		jc      retnull
	loop_entry:
		mov     ecx, dword ptr [esi + eax]                  // read 4 bytes
		mov     edi, -01010101H
		xor     ecx, edx                                    // edx is byte\byte\byte\byte
		add     edi, ecx
		xor     ecx, -1
		and     ecx, edi
		and     ecx, 80808080H
		jz      loop_begin
		and     ecx, 00808080H
		jnz     byte_0_to_2
		add     eax, 3
		jnc     found
	retnull:
		xor     eax, eax
		jmp     epilogue

		align   16
	byte_0_to_2:
		test    cl, cl
		jnz     found
		test    ch, ch
		jnz     byte_1
		inc     eax
		jz      epilogue
	byte_1:
		inc     eax
		jz      epilogue
	found:
		add     eax, esi
	epilogue:
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		ret     4

		#undef buffer
		#undef c
		#undef count
	}
}
#ifndef _DEBUG

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
#endif
