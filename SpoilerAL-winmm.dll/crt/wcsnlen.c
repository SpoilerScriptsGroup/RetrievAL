#include <wchar.h>

#ifndef _M_IX86
size_t __cdecl wcsnlen(const wchar_t *string, size_t maxlen)
{
	size_t length;

	if (length = maxlen)
	{
		while (*(string++) && --length);
		length = maxlen - length;
	}
	return length;
}
#else
static size_t __cdecl wcsnlenSSE2(const wchar_t *string, size_t maxlen);
static size_t __cdecl wcsnlen386(const wchar_t *string, size_t maxlen);
static size_t __cdecl wcsnlenCPUDispatch(const wchar_t *string, size_t maxlen);

static size_t(__cdecl * wcsnlenDispatch)(const wchar_t *string, size_t maxlen) = wcsnlenCPUDispatch;

__declspec(naked) size_t __cdecl wcsnlen(const wchar_t *string, size_t maxlen)
{
	__asm
	{
		jmp     dword ptr [wcsnlenDispatch]
	}
}

__declspec(naked) static size_t __cdecl wcsnlenSSE2(const wchar_t *string, size_t maxlen)
{
	__asm
	{
		#define string (esp + 4)
		#define maxlen (esp + 8)

		mov     eax, dword ptr [maxlen]                     // eax = maxlen
		mov     edx, dword ptr [string]                     // edx = string
		test    eax, eax                                    // check if maxlen=0
		jz      retzero                                     // if maxlen=0, leave
		pxor    xmm1, xmm1                                  // xmm1 = zero clear
		push    esi                                         // preserve esi
		lea     esi, [edx + eax * 2]                        // esi = end of string
		test    edx, 1
		jnz     unaligned
		mov     ecx, edx
		and     edx, -16
		and     ecx, 15
		jz      negate_count_at_aligned
		movdqa  xmm0, xmmword ptr [edx]
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		shr     edx, cl
		xor     ecx, 15
		test    edx, edx
		lea     ecx, [ecx + 1]
		jnz     found_at_first
		shr     ecx, 1
	negate_count_at_aligned:
		sub     ecx, eax                                    // ecx = negative count
		jb      aligned_loop
		pop     esi                                         // restore esi
	retzero:
		ret

		align   16
	aligned_loop:
		movdqa  xmm0, xmmword ptr [esi + ecx * 2]
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jnz     found
		add     ecx, 8
		jnc     aligned_loop
		pop     esi                                         // restore esi
		ret

		align   16
	unaligned:
		lea     ecx, [edx + 1]
		and     edx, -16
		and     ecx, 15
		jz      negate_count_at_unaligned
		movdqa  xmm0, xmmword ptr [edx]
		pslldq  xmm0, 1
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		shr     edx, cl
		xor     ecx, 15
		test    edx, edx
		lea     ecx, [ecx + 1]
		jnz     found_at_first
		shr     ecx, 1
	negate_count_at_unaligned:
		sub     ecx, eax                                    // ecx = negative count
		jb      unaligned_loop
		pop     esi                                         // restore esi
		ret

		align   16
	unaligned_loop:
		movdqu  xmm0, xmmword ptr [esi + ecx * 2]
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jnz     found
		add     ecx, 8
		jnc     unaligned_loop
		pop     esi                                         // restore esi
		ret

		align   8
		nop __asm nop __asm nop __asm nop                   // padding 4 byte
	found_at_first:
		xor     ecx, ecx
		sub     ecx, eax                                    // ecx = negative count

		align   16                                          // already aligned
	found:
		bsf     edx, edx
		shr     edx, 1
		add     ecx, edx
		jc      epilog
		add     eax, ecx
	epilog:
		pop     esi                                         // restore esi
		ret

		#undef string
		#undef maxlen
	}
}

__declspec(naked) static size_t __cdecl wcsnlen386(const wchar_t *string, size_t maxlen)
{
	__asm
	{
		#define string (esp + 4)
		#define maxlen (esp + 8)

		mov     eax, dword ptr [maxlen]
		mov     edx, dword ptr [string]
		test    eax, eax
		jz      retzero

		align   16
	loop_begin:
		mov     cx, word ptr [edx]
		add     edx, 2
		test    cx, cx
		jz      found
		dec     eax
		jnz     loop_begin
	found:
		mov     ecx, eax
		mov     eax, dword ptr [maxlen]
		sub     eax, ecx
	retzero:
		ret

		#undef string
		#undef maxlen
	}
}

__declspec(naked) static size_t __cdecl wcsnlenCPUDispatch(const wchar_t *string, size_t maxlen)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wcsnlenDispatch], offset wcsnlen386
		jmp     wcsnlen386

	L1:
		mov     dword ptr [wcsnlenDispatch], offset wcsnlenSSE2
		jmp     wcsnlenSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
