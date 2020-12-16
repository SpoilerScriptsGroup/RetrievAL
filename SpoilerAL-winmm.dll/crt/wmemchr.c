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
#include <xmmintrin.h>

wchar_t * __cdecl wmemchrSSE42(const wchar_t *buffer, wchar_t c, size_t count);
wchar_t * __vectorcall internal_wmemchrSSE42(const wchar_t *buffer, __m128 reserved, __m128 c, size_t count);
wchar_t * __cdecl wmemchrSSE2(const wchar_t *buffer, wchar_t c, size_t count);
wchar_t * __vectorcall internal_wmemchrSSE2(const wchar_t *buffer, __m128 c, size_t count);
#ifndef _DEBUG
static wchar_t * __cdecl wmemchr386(const wchar_t *buffer, wchar_t c, size_t count);
static wchar_t * __cdecl wmemchrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count);

static wchar_t *(__cdecl *wmemchrDispatch)(const wchar_t *buffer, wchar_t c, size_t count) = wmemchrCPUDispatch;

__declspec(naked) wchar_t * __cdecl wmemchr(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		jmp     dword ptr [wmemchrDispatch]
	}
}
#endif

// SSE4.2 version
__declspec(naked) wchar_t * __cdecl wmemchrSSE42(const wchar_t *buffer, wchar_t c, size_t count)
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
		movd    xmm1, dword ptr [c]                         // xmm1 = search char
		jmp     internal_wmemchrSSE42

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) wchar_t * __vectorcall internal_wmemchrSSE42(const wchar_t *buffer, __m128 reserved, __m128 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      xmm1
		#define count  edx

		push    esi                                         // preserve esi
		push    edi                                         // preserve edi
		mov     edi, edx                                    // edi = count
		mov     eax, ecx                                    // eax = buffer
		xor     edi, -1
		lea     esi, [ecx + edx * 2]                        // esi = end of buffer
		inc     edi                                         // edi = -count
		mov     edx, 8
		test    eax, 1
		jnz     unaligned
		and     ecx, 15
		jz      aligned_loop_entry
		sub     eax, ecx
		movdqa  xmm0, xmmword ptr [eax]
		shr     ecx, 1
		mov     eax, 1
		pcmpestrm xmm1, xmm0, 00000001B
		jnc     aligned_increment
		movd    eax, xmm0
		shr     eax, cl
		jnz     found_at_first
	aligned_increment:
		sub     ecx, 8
		sub     edi, ecx
		jae     retnull
	aligned_loop_entry:
		mov     eax, 1

		align   16
	aligned_loop:
		pcmpestri xmm1, xmmword ptr [esi + edi * 2], 00000001B
		jc      found
		add     edi, 8
		jnc     aligned_loop
		jmp     retnull

		align   16
	unaligned:
		inc     ecx
		and     eax, -16
		and     ecx, 15
		jz      unaligned_loop_entry
		movdqa  xmm0, xmmword ptr [eax]
		pslldq  xmm0, 1
		shr     ecx, 1
		mov     eax, 1
		pcmpestrm xmm1, xmm0, 00000001B
		jnc     aligned_increment
		movd    eax, xmm0
		shr     eax, cl
		jnz     found_at_first
	unaligned_increment:
		sub     ecx, 8
		sub     edi, ecx
		jae     retnull
	unaligned_loop_entry:
		mov     eax, 1

		align   16
	unaligned_loop:
		pcmpestri xmm1, xmmword ptr [esi + edi * 2], 00000001B
		jc      found
		add     edi, 8
		jnc     unaligned_loop
	retnull:
		xor     eax, eax
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		ret                                                 // __cdecl return

		align   16
	found_at_first:
		bsf     ecx, eax
	found:
		add     ecx, edi
		jc      retnull
		lea     eax, [esi + ecx * 2]
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		ret                                                 // __cdecl return

		#undef buffer
		#undef c
		#undef count
	}
}

