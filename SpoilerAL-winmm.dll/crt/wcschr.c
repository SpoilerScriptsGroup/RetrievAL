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

wchar_t * __cdecl wcschrSSE42(const wchar_t *string, wchar_t c);
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

// SSE4.2 version
__declspec(naked) wchar_t * __cdecl wcschrSSE42(const wchar_t *string, wchar_t c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		movzx   edx, word ptr [c]
		mov     ecx, dword ptr [string]
		test    edx, edx
		jz      char_is_null
		movd    xmm1, edx
		push    esi
		mov     esi, ecx
		mov     eax, 2
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
	char_is_null:
		push    ecx
		push    ecx
		call    wcslen
		pop     edx
		pop     ecx
		lea     eax, [ecx + eax * 2]
		ret

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
		pshuflw xmm1, xmm1, 0
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
		pxor    xmm1, xmm1
		movd    xmm2, edx
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
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
		je      epilog
		test    cx, cx
		jnz     main_loop
		xor     eax, eax
	epilog:
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
	#define __ISA_AVAILABLE_X86   0
	#define __ISA_AVAILABLE_SSE2  1
	#define __ISA_AVAILABLE_SSE42 2

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_SSE2
		jbe     L1
		mov     dword ptr [wcschrDispatch], offset wcschrSSE42
		jmp     wcschrSSE42

	L1:
		mov     dword ptr [wcschrDispatch], offset wcschrSSE2
		jb      L2
		jmp     wcschrSSE2

	L2:
		mov     dword ptr [wcschrDispatch], offset wcschr386
		jmp     wcschr386
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
}
#endif
