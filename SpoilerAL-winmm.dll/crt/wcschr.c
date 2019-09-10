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

		mov     edx, dword ptr [c]
		mov     eax, dword ptr [string]
		test    dx, dx
		jnz     chr_is_not_null
		push    eax
		push    eax
		call    wcslen
		pop     edx
		pop     ecx
		lea     eax, [ecx + eax * 2]
		ret

		align   16
	chr_is_not_null:
		pxor    xmm1, xmm1
		movd    xmm2, edx
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		test    eax, 1
		jnz     unaligned
		test    eax, 15
		jz      aligned_loop_entry
		mov     ecx, eax
		and     eax, -16
		and     ecx, 15
		or      edx, -1
		shl     edx, cl
		movdqa  xmm0, xmmword ptr [eax]
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb ecx, xmm0
		and     edx, ecx
		jnz     epilogue
		pxor    xmm1, xmm1

		align   16
	aligned_loop:
		add     eax, 16
	aligned_loop_entry:
		movdqa  xmm0, xmmword ptr [eax]
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jz      aligned_loop
		jmp     epilogue

		align   16
	unaligned:
		mov     ecx, eax
		and     eax, -16
		inc     ecx
		dec     eax
		and     ecx, 15
		jz      unaligned_loop_entry
		mov     edx, -1
		shl     edx, cl
		movdqa  xmm0, xmmword ptr [eax + 1]
		pslldq  xmm0, 1
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb ecx, xmm0
		and     edx, ecx
		jnz     epilogue
		pxor    xmm1, xmm1

		align   16
	unaligned_loop:
		add     eax, 16
	unaligned_loop_entry:
		movdqu  xmm0, xmmword ptr [eax]
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jz      unaligned_loop

		align   16
	epilogue:
		bsf     edx, edx
		mov     cx, word ptr [eax + edx]
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

		mov     edx, dword ptr [c]
		mov     eax, dword ptr [string]
		test    dx, dx
		jz      chr_is_null
		sub     eax, 2

		align   16
	main_loop:
		mov     cx, word ptr [eax + 2]
		add     eax, 2
		cmp     cx, dx
		je      epilogue
		test    cx, cx
		jnz     main_loop
		xor     eax, eax
	epilogue:
		ret

		align   16
	chr_is_null:
		push    eax
		push    eax
		call    wcslen
		pop     edx
		pop     ecx
		lea     eax, [ecx + eax * 2]
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
