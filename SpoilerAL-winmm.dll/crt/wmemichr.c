#define wmemchr inline_wmemchr
#include <wchar.h>
#undef wmemchr
extern wchar_t * __cdecl wmemchr(const wchar_t *buffer, wchar_t c, size_t count);
#ifndef _M_IX86

wchar_t * __cdecl _wmemichr(const wchar_t *buffer, wchar_t c, size_t count)
{
	wchar_t c2;

	c2 = c | ('a' - 'A');
	if (c2 - 'a' >= 'z' - 'a' + 1)
		return wmemchr(buffer, c, count);
	while (count--)
		if ((*(buffer++) | ('a' - 'A')) == c2)
			return (wchar_t *)buffer - 1;
	return NULL;
}
#else
#include <xmmintrin.h>

#pragma warning(disable:4414)

extern const wchar_t xmmconst_casebitW[8];
#define casebit xmmconst_casebitW

static wchar_t * __cdecl wmemichrSSE42(const wchar_t *buffer, wchar_t c, size_t count);
wchar_t * __vectorcall internal_wmemichrSSE42(const wchar_t *buffer, __m128 reserved, __m128 c, size_t count);
static wchar_t * __cdecl wmemichrSSE2(const wchar_t *buffer, wchar_t c, size_t count);
wchar_t * __vectorcall internal_wmemichrSSE2(const wchar_t *buffer, __m128 c, size_t count);
static wchar_t * __cdecl wmemichr386(const wchar_t *buffer, wchar_t c, size_t count);
static wchar_t * __cdecl wmemichrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count);

static wchar_t *(__cdecl *wmemichrDispatch)(const wchar_t *buffer, wchar_t c, size_t count) = wmemichrCPUDispatch;

__declspec(naked) wchar_t * __cdecl _wmemichr(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		jmp     dword ptr [wmemichrDispatch]
	}
}

// SSE4.2 version
__declspec(naked) static wchar_t * __cdecl wmemichrSSE42(const wchar_t *buffer, wchar_t c, size_t count)
{
	extern wchar_t * __cdecl wmemchrSSE42(const wchar_t *buffer, wchar_t c, size_t count);

	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                      // edx = count
		mov     ecx, dword ptr [c]
		test    edx, edx                                    // check if count == 0
		jz      retnull                                     // if count == 0, leave
		or      ecx, 'a' - 'A'
		xor     eax, eax
		mov     ax, cx
		sub     ecx, 'a'
		cmp     cx, 'z' - 'a' + 1
		jae     wmemchrSSE42
		mov     ecx, eax
		xor     eax, 'a' - 'A'
		shl     ecx, 16
		or      eax, ecx
		mov     ecx, dword ptr [buffer]                     // ecx = buffer
		movd    xmm1, eax                                   // xmm1 = search char
		jmp     internal_wmemichrSSE42

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) wchar_t * __vectorcall internal_wmemichrSSE42(const wchar_t *buffer, __m128 reserved, __m128 c, size_t count)
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
		jz      loop_entry
		sub     eax, ecx
		movdqa  xmm0, xmmword ptr [eax]
		jmp     compare

		align   16
	unaligned:
		inc     ecx
		and     eax, -16
		and     ecx, 15
		jz      loop_entry
		movdqa  xmm0, xmmword ptr [eax]
		pslldq  xmm0, 1
	compare:
		shr     ecx, 1
		mov     eax, 2
		pcmpestrm xmm1, xmm0, 00000001B
		jnc     increment
		movd    eax, xmm0
		shr     eax, cl
		jnz     found_at_first
	increment:
		sub     ecx, 8
		sub     edi, ecx
		jae     retnull
	loop_entry:
		mov     eax, 2

		align   16
	loop_begin:
		pcmpestri xmm1, xmmword ptr [esi + edi * 2], 00000001B
		jc      found
		add     edi, 8
		jnc     loop_begin
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
__declspec(naked) static wchar_t * __cdecl wmemichrSSE2(const wchar_t *buffer, wchar_t c, size_t count)
{
	extern wchar_t * __cdecl wmemchrSSE2(const wchar_t *buffer, wchar_t c, size_t count);

	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                      // edx = count
		mov     ecx, dword ptr [c]
		test    edx, edx                                    // check if count == 0
		jz      retnull                                     // if count == 0, leave
		or      ecx, 'a' - 'A'
		xor     eax, eax
		mov     ax, cx
		sub     ecx, 'a'
		cmp     cx, 'z' - 'a' + 1
		jae     wmemchrSSE2
		movd    xmm0, eax                                   // xmm0 = search char
		pshuflw xmm0, xmm0, 0
		movlhps xmm0, xmm0
		mov     ecx, dword ptr [buffer]                     // ecx = buffer
		jmp     internal_wmemichrSSE2

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) wchar_t * __vectorcall internal_wmemichrSSE2(const wchar_t *buffer, __m128 c, size_t count)
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
		movdqa  xmm2, xmmword ptr [casebit]
		test    eax, 1
		jnz     unaligned
		and     ecx, 15
		jz      aligned_loop
		sub     eax, ecx
		movdqa  xmm1, xmmword ptr [eax]
		por     xmm1, xmm2
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
		por     xmm1, xmm2
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
		por     xmm1, xmm2
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
		por     xmm1, xmm2
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

// 80386 version
__declspec(naked) static wchar_t * __cdecl wmemichr386(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     eax, dword ptr [count]
		mov     ecx, dword ptr [c]
		test    eax, eax
		jz      retnull
		or      ecx, 'a' - 'A'
		xor     edx, edx
		mov     dx, cx                                      // edx = search char
		sub     ecx, 'a'
		cmp     cx, 'z' - 'a' + 1
		jae     wmemchr
		mov     ecx, eax                                    // ecx = count
		mov     eax, dword ptr [buffer]                     // eax = buffer
		push    esi                                         // preserve esi
		sub     eax, 2                                      // eax = buffer - 1

		align   16
	loop_begin:
		mov     bx, word ptr [eax + 2]
		add     eax, 2
		or      bx, 'a' - 'A'
		cmp     bx, dx
		je      found
		dec     ecx
		jnz     loop_begin
		xor     eax, eax
	found:
		pop     esi                                         // restore esi
	retnull:
		ret                                                 // __cdecl return

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static wchar_t * __cdecl wmemichrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count)
{
	#define __ISA_AVAILABLE_X86   0
	#define __ISA_AVAILABLE_SSE2  1
	#define __ISA_AVAILABLE_SSE42 2

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_SSE2
		jbe     L1
		mov     dword ptr [wmemichrDispatch], offset wmemichrSSE42
		jmp     wmemichrSSE42

	L1:
		mov     dword ptr [wmemichrDispatch], offset wmemichrSSE2
		jb      L2
		jmp     wmemichrSSE2

	L2:
		mov     dword ptr [wmemichrDispatch], offset wmemichr386
		jmp     wmemichr386
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
}
#endif
