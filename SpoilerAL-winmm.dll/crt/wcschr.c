#include <wchar.h>

#ifndef _M_IX86
wchar_t * __cdecl wcschr(const wchar_t *string, wchar_t c)
{
	wchar_t c2;

	if (!c)
		return (wchar_t *)string + wcslen(string);
	string--;
	do
		if ((c2 = *(++string)) == c)
			return (wchar_t *)string;
	while (c2);
	return NULL;
}
#else
#pragma function(wcslen)

wchar_t * __cdecl wcschrSSE2(const wchar_t *string, wchar_t c);
wchar_t * __cdecl wcschr386(const wchar_t *string, wchar_t c);
static wchar_t * __cdecl wcschrCPUDispatch(const wchar_t *string, wchar_t c);

static wchar_t *(__cdecl *wcschrDispatch)(const wchar_t *string, wchar_t c) = wcschrCPUDispatch;

__declspec(naked) wchar_t * __cdecl wcschr(const wchar_t *string, wchar_t c)
{
	__asm
	{
		jmp     dword ptr [wcschrDispatch]
	}
}

__declspec(naked) wchar_t * __cdecl wcschrSSE2(const wchar_t *string, wchar_t c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     edx, dword ptr [c]
		mov     eax, dword ptr [string]
		test    dx, dx
		jnz     char_is_not_null
		push    eax
		push    eax
		call    wcslen
		pop     edx
		pop     ecx
		lea     eax, [ecx + eax * 2]
		ret

		align   16
	char_is_not_null:
		movd    xmm2, edx
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		test    eax, 1
		jnz     unaligned
		mov     ecx, eax
		or      edx, -1
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
		pxor    xmm1, xmm1
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb ecx, xmm0
		and     ecx, edx
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
		movdqa  xmm0, xmmword ptr [eax + 1]
		pslldq  xmm0, 1
		shl     edx, cl
		jmp     unaligned_loop_entry

		align   16
	unaligned_loop:
		add     eax, 16
		or      edx, -1
		movdqu  xmm0, xmmword ptr [eax]
	unaligned_loop_entry:
		pxor    xmm1, xmm1
		pcmpeqw xmm1, xmm0
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

__declspec(naked) wchar_t * __cdecl wcschr386(const wchar_t *string, wchar_t c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     edx, dword ptr [c]
		mov     eax, dword ptr [string]
		test    dx, dx
		jz      char_is_null
		sub     eax, 2

		align   16
	main_loop:
		mov     cx, word ptr [eax + 2]
		add     eax, 2
		cmp     cx, dx
		je      epilogue
		test    cx, cx
		jnz     main_loop
		xor     eax, eax
	epilogue:
		ret

		align   16
	char_is_null:
		push    eax
		push    eax
		call    wcslen
		pop     edx
		pop     ecx
		lea     eax, [ecx + eax * 2]
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static wchar_t * __cdecl wcschrCPUDispatch(const wchar_t *string, wchar_t c)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wcschrDispatch], offset wcschr386
		jmp     wcschr386

	L1:
		mov     dword ptr [wcschrDispatch], offset wcschrSSE2
		jmp     wcschrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
