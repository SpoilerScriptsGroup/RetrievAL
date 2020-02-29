#include <string.h>

#pragma warning(disable:4414)

#ifndef _M_IX86
wchar_t * __cdecl _wcsrichr(const wchar_t *string, wchar_t c)
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

static wchar_t * __cdecl wcsrichrSSE2(const wchar_t *string, wchar_t c);
static wchar_t * __cdecl wcsrichr386(const wchar_t *string, wchar_t c);
static wchar_t * __cdecl wcsrichrCPUDispatch(const wchar_t *string, wchar_t c);

static wchar_t *(__cdecl *wcsrichrDispatch)(const wchar_t *string, wchar_t c) = wcsrichrCPUDispatch;

__declspec(naked) wchar_t * __cdecl _wcsrichr(const wchar_t *string, wchar_t c)
{
	__asm
	{
		jmp     dword ptr [wcsrichrDispatch]
	}
}

__declspec(naked) static wchar_t * __cdecl wcsrichrSSE2(const wchar_t *string, wchar_t c)
{
	extern wchar_t * __cdecl wcsrchrSSE2(const wchar_t *string, wchar_t c);

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
		ja      wcsrchrSSE2
		push    ebx
		push    esi
		push    edi
		mov     edx, eax
		or      edi, -1
		movd    xmm2, ecx
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		movdqa  xmm3, xmmword ptr [casebit]
		mov     ecx, eax
		or      edi, -1
		and     eax, 1
		jnz     unaligned
		and     ecx, 15
		jz      aligned_loop_entry
		shl     edi, cl
		sub     edx, ecx
		jmp     aligned_loop_entry

		align   16
	aligned_loop:
		and     ebx, edi
		jz      aligned_loop_increment
		mov     eax, edx
		mov     esi, ebx
	aligned_loop_increment:
		add     edx, 16
		or      edi, -1
	aligned_loop_entry:
		movdqa  xmm0, xmmword ptr [edx]
		pxor    xmm1, xmm1
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		pmovmskb ecx, xmm1
		pmovmskb ebx, xmm0
		and     ecx, edi
		jz      aligned_loop
		jmp     null_is_found

		align   16
	unaligned:
		inc     ecx
		and     edx, -16
		dec     eax
		dec     edx
		and     ecx, 15
		jz      unaligned_loop_increment
		movdqa  xmm0, xmmword ptr [edx + 1]
		pslldq  xmm0, 1
		shl     edi, cl
		jmp     unaligned_loop_entry

		align   16
	unaligned_loop:
		and     ebx, edi
		jz      unaligned_loop_increment
		mov     eax, edx
		mov     esi, ebx
	unaligned_loop_increment:
		add     edx, 16
		or      edi, -1
		movdqu  xmm0, xmmword ptr [edx]
	unaligned_loop_entry:
		pxor    xmm1, xmm1
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		pmovmskb ecx, xmm1
		pmovmskb ebx, xmm0
		and     ecx, edi
		jz      unaligned_loop

		align   16
	null_is_found:
		bsf     ecx, ecx
		and     ebx, edi
		xor     ecx, 15
		shl     ebx, cl
		and     ebx, 7FFFH
		jz      process_stored_pointer
		bsr     eax, ebx
		sub     edx, ecx
		jmp     return_pointer

		align   16
	process_stored_pointer:
		test    eax, eax
		jz      epilogue
		bsr     edx, esi
	return_pointer:
		lea     eax, [eax + edx - 1]
	epilogue:
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static wchar_t * __cdecl wcsrichr386(const wchar_t *string, wchar_t c)
{
	extern wchar_t * __cdecl wcsrchr386(const wchar_t *string, wchar_t c);

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
		ja      wcsrchr386
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

__declspec(naked) static wchar_t * __cdecl wcsrichrCPUDispatch(const wchar_t *string, wchar_t c)
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
