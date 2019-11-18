#include <string.h>

#pragma warning(disable:4414)

#ifndef _M_IX86
wchar_t * __cdecl _wcsichr(const wchar_t *string, wint_t c)
{
	wchar_t c1, c2;

	c1 = c | ('a' - 'A');
	if ((unsigned)(c1 - 'a') > 'z' - 'a')
		return wcschr(string, c);
	do
		if (((c2 = *(string++)) | ('a' - 'A')) == c1)
			return (wchar_t *)string - 1;
	while (c2);
	return NULL;
}
#else
extern const wchar_t xmmconst_casebitW[8];
#define casebit xmmconst_casebitW

static wchar_t * __cdecl wcsichrSSE2(const wchar_t *string, wint_t c);
static wchar_t * __cdecl wcsichr386(const wchar_t *string, wint_t c);
static wchar_t * __cdecl wcsichrCPUDispatch(const wchar_t *string, wint_t c);

static wchar_t *(__cdecl * wcsichrDispatch)(const wchar_t *string, wint_t c) = wcsichrCPUDispatch;

__declspec(naked) wchar_t * __cdecl _wcsichr(const wchar_t *string, wint_t c)
{
	__asm
	{
		jmp     dword ptr [wcsichrDispatch]
	}
}

__declspec(naked) static wchar_t * __cdecl wcsichrSSE2(const wchar_t *string, wint_t c)
{
	extern wchar_t * __cdecl wcschrSSE2(const wchar_t *string, wint_t c);

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
		ja      wcschrSSE2
		pxor    xmm1, xmm1
		movd    xmm2, edx
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		movdqa  xmm3, xmmword ptr [casebit]
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
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb ecx, xmm0
		and     edx, ecx
		jnz     found
		pxor    xmm1, xmm1

		align   16
	aligned_loop:
		add     eax, 16
	aligned_loop_entry:
		movdqa  xmm0, xmmword ptr [eax]
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jz      aligned_loop
		jmp     found

		align   16
	unaligned:
		lea     ecx, [eax + 1]
		and     eax, -16
		or      edx, -1
		dec     eax
		and     ecx, 15
		jz      unaligned_loop
		shl     edx, cl
		movdqa  xmm0, xmmword ptr [eax + 1]
		pslldq  xmm0, 1
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb ecx, xmm0
		and     edx, ecx
		jnz     found
		pxor    xmm1, xmm1

		align   16
	unaligned_loop:
		add     eax, 16
		movdqu  xmm0, xmmword ptr [eax]
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jz      unaligned_loop

		align   16
	found:
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

__declspec(naked) static wchar_t * __cdecl wcsichr386(const wchar_t *string, wint_t c)
{
	extern wchar_t * __cdecl wcschr386(const wchar_t *string, wint_t c);

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
		cmp     cx, 'z' - 'a'
		ja      wcschr386
		push    ebx

		align   16
	main_loop:
		mov     cx, word ptr [eax + 2]
		add     eax, 2
		mov     bx, cx
		or      cx, 'a' - 'A'
		cmp     cx, dx
		je      epilogue
		test    bx, bx
		jnz     main_loop
		xor     eax, eax
	epilogue:
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static wchar_t * __cdecl wcsichrCPUDispatch(const wchar_t *string, wint_t c)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wcsichrDispatch], offset wcsichr386
		jmp     wcsichr386
	L1:
		mov     dword ptr [wcsichrDispatch], offset wcsichrSSE2
		jmp     wcsichrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
