#include <wchar.h>

#ifndef _M_IX86
wchar_t * __cdecl _wcsichr(const wchar_t *string, wchar_t c)
{
	wchar_t c1, c2;

	c1 = c | ('a' - 'A');
	if ((wchar_t)(c1 - 'a') >= 'z' - 'a' + 1)
		return wcschr(string, c);
	string--;
	do
		if (((c2 = *(++string)) | ('a' - 'A')) == c1)
			return (wchar_t *)string;
	while (c2);
	return NULL;
}
#else
#pragma warning(disable:4414)

extern const wchar_t xmmconst_casebitW[8];
#define casebit xmmconst_casebitW

static wchar_t * __cdecl wcsichrSSE42(const wchar_t *string, wchar_t c);
static wchar_t * __cdecl wcsichrSSE2(const wchar_t *string, wchar_t c);
static wchar_t * __cdecl wcsichr386(const wchar_t *string, wchar_t c);
static wchar_t * __cdecl wcsichrCPUDispatch(const wchar_t *string, wchar_t c);

static wchar_t *(__cdecl *wcsichrDispatch)(const wchar_t *string, wchar_t c) = wcsichrCPUDispatch;

__declspec(naked) wchar_t * __cdecl _wcsichr(const wchar_t *string, wchar_t c)
{
	__asm
	{
		jmp     dword ptr [wcsichrDispatch]
	}
}

// SSE4.2 version
__declspec(naked) static wchar_t * __cdecl wcsichrSSE42(const wchar_t *string, wchar_t c)
{
	extern wchar_t * __cdecl wcschrSSE42(const wchar_t *string, wchar_t c);

	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     eax, dword ptr [c]
		mov     ecx, dword ptr [string]
		or      eax, 'a' - 'A'
		xor     edx, edx
		mov     dx, ax
		sub     eax, 'a'
		cmp     ax, 'z' - 'a' + 1
		jae     wcschrSSE42
		mov     eax, edx
		xor     edx, 'a' - 'A'
		shl     eax, 16
		push    esi
		or      edx, eax
		mov     esi, ecx
		movd    xmm1, edx
		mov     eax, 3
		sub     esi, 16
		and     ecx, 15
		jz      loop_entry
		sub     esi, ecx
		mov     edx, 8
		shr     ecx, 1
		jc      unaligned
		movdqa  xmm0, xmmword ptr [esi + 16]
		lea     esi, [esi + 16]
		jmp     compare

		align   16
	unaligned:
		inc     ecx
		add     esi, 15
		and     ecx, 7
		jz      loop_entry
		movdqa  xmm0, xmmword ptr [esi + 1]
		pslldq  xmm0, 1
	compare:
		pcmpestrm xmm1, xmm0, 00000001B
		jnc     loop_entry
		movd    eax, xmm0
		shr     eax, cl
		jz      loop_entry
		bsf     eax, eax
		add     ecx, eax
		xor     eax, eax
		mov     al, byte ptr [esi + ecx * 2]
		add     eax, -1
		jmp     epilog

		align   16
	loop_entry:
		movlhps xmm1, xmm1

		align   16
	loop_begin:
		pcmpistri xmm1, xmmword ptr [esi + 16], 00000001B
		lea     esi, [esi + 16]
		jnbe    loop_begin

	epilog:
		sbb     edx, edx
		lea     eax, [esi + ecx * 2]
		and     eax, edx
		pop     esi
		ret

		#undef string
		#undef c
	}
}

// SSE2 version
__declspec(naked) static wchar_t * __cdecl wcsichrSSE2(const wchar_t *string, wchar_t c)
{
	extern wchar_t * __cdecl wcschrSSE2(const wchar_t *string, wchar_t c);

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
		cmp     cx, 'z' - 'a' + 1
		jae     wcschrSSE2
		pxor    xmm1, xmm1
		movd    xmm2, edx
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		movdqa  xmm3, xmmword ptr [casebit]
		or      edx, -1
		mov     ecx, eax
		test    eax, 1
		jnz     unaligned
		and     ecx, 15
		jz      aligned_loop_entry
		shl     edx, cl
		sub     eax, ecx
		jmp     aligned_loop_entry

		align   16
	aligned_loop:
		add     eax, 16
		or      edx, -1
	aligned_loop_entry:
		movdqa  xmm0, xmmword ptr [eax]
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb ecx, xmm0
		and     ecx, edx
		jz      aligned_loop
		jmp     found

		align   16
	unaligned:
		inc     ecx
		or      eax, 15
		and     ecx, 15
		jz      unaligned_loop_entry1
		movdqa  xmm0, xmmword ptr [eax - 15]
		pslldq  xmm0, 1
		shl     edx, cl
		sub     eax, 16
		jmp     unaligned_loop_entry2

		align   16
	unaligned_loop:
		add     eax, 16
		or      edx, -1
	unaligned_loop_entry1:
		movdqu  xmm0, xmmword ptr [eax]
	unaligned_loop_entry2:
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb ecx, xmm0
		and     ecx, edx
		jz      unaligned_loop

		align   16
	found:
		bsf     ecx, ecx
		mov     dx, word ptr [eax + ecx]
		add     eax, ecx
		xor     ecx, ecx
		test    dx, dx
		cmovz   eax, ecx
		ret

		#undef string
		#undef c
	}
}

// 80386 version
__declspec(naked) static wchar_t * __cdecl wcsichr386(const wchar_t *string, wchar_t c)
{
	extern wchar_t * __cdecl wcschr386(const wchar_t *string, wchar_t c);

	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     ecx, dword ptr [c]
		mov     eax, dword ptr [string]
		or      ecx, 'a' - 'A'
		sub     eax, 2
		mov     edx, ecx
		sub     ecx, 'a'
		cmp     cx, 'z' - 'a' + 1
		jae     wcschr386
		push    ebx

		align   16
	main_loop:
		mov     cx, word ptr [eax + 2]
		add     eax, 2
		mov     bx, cx
		or      cx, 'a' - 'A'
		cmp     cx, dx
		je      epilog
		test    bx, bx
		jnz     main_loop
		xor     eax, eax
	epilog:
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static wchar_t * __cdecl wcsichrCPUDispatch(const wchar_t *string, wchar_t c)
{
	#define __ISA_AVAILABLE_X86   0
	#define __ISA_AVAILABLE_SSE2  1
	#define __ISA_AVAILABLE_SSE42 2

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_SSE2
		jbe     L1
		mov     dword ptr [wcsichrDispatch], offset wcsichrSSE42
		jmp     wcsichrSSE42

	L1:
		mov     dword ptr [wcsichrDispatch], offset wcsichrSSE2
		jb      L2
		jmp     wcsichrSSE2

	L2:
		mov     dword ptr [wcsichrDispatch], offset wcsichr386
		jmp     wcsichr386
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
}
#endif
