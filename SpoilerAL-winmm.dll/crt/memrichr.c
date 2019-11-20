#include <memory.h>
#include <xmmintrin.h>

#pragma warning(disable:4414)

extern void * __cdecl _memrchr(const void *buffer, int c, size_t count);

extern const char xmmconst_casebitA[16];
extern const char xmmconst_maskbit[32];
#define casebit xmmconst_casebitA
#define maskbit xmmconst_maskbit

#ifndef _M_IX86
void * __cdecl _memrichr(const void *buffer, int c, size_t count)
{
	char c2;

	c2 = (char)c | ('a' - 'A');
	if ((unsigned char)(c2 - 'a') > 'z' - 'a')
		return _memrchr(buffer, c, count);
	while (count--)
		if ((((char *)buffer)[count] | ('a' - 'A')) == c2)
			return (char *)buffer + count;
	return NULL;
}
#else
static void * __cdecl memrichrSSE2(const void *buffer, int c, size_t count);
void * __vectorcall internal_memrichrSSE2(const void *buffer, __m128 c, size_t count);
static void * __cdecl memrichr386(const void *buffer, int c, size_t count);
void * __fastcall internal_memrichr386(const void *buffer, unsigned long c, size_t count);
static void * __cdecl memrichrCPUDispatch(const void *buffer, int c, size_t count);

static void *(__cdecl *memrichrDispatch)(const void *buffer, int c, size_t count) = memrichrCPUDispatch;

__declspec(naked) void * __cdecl _memrichr(const void *buffer, int c, size_t count)
{
	__asm
	{
		jmp     dword ptr [memrichrDispatch]
	}
}

