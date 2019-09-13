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

		mov     ecx, dword ptr [c]
		mov     eax, dword ptr [string]
		or      ecx, 'a' - 'A'
		xor     edx, edx
		mov     dx, cx
		sub     ecx, 'a'
		cmp     cx, 'z' - 'a'
		ja      wcsrchr
		push    ebx
		xor     ebx, ebx
		pxor    xmm1, xmm1
		movd    xmm2, edx
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		movdqa  xmm3, xmmword ptr [casebitW]
		test    eax, 1
		jnz     unaligned
		test    eax, 15
		jz      aligned_loop_entry
		mov     ecx, eax
		and     eax, -16
		and     ecx, 15
		dec     ebx
		shl     ebx, cl
		movdqa  xmm0, xmmword ptr [eax]
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		pmovmskb ecx, xmm1
		pmovmskb edx, xmm0
		pxor    xmm1, xmm1
		and     ecx, ebx
		and     edx, ebx
		xor     ebx, ebx
		or      edx, ecx
		jz      aligned_loop_increment
		cmp     ecx, 0                                  // append 1 byte (test ecx,ecx -> cmp ecx,0)
		jne     null_is_found

		align   16                                      // already aligned
	aligned_loop:
		bsr     edx, edx
		lea     ebx, [eax + edx]
	aligned_loop_increment:
		add     eax, 16
	aligned_loop_entry:
		movdqa  xmm0, xmmword ptr [eax]
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jz      aligned_loop_increment
		pmovmskb ecx, xmm1
		test    ecx, ecx
		jz      aligned_loop
		jmp     null_is_found

		align   16
	unaligned:
		mov     ecx, eax
		and     eax, -16
		inc     ecx
		dec     eax
		and     ecx, 15
		jz      unaligned_loop_increment
		dec     ebx
		shl     ebx, cl
		movdqa  xmm0, xmmword ptr [eax + 1]
		pslldq  xmm0, 1
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		pmovmskb ecx, xmm1
		pmovmskb edx, xmm0
		pxor    xmm1, xmm1
		and     ecx, ebx
		and     edx, ebx
		xor     ebx, ebx
		or      edx, ecx
		jz      unaligned_loop_increment
		cmp     ecx, 0                                  // append 1 byte (test ecx,ecx -> cmp ecx,0)
		jne     null_is_found

		align   16                                      // already aligned
	unaligned_loop:
		bsr     edx, edx
		lea     ebx, [eax + edx]
	unaligned_loop_increment:
		add     eax, 16
		movdqu  xmm0, xmmword ptr [eax]
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jz      unaligned_loop_increment
		pmovmskb ecx, xmm1
		test    ecx, ecx
		jz      unaligned_loop

		align   16
	null_is_found:
		xor     edx, ecx
		jz      epilogue
		bsf     ecx, ecx
		add     eax, ecx
		xor     ecx, 15
		shl     edx, cl
		and     edx, 7FFFH
		jz      epilogue
		bsr     edx, edx
		lea     eax, [eax + edx - 15]
		pop     ebx
		ret

		align   16
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
