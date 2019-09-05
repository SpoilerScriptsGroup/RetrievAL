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

		mov     cx, word ptr [c]
		mov     eax, dword ptr [string]
		or      cx, 'a' - 'A'
		xor     edx, edx
		mov     dx, cx
		sub     cx, 'a'
		cmp     cx, 'z' - 'a'
		ja      wcschr
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
		or      edx, -1
		shl     edx, cl
		movdqa  xmm0, xmmword ptr [eax]
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb ecx, xmm0
		and     edx, ecx
		jnz     epilogue
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
		jmp     epilogue

		align   16
	unaligned:
		mov     ecx, eax
		and     eax, -16
		inc     ecx
		dec     eax
		and     ecx, 15
		jz      unaligned_loop_entry
		or      edx, -1
		shl     edx, cl
		movdqa  xmm0, xmmword ptr [eax + 1]
		pslldq  xmm0, 1
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb ecx, xmm0
		and     edx, ecx
		jnz     epilogue
		pxor    xmm1, xmm1

		align   16
	unaligned_loop:
		add     eax, 16
	unaligned_loop_entry:
		movdqu  xmm0, xmmword ptr [eax]
		pcmpeqw xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jz      unaligned_loop

		align   16
	epilogue:
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
