#include <string.h>

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

		mov     edx, dword ptr [maxlen]                     // edx = maxlen
		mov     eax, dword ptr [string]                     // eax = string
		test    edx, edx                                    // check if maxlen=0
		jnz     entry                                       // if maxlen=0, leave
		xor     eax, eax
		ret

		align   16
	entry:
		pxor    xmm1, xmm1                                  // xmm1 = zero clear
		push    ebx                                         // preserve ebx
		lea     ebx, [eax + edx * 2]                        // ebx = end of string
		test    eax, 1
		jnz     unaligned
		mov     ecx, eax
		and     eax, -16
		and     ecx, 15
		jz      negate_count_at_aligned
		movdqa  xmm0, xmmword ptr [eax]
		pcmpeqw xmm0, xmm1
		pmovmskb eax, xmm0
		shr     eax, cl
		lea     ecx, [ecx - 16]
		jnz     found_at_first
		neg     ecx
		shr     ecx, 1
	negate_count_at_aligned:
		mov     eax, edx                                    // eax = maxlen
		sub     ecx, edx                                    // ecx = negative count
		jae     prologue

		align   16
	aligned_loop:
		movdqa  xmm0, xmmword ptr [ebx + ecx * 2]
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jnz     found
		add     ecx, 8
		jnc     aligned_loop
		jmp     prologue

		align   16
	unaligned:
		lea     ecx, [eax + 1]
		and     eax, -16
		and     ecx, 15
		jz      negate_count_at_unaligned
		movdqa  xmm0, xmmword ptr [eax]
		pslldq  xmm0, 1
		pcmpeqw xmm0, xmm1
		pmovmskb eax, xmm0
		shr     eax, cl
		lea     ecx, [ecx - 16]
		jnz     found_at_first
		neg     ecx
		shr     ecx, 1
	negate_count_at_unaligned:
		mov     eax, edx                                    // eax = maxlen
		sub     ecx, edx                                    // ecx = negative count
		jae     prologue

		align   16
	unaligned_loop:
		movdqu  xmm0, xmmword ptr [ebx + ecx * 2]
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jnz     found
		add     ecx, 8
		jnc     unaligned_loop
		jmp     prologue

		align   16
	found_at_first:
		xor     ecx, ecx
		mov     ebx, eax
		mov     eax, edx                                    // eax = maxlen
		sub     ecx, edx                                    // ecx = negative count
		mov     edx, ebx                                    // edx = result of pmovmskb

		align   16
	found:
		bsf     edx, edx
		shr     edx, 1
		add     ecx, edx
		jc      prologue
		add     eax, ecx
	prologue:
		pop     ebx                                         // restore ebx
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
		jz      retnull

		align   16
	loop_head:
		mov     cx, word ptr [edx]
		add     edx, 2
		test    cx, cx
		jz      found
		dec     eax
		jnz     loop_head
	found:
		mov     ecx, eax
		mov     eax, dword ptr [maxlen]
		sub     eax, ecx
	retnull:
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
