#include <wchar.h>

#ifndef _M_IX86
wchar_t * __cdecl _wcsrichr(const wchar_t *string, wchar_t c)
{
	wchar_t *p, c1, c2;

	c1 = c | ('a' - 'A');
	if ((wchar_t)(c1 - 'a') >= 'z' - 'a' + 1)
		return wcsrchr(string, c);
	string--;
	p = NULL;
	while (c2 = *(++string))
		if ((c2 | ('a' - 'A')) == c1)
			p = (wchar_t *)string;
	return p;
}
#else
#pragma warning(disable:4414)

extern const wchar_t xmmconst_casebitW[8];
extern const char    xmmconst_maskbit[32];
#define casebit xmmconst_casebitW
#define maskbit xmmconst_maskbit

static wchar_t * __cdecl wcsrichrSSE42(const wchar_t *string, wchar_t c);
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

// SSE4.2 version
__declspec(naked) static wchar_t * __cdecl wcsrichrSSE42(const wchar_t *string, wchar_t c)
{
	extern wchar_t * __cdecl wcsrchrSSE42(const wchar_t *string, wchar_t c);

	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     ecx, dword ptr [c]
		mov     edx, dword ptr [string]
		or      ecx, 'a' - 'A'
		xor     eax, eax
		mov     ax, cx
		sub     edx, 'a'
		cmp     cx, 'z' - 'a' + 1
		jae     wcsrchrSSE42
		movd    xmm0, ecx
		pshuflw xmm0, xmm0, 0
		movlhps xmm0, xmm0
		mov     eax, edx
		sub     edx, 16
		and     eax, 15
		jz      loop_begin
		test    eax, 1
		jnz     unaligned
		sub     edx, eax
		xor     eax, 15
		movdqa  xmm1, xmmword ptr [edx + 16]
		movdqu  xmm2, xmmword ptr [maskbit + eax + 1]
		add     edx, 16
		jmp     compare

		align   16
	unaligned:
		xor     eax, 15
		jz      loop_begin
		and     edx, -16
		movdqa  xmm1, xmmword ptr [edx + 16]
		movdqu  xmm2, xmmword ptr [maskbit + eax]
		pslldq  xmm1, 1
		add     edx, 15
	compare:
		xor     eax, eax
		pcmpeqb xmm3, xmm3
		movdqa  xmm4, xmm2
		pxor    xmm3, xmm2
		pand    xmm4, xmm0
		pand    xmm1, xmm3
		por     xmm1, xmm4
		paddb   xmm1, xmm2
		pcmpistri xmm0, xmm1, 01000001B
		jbe     loop_found

		align   16
	loop_begin:
		pcmpistri xmm0, xmmword ptr [edx + 16], 01000001B
		lea     edx, [edx + 16]
		jnbe    loop_begin
		jnc     epilog
	loop_found:
		lea     eax, [edx + ecx * 2]
		jnz     loop_begin
	epilog:
		ret

		#undef string
		#undef c
	}
}

// SSE2 version
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
		cmp     dx, 'z' - 'a' + 1
		jae     wcsrchrSSE2
		push    ebx
		push    esi
		push    edi
		mov     edx, eax
		or      edi, -1
		pxor    xmm1, xmm1
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
		jz      epilog
		bsr     edx, esi
	return_pointer:
		lea     eax, [eax + edx - 1]
	epilog:
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

// 80386 version
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
		cmp     ax, 'z' - 'a' + 1
		jae     wcsrchr386
		push    esi
		xor     esi, esi

		align   16
	main_loop:
		mov     ax, word ptr [ecx + 2]
		add     ecx, 2
		test    ax, ax
		jz      null_found
		or      ax, 'a' - 'A'
		cmp     ax, dx
		jne     main_loop
		mov     esi, ecx
		jmp     main_loop

		align   16
	null_found:
		mov     eax, esi
		pop     esi
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static wchar_t * __cdecl wcsrichrCPUDispatch(const wchar_t *string, wchar_t c)
{
	#define __ISA_AVAILABLE_X86   0
	#define __ISA_AVAILABLE_SSE2  1
	#define __ISA_AVAILABLE_SSE42 2

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_SSE2
		jbe     L1
		mov     dword ptr [wcsrichrDispatch], offset wcsrichrSSE42
		jmp     wcsrichrSSE42

	L1:
		mov     dword ptr [wcsrichrDispatch], offset wcsrichrSSE2
		jb      L2
		jmp     wcsrichrSSE2

	L2:
		mov     dword ptr [wcsrichrDispatch], offset wcsrichr386
		jmp     wcsrichr386
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
}
#endif
