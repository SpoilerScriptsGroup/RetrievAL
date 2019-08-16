#include <string.h>

#pragma warning(disable:4414)

#ifndef _M_IX86
wchar_t * __cdecl _wcsichr(const wchar_t *string, wint_t c)
{
	const wchar_t *p;
	wchar_t       c1, c2;

	c1 = c | ('a' - 'A');
	if (c1 - 'a' > 'z' - 'a')
		return wcschr(string, c);
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
		ja      wcschr
		movd    xmm3, cx
		pshuflw xmm3, xmm3, 0
		movlhps xmm3, xmm3
		movdqa  xmm4, xmm3
		psubw   xmm4, xmmword ptr [casebitW]
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
		jnz     epilogue

		align   16
	aligned_loop:
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
		jz      aligned_loop
		jmp     epilogue

		align   16
	unaligned:
		inc     ecx
		or      eax, -1
		and     ecx, 15
		jz      unaligned_loop
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
		jnz     epilogue

		align   16
	unaligned_loop:
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
		jz      unaligned_loop

		align   16
	epilogue:
		bsf     eax, eax
		mov     cx, word ptr [edx + eax]
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
		ja      wcschr

		align   16
	L1:
		mov     cx, word ptr [eax + 2]
		add     eax, 2
		test    cx, cx
		jz      L2
		or      cx, 'a' - 'A'
		cmp     cx, dx
		jne     L1
		ret

		align   16
	L2:
		xor     eax, eax
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
