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

		mov     dx, word ptr [c]
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
		push    ebx
		push    esi
		movd    xmm2, dx
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		mov     ecx, eax
		test    eax, 1
		jnz     unaligned
		xor     ebx, ebx
		mov     edx, -1
		and     ecx, 15
		and     eax, -16
		shl     edx, cl
		movdqa  xmm0, xmmword ptr [eax]
		pxor    xmm1, xmm1
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb ecx, xmm0
		and     edx, ecx
		jz      aligned_loop_by_xmmword
		jmp     aligned_is_null

		align   16
	aligned_loop:
		mov     esi, edx
		mov     edx, -4
		shl     edx, cl
		lea     ebx, [eax + ecx]
		and     edx, esi
		jnz     aligned_is_null
	aligned_loop_by_xmmword:
		movdqa  xmm0, xmmword ptr [eax + 16]
		add     eax, 16
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jz      aligned_loop_by_xmmword
	aligned_is_null:
		bsf     ecx, edx
		cmp     word ptr [eax + ecx], 0
		jne     aligned_loop
		jmp     epilogue

		align   16
	unaligned:
		inc     ecx
		xor     ebx, ebx
		and     ecx, 15
		jz      unaligned_loop_by_xmmword
		mov     edx, -1
		sub     eax, ecx
		shl     edx, cl
		movdqa  xmm0, xmmword ptr [eax + 1]
		pslldq  xmm0, 1
		pxor    xmm1, xmm1
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb ecx, xmm0
		and     edx, ecx
		jz      unaligned_loop_by_xmmword
		jmp     unaligned_is_null

		align   16
	unaligned_loop:
		mov     esi, ecx
		mov     edx, -4
		shl     edx, cl
		lea     ebx, [eax + ecx]
		and     edx, esi
		jnz     unaligned_is_null
	unaligned_loop_by_xmmword:
		movdqu  xmm0, xmmword ptr [eax + 16]
		add     eax, 16
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jz      unaligned_loop_by_xmmword
	unaligned_is_null:
		bsf     ecx, edx
		cmp     word ptr [eax + ecx], 0
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

		mov     dx, word ptr [c]
		mov     ecx, dword ptr [string]
		test    dx, dx
		jz      chr_is_null
		push    ebx
		sub     ecx, 2
		xor     ebx, ebx

		align   16
	main_loop:
		mov     ax, word ptr [ecx + 2]
		add     ecx, 2
		cmp     ax, dx
		jne     is_null
		mov     ebx, ecx
		jmp     main_loop
	is_null:
		test    ax, ax
		jnz     main_loop
		mov     eax, ebx
		pop     ebx
		ret

		align   16
	chr_is_null:
		push    ecx
		push    ecx
		call    wcslen
		pop     edx
		pop     ecx
		lea     eax, [ecx + eax * 2]
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
