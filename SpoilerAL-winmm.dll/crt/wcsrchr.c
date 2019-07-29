#include <windows.h>

#ifndef _M_IX86
wchar_t * __cdecl wcsrchr(const wchar_t *string, wint_t c)
{
	wchar_t *p, c2;

	if (!c)
		return (wchar_t *)string + wcslen(string);
	p = NULL;
	while (c2 = *(string++))
		if (c2 == c)
			p = (wchar_t *)string - 1;
	return p;
}
#else
#pragma function(wcslen)

static wchar_t * __cdecl wcsrchrSSE2(const wchar_t *string, wint_t c);
static wchar_t * __cdecl wcsrchr386(const wchar_t *string, wint_t c);
static wchar_t * __cdecl wcsrchrCPUDispatch(const wchar_t *string, wint_t c);

static wchar_t *(__cdecl * wcsrchrDispatch)(const wchar_t *string, wint_t c) = wcsrchrCPUDispatch;

__declspec(naked) wchar_t * __cdecl wcsrchr(const wchar_t *string, wint_t c)
{
	__asm
	{
		jmp     dword ptr [wcsrchrDispatch]
	}
}

__declspec(naked) static wchar_t * __cdecl wcsrchrSSE2(const wchar_t *string, wint_t c)
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
		push    ebx
		push    esi
		mov     ecx, edx
		xor     ebx, ebx
		movd    xmm2, ax
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		test    edx, 1
		jnz     unaligned
		or      eax, -1
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
		jz      aligned_loop_by_xmmword
		jmp     aligned_is_null

		align   16
	aligned_loop:
		mov     esi, eax
		mov     eax, -4
		shl     eax, cl
		lea     ebx, [edx + ecx]
		and     eax, esi
		jnz     aligned_is_null
	aligned_loop_by_xmmword:
		movdqa  xmm0, xmmword ptr [edx + 16]
		add     edx, 16
		pxor    xmm1, xmm1
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		por     xmm1, xmm0
		pmovmskb eax, xmm1
		test    eax, eax
		jz      aligned_loop_by_xmmword
	aligned_is_null:
		bsf     ecx, eax
		cmp     word ptr [edx + ecx], 0
		jne     aligned_loop
		jmp     epilogue

		align   16
	unaligned:
		inc     ecx
		or      eax, -1
		and     ecx, 15
		jz      unaligned_loop_by_xmmword
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
		jz      unaligned_loop_by_xmmword
		jmp     unaligned_is_null

		align   16
	unaligned_loop:
		mov     esi, ecx
		mov     eax, -4
		shl     eax, cl
		lea     ebx, [edx + ecx]
		and     eax, esi
		jnz     unaligned_is_null
	unaligned_loop_by_xmmword:
		movdqu  xmm0, xmmword ptr [edx + 16]
		add     edx, 16
		pxor    xmm1, xmm1
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		por     xmm1, xmm0
		pmovmskb eax, xmm1
		test    eax, eax
		jz      unaligned_loop_by_xmmword
	unaligned_is_null:
		bsf     ecx, eax
		cmp     word ptr [edx + ecx], 0
		jne     unaligned_loop

		align   16
	epilogue:
		mov     eax, ebx
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static wchar_t * __cdecl wcsrchr386(const wchar_t *string, wint_t c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     edx, dword ptr [string]
		xor     eax, eax
		mov     ax, word ptr [c]
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
		push    ebx
		xor     ebx, ebx

		align   16
	main_loop:
		mov     cx, word ptr [edx]
		add     edx, 2
		test    cx, cx
		jz      epilogue
		cmp     cx, ax
		jne     main_loop
		lea     ebx, [edx - 2]
		jmp     main_loop

		align   16
	epilogue:
		mov     eax, ebx
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static wchar_t * __cdecl wcsrchrCPUDispatch(const wchar_t *string, wint_t c)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wcsrchrDispatch], offset wcsrchr386
		jmp     wcsrchr386
	L1:
		mov     dword ptr [wcsrchrDispatch], offset wcsrchrSSE2
		jmp     wcsrchrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
