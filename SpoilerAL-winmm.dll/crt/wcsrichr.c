#include <string.h>

#pragma warning(disable:4414)

#ifndef _M_IX86
wchar_t * __cdecl _wcsrichr(const wchar_t *string, wint_t c)
{
	const wchar_t *p;
	wchar_t       c1, c2;

	c1 = c | ('a' - 'A');
	if (c1 - 'a' > 'z' - 'a')
		return wcsrchr(string, c);
	p = string - 1;
	do
		if (((c2 = *(++p)) | ('a' - 'A')) == c1)
			goto DONE;
	while (c2);
	p = NULL;
DONE:
	return (wchar_t *)p;
}
#else
extern const wchar_t casebitW[8];

static wchar_t * __cdecl wcsrichrSSE2(const wchar_t *string, wint_t c);
static wchar_t * __cdecl wcsrichr386(const wchar_t *string, wint_t c);
static wchar_t * __cdecl wcsrichrCPUDispatch(const wchar_t *string, wint_t c);

static wchar_t *(__cdecl * wcsrichrDispatch)(const wchar_t *string, wint_t c) = wcsrichrCPUDispatch;

__declspec(naked) wchar_t * __cdecl _wcsrichr(const wchar_t *string, wint_t c)
{
	__asm
	{
		jmp     dword ptr [wcsrichrDispatch]
	}
}

__declspec(naked) static wchar_t * __cdecl wcsrichrSSE2(const wchar_t *string, wint_t c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     ax, word ptr [c]
		mov     edx, dword ptr [string]
		or      ax, 'a' - 'A'
		mov     cx, ax
		sub     ax, 'a'
		cmp     ax, 'z' - 'a'
		ja      wcsrchr
		push    ebx
		push    esi
		movd    xmm3, cx
		pshuflw xmm3, xmm3, 0
		movlhps xmm3, xmm3
		movdqa  xmm4, xmm3
		psubw   xmm4, xmmword ptr [casebitW]
		xor     ebx, ebx
		mov     ecx, edx
		test    edx, 1
		jnz     unaligned
		mov     eax, -1
		and     ecx, 15
		shl     eax, cl
		sub     edx, ecx
		movdqa  xmm1, xmmword ptr [edx]
		pxor    xmm0, xmm0
		movdqa  xmm2, xmm1
		pcmpeqw xmm0, xmm1
		pcmpeqw xmm1, xmm3
		pcmpeqw xmm2, xmm4
		por     xmm0, xmm1
		por     xmm0, xmm2
		pmovmskb ecx, xmm0
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
		movdqa  xmm1, xmmword ptr [edx + 16]
		add     edx, 16
		pxor    xmm0, xmm0
		movdqa  xmm2, xmm1
		pcmpeqw xmm0, xmm1
		pcmpeqw xmm1, xmm3
		pcmpeqw xmm2, xmm4
		por     xmm0, xmm1
		por     xmm0, xmm2
		pmovmskb eax, xmm0
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
		movdqa  xmm1, xmmword ptr [edx + 1]
		pslldq  xmm1, 1
		pxor    xmm0, xmm0
		movdqa  xmm2, xmm1
		pcmpeqw xmm0, xmm1
		pcmpeqw xmm1, xmm3
		pcmpeqw xmm2, xmm4
		por     xmm0, xmm1
		por     xmm0, xmm2
		pmovmskb ecx, xmm0
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
		movdqu  xmm1, xmmword ptr [edx + 16]
		add     edx, 16
		pxor    xmm0, xmm0
		movdqa  xmm2, xmm1
		pcmpeqw xmm0, xmm1
		pcmpeqw xmm1, xmm3
		pcmpeqw xmm2, xmm4
		por     xmm0, xmm1
		por     xmm0, xmm2
		pmovmskb eax, xmm0
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

__declspec(naked) static wchar_t * __cdecl wcsrichr386(const wchar_t *string, wint_t c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     cx, word ptr [c]
		mov     eax, dword ptr [string]
		or      cx, 'a' - 'A'
		sub     eax, 2
		mov     dx, cx
		sub     cx, 'a'
		cmp     cx, 'z' - 'a'
		ja      wcsrchr
		push    ebx
		xor     ebx, ebx

		align   16
	main_loop:
		mov     cx, word ptr [eax + 2]
		add     eax, 2
		test    cx, cx
		jz      epilogue
		or      cx, 'a' - 'A'
		cmp     cx, dx
		jne     main_loop
		mov     ebx, eax
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

__declspec(naked) static wchar_t * __cdecl wcsrichrCPUDispatch(const wchar_t *string, wint_t c)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wcsrichrDispatch], offset wcsrichr386
		jmp     wcsrichr386
	L1:
		mov     dword ptr [wcsrichrDispatch], offset wcsrichrSSE2
		jmp     wcsrichrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