// SSE2 version
__declspec(naked) wchar_t * __cdecl wmemchrSSE2(const wchar_t *buffer, wchar_t c, size_t count)
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
		pshuflw xmm0, xmm0, 0
		movlhps xmm0, xmm0
		jmp     internal_wmemchrSSE2

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) wchar_t * __vectorcall internal_wmemchrSSE2(const wchar_t *buffer, __m128 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      xmm0
		#define count  edx

		push    esi                                         // preserve esi
		lea     esi, [ecx + edx * 2]                        // esi = end of buffer
		mov     eax, ecx                                    // eax = buffer
		neg     edx                                         // edx = -count
		test    eax, 1
		jnz     unaligned
		and     ecx, 15
		jz      aligned_loop
		sub     eax, ecx
		movdqa  xmm1, xmmword ptr [eax]
		pcmpeqw xmm1, xmm0
		pmovmskb eax, xmm1
		shr     eax, cl
		shr     ecx, 1
		test    eax, eax
		lea     ecx, [ecx - 8]
		jnz     found
		sub     edx, ecx
		jb      aligned_loop
		pop     esi                                         // restore esi
		ret                                                 // __cdecl return

		align   16
	aligned_loop:
		movdqa  xmm1, xmmword ptr [esi + edx * 2]
		pcmpeqw xmm1, xmm0
		pmovmskb eax, xmm1
		test    eax, eax
		jnz     found
		add     edx, 8
		jnc     aligned_loop
		pop     esi                                         // restore esi
		ret                                                 // __cdecl return

		align   16
	unaligned:
		inc     ecx
		and     eax, -16
		and     ecx, 15
		jz      unaligned_loop
		movdqa  xmm1, xmmword ptr [eax]
		pslldq  xmm1, 1
		pcmpeqw xmm1, xmm0
		pmovmskb eax, xmm1
		shr     eax, cl
		shr     ecx, 1
		test    eax, eax
		lea     ecx, [ecx - 8]
		jnz     found
		sub     edx, ecx
		jb      unaligned_loop
		pop     esi                                         // restore esi
		ret                                                 // __cdecl return

		align   16
	unaligned_loop:
		movdqu  xmm1, xmmword ptr [esi + edx * 2]
		pcmpeqw xmm1, xmm0
		pmovmskb eax, xmm1
		test    eax, eax
		jnz     found
		add     edx, 8
		jnc     unaligned_loop
		pop     esi                                         // restore esi
		ret                                                 // __cdecl return

		align   16
	found:
		bsf     eax, eax
		shr     eax, 1
		add     eax, edx
		jc      retnull
		lea     eax, [esi + eax * 2]
		pop     esi                                         // restore esi
		ret                                                 // __cdecl return

		align   16
	retnull:
		xor     eax, eax
		pop     esi                                         // restore esi
		ret                                                 // __cdecl return

		#undef buffer
		#undef c
		#undef count
	}
}
#ifndef _DEBUG

// 80386 version
__declspec(naked) static wchar_t * __cdecl wmemchr386(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [buffer]                     // edx = buffer
		mov     eax, dword ptr [count]                      // eax = count
		mov     cx, word ptr [c]                            // cx = search char
		lea     edx, [edx + eax * 2]                        // edx = end of buffer
		xor     eax, -1                                     // eax = -count - 1

		align   16
	loop_begin:
		inc     eax
		jz      retnull
		cmp     word ptr [edx + eax * 2], cx
		jne     loop_begin
		lea     eax, [edx + eax * 2]
	retnull:
		ret                                                 // __cdecl return

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static wchar_t * __cdecl wmemchrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count)
{
	#define __ISA_AVAILABLE_X86   0
	#define __ISA_AVAILABLE_SSE2  1
	#define __ISA_AVAILABLE_SSE42 2

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_SSE2
		jbe     L1
		mov     dword ptr [wmemchrDispatch], offset wmemchrSSE42
		jmp     wmemchrSSE42

	L1:
		mov     dword ptr [wmemchrDispatch], offset wmemchrSSE2
		jb      L2
		jmp     wmemchrSSE2

	L2:
		mov     dword ptr [wmemchrDispatch], offset wmemchr386
		jmp     wmemchr386
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
}
#endif
#endif
