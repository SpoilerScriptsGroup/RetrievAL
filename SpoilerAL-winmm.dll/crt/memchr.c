#include <memory.h>
#include <xmmintrin.h>

#ifndef _M_IX86
void * __cdecl memchr(const void *buffer, int c, size_t count)
{
	while (count--)
		if (*(((char *)buffer)++) == (char)c)
			return (char *)buffer - 1;
	return NULL;
}
#else
#pragma function(memchr)

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

		mov     edx, dword ptr [count]                  // edx = count
		mov     ecx, dword ptr [buffer]                 // ecx = buffer
		test    edx, edx                                // check if count == 0
		jz      retnull                                 // if count == 0, leave
		movd    xmm0, dword ptr [c]                     // xmm0 = search char
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

		push    ebx                                     // preserve ebx
		mov     ebx, ecx
		mov     eax, edx
		add     ebx, edx                                // ebx = end of buffer
		mov     edx, ecx
		xor     eax, -1
		and     edx, -16
		add     eax, 1                                  // eax = -count
		and     ecx, 15
		jz      loop_begin
		movdqa  xmm1, xmmword ptr [edx]
		pcmpeqb xmm1, xmm0
		pmovmskb edx, xmm1
		shr     edx, cl
		test    edx, edx
		jnz     found
		sub     ecx, 16
		sub     eax, ecx
		jae     retnull

		align   16                                      // already aligned
	loop_begin:
		movdqa  xmm1, xmmword ptr [ebx + eax]
		pcmpeqb xmm1, xmm0
		pmovmskb edx, xmm1
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

__declspec(naked) void * __cdecl memchr386(const void *buffer, int c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                  // edx = count
		xor     eax, eax
		test    edx, edx                                // check if count == 0
		jz      retnull                                 // if count == 0, leave
		                                                // set all 4 bytes of edx to [value]
		mov     al, byte ptr [c]                        // al = search char
		mov     ecx, dword ptr [buffer]                 // ecx = buffer
		push    edx
		mov     edx, eax                                // edx = 0/0/0/c
		shl     eax, 8                                  // eax = 0/0/c/0
		or      eax, edx                                // eax = 0/0/c/c
		mov     edx, eax                                // edx = 0/0/c/c
		shl     eax, 16                                 // eax = c/c/0/0
		or      edx, eax                                // edx = all 4 bytes = [search char]
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

		push    ebx                                     // preserve ebx
		push    esi                                     // preserve esi
		mov     ebx, edx                                // ebx = c
		mov     edx, dword ptr [count + 8]
		mov     eax, edx
		xor     edx, -1
		add     eax, ecx                                // eax = end of buffer
		inc     edx                                     // edx = -count
		and     ecx, 3
		jz      loop_entry
		xor     ecx, 3
		jz      modulo3
		dec     ecx
		jz      modulo2
		cmp     byte ptr [eax + edx], bl
		je      found
		inc     edx
		jz      retnull
	modulo2:
		cmp     byte ptr [eax + edx], bl
		je      found
		inc     edx
		jz      retnull
	modulo3:
		cmp     byte ptr [eax + edx], bl
		je      found
		inc     edx
		jnz     loop_entry
		jmp     retnull

		align   16
	loop_begin:
		add     edx, 4
		jc      retnull
	loop_entry:
		mov     ecx, dword ptr [eax + edx]              // read 4 bytes
		mov     esi, 7EFEFEFFH
		xor     ecx, ebx                                // ebx is byte\byte\byte\byte
		add     esi, ecx
		xor     ecx, -1
		xor     ecx, esi
		and     ecx, 81010100H
		jz      loop_begin
		and     ecx, 01010100H
		jnz     byte_0_to_2
		test    esi, esi
		js      loop_begin
		add     edx, 3
		jnc     found
		jmp     retnull

		align   16
	byte_0_to_2:
		test    ch, ch
		jnz     found
		and     ecx, 00010000H
		jnz     byte_1
		inc     edx
		jz      retnull
	byte_1:
		inc     edx
		jz      retnull
	found:
		add     eax, edx
		jmp     epilogue

		align   16
	retnull:
		xor     eax, eax
	epilogue:
		pop     esi                                     // restore esi
		pop     ebx                                     // restore ebx
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
