#include <windows.h>

#ifndef _M_IX86
wchar_t * __cdecl wcschr(const wchar_t *string, wint_t c)
{
	wchar_t c2;

	if (!c)
		return (wchar_t *)string + wcslen(string);
	do
		if ((c2 = *(string++)) == c)
			return (wchar_t *)string - 1;
	while (c2);
	return NULL;
}
#else
#pragma function(wcslen)

static wchar_t * __cdecl wcschrSSE2(const wchar_t *string, wint_t c);
static wchar_t * __cdecl wcschr386(const wchar_t *string, wint_t c);
static wchar_t * __cdecl wcschrCPUDispatch(const wchar_t *string, wint_t c);

static wchar_t *(__cdecl * wcschrDispatch)(const wchar_t *string, wint_t c) = wcschrCPUDispatch;

__declspec(naked) wchar_t * __cdecl wcschr(const wchar_t *string, wint_t c)
{
	__asm
	{
		jmp     dword ptr [wcschrDispatch]
	}
}

__declspec(naked) static wchar_t * __cdecl wcschrSSE2(const wchar_t *string, wint_t c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     ax, word ptr [c]
		mov     edx, dword ptr [string]
		test    ax, ax
		jnz     chr_is_not_null
		push    edx
		push    edx
		call    wcslen
		pop     edx
		pop     ecx
		lea     eax, [ecx + eax * 2]
		ret

		align   16
	chr_is_not_null:
		mov     ecx, edx
		movd    xmm2, ax
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		test    edx, 1
		jnz     unaligned
		mov     eax, -1
		and     ecx, 15
		shl     eax, cl
		sub     edx, ecx
		movdqa  xmm0, xmmword ptr [edx]
		pxor    xmm1, xmm1
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		por     xmm1, xmm0
		pmovmskb ecx, xmm1
		and     eax, ecx
		jnz     epilogue

		align   16
	aligned_loop:
		movdqa  xmm0, xmmword ptr [edx + 16]
		add     edx, 16
		pxor    xmm1, xmm1
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		por     xmm1, xmm0
		pmovmskb eax, xmm1
		test    eax, eax
		jz      aligned_loop
		jmp     epilogue

		align   16
	unaligned:
		inc     ecx
		or      eax, -1
		and     ecx, 15
		jz      unaligned_loop
		shl     eax, cl
		sub     edx, ecx
		movdqa  xmm0, xmmword ptr [edx + 1]
		pslldq  xmm0, 1
		pxor    xmm1, xmm1
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		por     xmm1, xmm0
		pmovmskb ecx, xmm1
		and     eax, ecx
		jnz     epilogue

		align   16
	unaligned_loop:
		movdqu  xmm0, xmmword ptr [edx + 16]
		add     edx, 16
		pxor    xmm1, xmm1
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		por     xmm1, xmm0
		pmovmskb eax, xmm1
		test    eax, eax
		jz      unaligned_loop

		align   16
	epilogue:
		bsf     eax, eax
		mov     cx, word ptr [edx + eax]
		add     eax, edx
		xor     edx, edx
		test    cx, cx
		cmovz   eax, edx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static wchar_t * __cdecl wcschr386(const wchar_t *string, wint_t c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     edx, dword ptr [string]
		xor     eax, eax
		mov     ax, word ptr [c]
		test    ax, ax
		jnz     main_loop
		push    edx
		push    edx
		call    wcslen
		pop     edx
		pop     ecx
		lea     eax, [ecx + eax * 2]
		ret

		align   16
	main_loop:
		mov     cx, word ptr [edx]
		add     edx, 2
		cmp     cx, ax
		je      epilogue
		test    cx, cx
		jnz     main_loop
		xor     eax, eax
		ret

		align   16
	epilogue:
		lea     eax, [edx - 2]
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static wchar_t * __cdecl wcschrCPUDispatch(const wchar_t *string, wint_t c)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wcschrDispatch], offset wcschr386
		jmp     wcschr386
	L1:
		mov     dword ptr [wcschrDispatch], offset wcschrSSE2
		jmp     wcschrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
