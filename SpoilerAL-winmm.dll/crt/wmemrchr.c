#include <memory.h>

#ifndef _M_IX86
wchar_t * __cdecl _wmemrchr(const wchar_t *buffer, wchar_t c, size_t count)
{
	while (count--)
		if (buffer[count] == c)
			return (wchar_t *)buffer + count;
	return NULL;
}
#else
static wchar_t * __cdecl wmemrchrSSE2(const wchar_t *buffer, wchar_t c, size_t count);
static wchar_t * __cdecl wmemrchr386(const wchar_t *buffer, wchar_t c, size_t count);
static wchar_t * __cdecl wmemrchrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count);

static wchar_t *(__cdecl * wmemrchrDispatch)(const wchar_t *buffer, wchar_t c, size_t count) = wmemrchrCPUDispatch;

__declspec(naked) wchar_t * __cdecl _wmemrchr(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		jmp     dword ptr [wmemrchrDispatch]
	}
}

__declspec(naked) static wchar_t * __cdecl wmemrchrSSE2(const wchar_t *buffer, wchar_t c, size_t count)
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
		push    esi                                     // preserve esi
		lea     ebx, [ecx + eax * 2 - 2]                // ebx = last word of buffer
		lea     edx, [eax + eax]                        // edx = count * 2
		and     ebx, -16 or 1                           // ebx = last xmmword of buffer
		add     ecx, edx                                // ecx = end of buffer
		sub     ebx, edx                                // ebx = last xmmword of buffer - count
		mov     edx, ecx                                // edx = end of buffer
		movd    xmm1, dword ptr [c + 8]                 // xmm1 = search char
		pshuflw xmm1, xmm1, 0
		movlhps xmm1, xmm1
		and     ecx, 15
		jz      aligned_loop
		and     edx, 1
		jnz     unaligned
		movdqa  xmm0, xmmword ptr [ebx + eax * 2]
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		mov     esi, 3FFFH
		xor     ecx, 14
		shr     esi, cl
		and     edx, esi
		jnz     has_char_at_last_xmmword
		sub     ebx, ecx
		xor     ecx, 14
		shr     ecx, 1
		sub     ebx, 2
		sub     eax, ecx
		ja      aligned_loop
		jmp     retnull

		align   16
	aligned_loop:
		movdqa  xmm0, xmmword ptr [ebx + eax * 2]
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jnz     has_char
		sub     eax, 8
		ja      aligned_loop
		jmp     retnull

		align   16
	unaligned:
		dec     ecx
		jz      unaligned_loop
		movdqa  xmm0, xmmword ptr [ebx + eax * 2 - 1]
		psrldq  xmm0, 1
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		mov     esi, 3FFFH
		xor     ecx, 14
		shr     esi, cl
		and     edx, esi
		jnz     has_char_at_last_xmmword
		sub     ebx, ecx
		xor     ecx, 14
		shr     ecx, 1
		sub     ebx, 2
		sub     eax, ecx
		ja      unaligned_loop
		jmp     retnull

		align   16
	unaligned_loop:
		movdqu  xmm0, xmmword ptr [ebx + eax * 2]
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jnz     has_char
		sub     eax, 8
		ja      unaligned_loop
	retnull:
		xor     eax, eax
		pop     esi                                     // restore esi
		pop     ebx                                     // restore ebx
	count_equal_zero:
		ret

		align   16
	has_char_at_last_xmmword:
		cmp     eax, 8
		jae     found
		xor     ecx, 14
		lea     esi, [eax + eax]
		sub     ecx, esi
		jbe     found
		or      esi, -1
		jmp     mask_first_xmmword

		align   16
	has_char:
		cmp     eax, 8
		jae     found
		mov     ecx, ebx
		or      esi, -1
		and     ecx, 14
		jz      found
	mask_first_xmmword:
		shl     esi, cl
		and     edx, esi
		jz      retnull
	found:
		bsr     edx, edx
		pop     esi                                     // restore esi
		lea     eax, [ebx + eax * 2 - 1]
		pop     ebx                                     // restore ebx
		add     eax, edx
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static wchar_t * __cdecl wmemrchr386(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     eax, dword ptr [count]                  // eax = count
		mov     edx, dword ptr [buffer]                 // edx = buffer
		test    eax, eax
		jz      retnull
		push    ebx                                     // preserve ebx
		mov     ecx, eax
		add     eax, eax
		mov     bx, word ptr [c + 4]                    // cx = search char
		add     eax, edx

		align   16
	loop_begin:
		mov     dx, word ptr [eax - 2]
		sub     eax, 2
		cmp     dx, bx
		je      found
		dec     ecx
		jnz     loop_begin
		xor     eax, eax
	found:
		pop     ebx                                     // restore ebx
	retnull:
		ret                                             // __cdecl return

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static wchar_t * __cdecl wmemrchrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wmemrchrDispatch], offset wmemrchr386
		jmp     wmemrchr386
	L1:
		mov     dword ptr [wmemrchrDispatch], offset wmemrchrSSE2
		jmp     wmemrchrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
