#define wmemchr inline_wmemchr
#include <wchar.h>
#undef wmemchr

#ifndef _M_IX86
wchar_t * __cdecl wmemchr(const wchar_t *buffer, wchar_t c, size_t count)
{
	while (count--)
		if (*(buffer++) == c)
			return buffer - 1;
	return NULL;
}
#else
static wchar_t * __cdecl wmemchrSSE2(const wchar_t *buffer, wchar_t c, size_t count);
static wchar_t * __cdecl wmemchr386(const wchar_t *buffer, wchar_t c, size_t count);
static wchar_t * __cdecl wmemchrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count);

static wchar_t *(__cdecl * wmemchrDispatch)(const wchar_t *buffer, wchar_t c, size_t count) = wmemchrCPUDispatch;

__declspec(naked) wchar_t * __cdecl wmemchr(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		jmp     dword ptr [wmemchrDispatch]
	}
}

__declspec(naked) static wchar_t * __cdecl wmemchrSSE2(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                  // edx = count
		mov     eax, dword ptr [buffer]                 // eax = buffere
		sub     edx, 1                                  // edx = count - 1
		jb      count_equal_zero                        // if count=0, leave
		push    ebx                                     // preserve ebx
		lea     ebx, [eax + edx * 2 + 2]                // ebx = end of buffer
		xor     edx, -1                                 // edx = -count
		nop                                             // padding 1 byte
		movd    xmm1, dword ptr [c + 4]                 // xmm1 = search char
		pshuflw xmm1, xmm1, 0
		movlhps xmm1, xmm1
		test    eax, 1
		jnz     unaligned
		mov     ecx, eax
		and     eax, -16
		and     ecx, 15
		jz      aligned_loop
		movdqa  xmm0, xmmword ptr [eax]
		pcmpeqw xmm0, xmm1
		pmovmskb eax, xmm0
		shr     eax, cl
		shr     ecx, 1
		test    eax, eax
		lea     ecx, [ecx - 8]
		jnz     found
		sub     edx, ecx
		jae     retnull_at_aligned

		align   16
	aligned_loop:                                      // already aligned
		movdqa  xmm0, xmmword ptr [ebx + edx * 2]
		pcmpeqw xmm0, xmm1
		pmovmskb eax, xmm0
		test    eax, eax
		jnz     found
		add     edx, 8
		jnc     aligned_loop
	retnull_at_aligned:
		pop     ebx                                     // restore ebx
	count_equal_zero:
		ret                                             // __cdecl return

		align   16
	unaligned:
		lea     ecx, [eax + 1]
		and     eax, -16
		and     ecx, 15
		jz      unaligned_loop
		movdqa  xmm0, xmmword ptr [eax]
		pslldq  xmm0, 1
		pcmpeqw xmm0, xmm1
		pmovmskb eax, xmm0
		shr     eax, cl
		shr     ecx, 1
		test    eax, eax
		lea     ecx, [ecx - 8]
		jnz     found
		sub     edx, ecx
		jb      unaligned_loop
		pop     ebx                                     // restore ebx
		ret                                             // __cdecl return

		align   16
	unaligned_loop:
		movdqu  xmm0, xmmword ptr [ebx + edx * 2]
		pcmpeqw xmm0, xmm1
		pmovmskb eax, xmm0
		test    eax, eax
		jnz     found
		add     edx, 8
		jnc     unaligned_loop
		pop     ebx                                     // restore ebx
		ret                                             // __cdecl return

		align   16
	found:
		bsf     eax, eax
		shr     eax, 1
		add     eax, edx
		jc      retnull
		lea     eax, [ebx + eax * 2]
		pop     ebx                                     // restore ebx
		ret                                             // __cdecl return

		align   16
	retnull:
		xor     eax, eax
		pop     ebx                                     // restore ebx
		ret                                             // __cdecl return

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static wchar_t * __cdecl wmemchr386(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [buffer]                 // edx = buffer
		mov     eax, dword ptr [count]                  // eax = count
		mov     cx, word ptr [c]                        // cx = search char
		lea     edx, [edx + eax * 2]                    // edx = end of buffer
		xor     eax, -1                                 // eax = -count - 1

		align   16
	loop_begin:
		inc     eax
		jz      retnull
		cmp     word ptr [edx + eax * 2], cx
		jne     loop_begin
		lea     eax, [edx + eax * 2]
	retnull:
		ret                                             // __cdecl return

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static wchar_t * __cdecl wmemchrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wmemchrDispatch], offset wmemchr386
		jmp     wmemchr386
	L1:
		mov     dword ptr [wmemchrDispatch], offset wmemchrSSE2
		jmp     wmemchrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
