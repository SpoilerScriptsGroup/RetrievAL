#include <string.h>

#pragma warning(disable:4414)

#ifndef _M_IX86
wchar_t * __cdecl _wcsrichr(const wchar_t *string, wint_t c)
{
	wchar_t *p, c1, c2;

	c1 = c | ('a' - 'A');
	if ((unsigned)(c1 - 'a') > 'z' - 'a')
		return wcsrchr(string, c);
	p = NULL;
	do
		if (((c2 = *(string++)) | ('a' - 'A')) == c1)
			p = (wchar_t *)string - 1;
	while (c2);
	return p;
}
#else
extern const wchar_t xmmconst_casebitW[8];
#define casebit xmmconst_casebitW

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

		mov     edx, dword ptr [c]
		mov     eax, dword ptr [string]
		or      edx, 'a' - 'A'
		xor     ecx, ecx
		mov     cx, dx
		sub     edx, 'a'
		cmp     dx, 'z' - 'a'
		ja      wcsrchr
		push    ebx
		push    esi
		mov     edx, eax
		or      esi, -1
		pxor    xmm1, xmm1
		movd    xmm2, ecx
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		movdqa  xmm3, xmmword ptr [casebit]
		and     eax, 1
		jnz     unaligned
		mov     ecx, edx
		and     edx, -16
		and     ecx, 15
		jz      aligned_loop_entry
		shl     esi, cl
		movdqa  xmm0, xmmword ptr [edx]
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		pmovmskb ecx, xmm1
		pmovmskb ebx, xmm0
		pxor    xmm1, xmm1
		and     ecx, esi
		and     ebx, esi
		or      ebx, ecx
		jz      aligned_loop_increment
		test    ecx, ecx
		jnz     null_is_found

		align   16
	aligned_loop:
		mov     eax, edx
		mov     esi, ebx
	aligned_loop_increment:
		add     edx, 16
	aligned_loop_entry:
		movdqa  xmm0, xmmword ptr [edx]
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		pmovmskb ecx, xmm1
		pmovmskb ebx, xmm0
		or      ebx, ecx
		jz      aligned_loop_increment
		test    ecx, ecx
		jz      aligned_loop
		jmp     null_is_found

		align   16
	unaligned:
		lea     ecx, [edx + 1]
		and     edx, -16
		dec     eax
		dec     edx
		and     ecx, 15
		jz      unaligned_loop_increment
		shl     esi, cl
		movdqa  xmm0, xmmword ptr [edx + 1]
		pslldq  xmm0, 1
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		pmovmskb ecx, xmm1
		pmovmskb ebx, xmm0
		pxor    xmm1, xmm1
		and     ecx, esi
		and     ebx, esi
		or      ebx, ecx
		jz      unaligned_loop_increment
		test    ecx, ecx
		jnz     null_is_found

		align   16
	unaligned_loop:
		mov     eax, edx
		mov     esi, ebx
	unaligned_loop_increment:
		add     edx, 16
		movdqu  xmm0, xmmword ptr [edx]
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		pmovmskb ecx, xmm1
		pmovmskb ebx, xmm0
		or      ebx, ecx
		jz      unaligned_loop_increment
		test    ecx, ecx
		jz      unaligned_loop

		align   16
	null_is_found:
		xor     ebx, ecx
		jz      process_stored_pointer
		bsf     ecx, ecx
		xor     ecx, 15
		shl     ebx, cl
		and     ebx, 7FFFH
		jz      process_stored_pointer
		bsr     ebx, ebx
		pop     esi
		lea     eax, [edx + ebx - 1]
		pop     ebx
		sub     eax, ecx
		ret

		align   16
	process_stored_pointer:
		test    eax, eax
		jz      epilogue
		bsr     ecx, esi
		lea     eax, [eax + ecx - 1]
	epilogue:
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

		mov     eax, dword ptr [c]
		mov     ecx, dword ptr [string]
		or      eax, 'a' - 'A'
		sub     ecx, 2
		mov     edx, eax
		sub     eax, 'a'
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
