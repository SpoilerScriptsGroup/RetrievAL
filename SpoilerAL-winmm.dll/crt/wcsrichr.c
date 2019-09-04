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

		mov     cx, word ptr [c]
		mov     eax, dword ptr [string]
		or      cx, 'a' - 'A'
		xor     edx, edx
		mov     dx, cx
		sub     cx, 'a'
		cmp     cx, 'z' - 'a'
		ja      wcsrchr
		push    ebx
		mov     ecx, eax
		movd    xmm2, edx
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		movdqa  xmm3, xmmword ptr [casebitW]
		test    eax, 1
		jnz     unaligned
		xor     ebx, ebx
		or      edx, -1
		and     ecx, 15
		and     eax, -16
		shl     edx, cl
		movdqa  xmm0, xmmword ptr [eax]
		pxor    xmm1, xmm1
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb ecx, xmm0
		and     edx, ecx
		jnz     aligned_is_null
		pxor    xmm1, xmm1

		align   16
	aligned_loop:
		movdqa  xmm0, xmmword ptr [eax + 16]
		add     eax, 16
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jz      aligned_loop
	aligned_is_null:
		pmovmskb ecx, xmm1
		test    ecx, ecx
		jnz     null_found
		bsr     edx, edx
		lea     ebx, [eax + edx]
		jmp     aligned_loop

		align   16
	unaligned:
		inc     ecx
		xor     ebx, ebx
		and     ecx, 15
		jz      unaligned_loop
		or      edx, -1
		sub     eax, ecx
		shl     edx, cl
		movdqa  xmm0, xmmword ptr [eax + 1]
		pslldq  xmm0, 1
		pxor    xmm1, xmm1
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb ecx, xmm0
		and     edx, ecx
		jnz     unaligned_is_null
		pxor    xmm1, xmm1

		align   16
	unaligned_loop:
		movdqu  xmm0, xmmword ptr [eax + 16]
		add     eax, 16
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jz      unaligned_loop
	unaligned_is_null:
		pmovmskb ecx, xmm1
		test    ecx, ecx
		jnz     null_found
		bsr     edx, edx
		lea     ebx, [eax + edx]
		jmp     unaligned_loop

		align   16
	null_found:
		xor     edx, ecx
		jz      epilogue
		bsf     ecx, ecx
		xor     ecx, 15
		shl     edx, cl
		and     edx, 7FFFH
		jz      epilogue
		bsr     edx, edx
		sub     edx, ecx
		lea     ebx, [eax + edx]
	epilogue:
		mov     eax, ebx
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

		mov     ax, word ptr [c]
		mov     ecx, dword ptr [string]
		or      ax, 'a' - 'A'
		sub     ecx, 2
		mov     dx, ax
		sub     ax, 'a'
		cmp     ax, 'z' - 'a'
		ja      wcsrchr
		push    ebx
		push    esi
		xor     esi, esi

		align   16
	main_loop:
		mov     ax, word ptr [ecx + 2]
		add     ecx, 2
		mov     bx, ax
		or      ax, 'a' - 'A'
		cmp     ax, dx
		jne     is_null
		mov     esi, ecx
		jmp     main_loop
	is_null:
		test    bx, bx
		jnz     main_loop
		mov     eax, esi
		pop     esi
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