__declspec(naked) static void * __cdecl memrichrSSE2(const void *buffer, int c, size_t count)
{
	extern void * __cdecl memrchrSSE2(const void *buffer, int c, size_t count);

	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                  // edx = count
		mov     ecx, dword ptr [c]
		test    edx, edx                                // check if count == 0
		jz      retnull                                 // if count == 0, leave
		or      ecx, 'a' - 'A'
		xor     eax, eax
		mov     al, cl
		sub     ecx, 'a'
		cmp     cl, 'z' - 'a'
		ja      memrchrSSE2
		movd    xmm0, eax                               // xmm0 = search char
		punpcklbw xmm0, xmm0
		pshuflw xmm0, xmm0, 0
		movlhps xmm0, xmm0
		mov     ecx, dword ptr [buffer]                 // ecx = buffer
		jmp     internal_memrichrSSE2

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) void * __vectorcall internal_memrichrSSE2(const void *buffer, __m128 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      xmm0
		#define count  edx

		push    ebx                                     // preserve ebx
		lea     ebx, [ecx + edx - 1]                    // ebx = last byte of buffer
		push    esi                                     // preserve esi
		and     ebx, -16                                // ebx = last xmmword of buffer
		add     ecx, edx                                // ecx = end of buffer
		sub     ebx, edx                                // ebx = last xmmword of buffer - count
		movdqa  xmm2, xmmword ptr [casebit]
		and     ecx, 15
		jz      loop_begin
		movdqa  xmm1, xmmword ptr [ebx + edx]
		por     xmm1, xmm2
		pcmpeqb xmm1, xmm0
		pmovmskb eax, xmm1
		mov     esi, 7FFFH
		xor     ecx, 15
		shr     esi, cl
		and     eax, esi
		jnz     has_char_at_last_xmmword
		sub     ebx, ecx
		xor     ecx, 15
		sub     edx, ecx
		jbe     retnull
		dec     ebx

		align   16
	loop_begin:
		movdqa  xmm1, xmmword ptr [ebx + edx]
		por     xmm1, xmm2
		pcmpeqb xmm1, xmm0
		pmovmskb eax, xmm1
		test    eax, eax
		jnz     has_char
		sub     edx, 16
		ja      loop_begin
	retnull:
		xor     eax, eax
		pop     esi                                     // restore esi
		pop     ebx                                     // restore ebx
		ret

		align   16
	has_char_at_last_xmmword:
		cmp     edx, 16
		jae     found
		xor     ecx, 15
		or      esi, -1
		sub     ecx, edx
		ja      mask_first_xmmword
		jmp     found

		align   16
	has_char:
		cmp     edx, 16
		jae     found
		mov     ecx, ebx
		or      esi, -1
		and     ecx, 15
		jz      found
	mask_first_xmmword:
		shl     esi, cl
		and     eax, esi
		jz      retnull
	found:
		bsr     eax, eax
		add     edx, ebx
		pop     esi                                     // restore esi
		pop     ebx                                     // restore ebx
		add     eax, edx
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static void * __cdecl memrichr386(const void *buffer, int c, size_t count)
{
	extern void * __cdecl memchr386(const void *buffer, int c, size_t count);

	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     eax, dword ptr [count]                  // eax = count
		mov     ecx, dword ptr [c]                      // cl = search char
		test    eax, eax                                // check if count=0
		jz      retnull                                 // if count=0, leave
		or      ecx, 'a' - 'A'
		xor     edx, edx
		mov     dl, cl
		sub     ecx, 'a'
		cmp     cl, 'z' - 'a'
		ja      memchr386
		push    eax
		                                                // set all 4 bytes of edx to [value]
		mov     eax, edx                                // eax = 0/0/0/c
		shl     edx, 8                                  // edx = 0/0/c/0
		mov     ecx, dword ptr [buffer + 4]             // ecx = buffer
		or      edx, eax                                // edx = 0/0/c/c
		mov     eax, edx                                // eax = 0/0/c/c
		shl     edx, 16                                 // eax = c/c/0/0
		or      edx, eax                                // edx = all 4 bytes = [search char]
		call    internal_memrichr386
	retnull:
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) void * __fastcall internal_memrichr386(const void *buffer, unsigned long c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      edx
		#define count  (esp + 4)

		push    ebx                                     // u preserve ebx
		push    esi                                     // v preserve esi
		push    edi                                     // v preserve edi
		mov     ebx, edx                                // ebx = c
		mov     edx, dword ptr [count + 12]             // edx = count
		mov     eax, ecx                                // eax = buffer
		add     ecx, edx                                // ecx = end of buffer
		dec     eax                                     // eax = buffer - 1
		and     ecx, 3
		jz      loop_entry
		dec     ecx
		jz      modulo1
		dec     ecx
		jz      modulo2
		mov     cl, byte ptr [eax + edx]
		or      cl, 'a' - 'A'
		cmp     cl, bl
		je      found
		dec     edx
		jz      retnull
	modulo2:
		mov     cl, byte ptr [eax + edx]
		or      cl, 'a' - 'A'
		cmp     cl, bl
		je      found
		dec     edx
		jz      retnull
	modulo1:
		mov     cl, byte ptr [eax + edx]
		or      cl, 'a' - 'A'
		cmp     cl, bl
		je      found
		dec     edx
		jnz     loop_entry
		jmp     retnull

		align   16
	loop_begin:
		sub     edx, 4
		jbe     retnull
	loop_entry:
		mov     esi, dword ptr [eax + edx - 3]          // read 4 bytes
		mov     edi, 7EFEFEFFH
		or      esi, 20202020H
		mov     ecx, esi
		xor     esi, ebx                                // ebx is byte\byte\byte\byte
		add     edi, esi
		xor     esi, -1
		xor     esi, edi
		and     esi, 81010100H
		jz      loop_begin
		and     esi, 01010100H
		jnz     has_char
		test    edi, edi
		js      loop_begin
		jmp     found

		align   16
	has_char:
		bswap   ecx
		cmp     cl, bl
		je      found
		cmp     ch, bl
		je      byte_2
		shr     ecx, 16
		cmp     cl, bl
		je      byte_1
		sub     edx, 3
		ja      found
		jmp     retnull

		align   16
	byte_1:
		dec     edx
		jz      retnull
	byte_2:
		dec     edx
		jz      retnull
	found:
		add     eax, edx
		jmp     epilogue

		align   16
	retnull:
		xor     eax, eax
	epilogue:
		pop     edi                                     // restore edi
		pop     esi                                     // restore esi
		pop     ebx                                     // restore ebx
		ret     4

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static void * __cdecl memrichrCPUDispatch(const void *buffer, int c, size_t count)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [memrichrDispatch], offset memrichr386
		jmp     memrichr386
	L1:
		mov     dword ptr [memrichrDispatch], offset memrichrSSE2
		jmp     memrichrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
