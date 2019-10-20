#include <windows.h>

#ifndef _M_IX86
wchar_t * __cdecl wcsrchr(const wchar_t *string, wint_t c)
{
	wchar_t *p, c2;

	if (!c)
		return (wchar_t *)string + wcslen(string);
	p = NULL;
	while (c2 = *(string++))
		if (c2 == c)
			p = (wchar_t *)string - 1;
	return p;
}
#else
#pragma function(wcslen)

static wchar_t * __cdecl wcsrchrSSE2(const wchar_t *string, wint_t c);
static wchar_t * __cdecl wcsrchr386(const wchar_t *string, wint_t c);
static wchar_t * __cdecl wcsrchrCPUDispatch(const wchar_t *string, wint_t c);

static wchar_t *(__cdecl * wcsrchrDispatch)(const wchar_t *string, wint_t c) = wcsrchrCPUDispatch;

__declspec(naked) wchar_t * __cdecl wcsrchr(const wchar_t *string, wint_t c)
{
	__asm
	{
		jmp     dword ptr [wcsrchrDispatch]
	}
}

__declspec(naked) static wchar_t * __cdecl wcsrchrSSE2(const wchar_t *string, wint_t c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     ecx, dword ptr [c]
		mov     eax, dword ptr [string]
		test    cx, cx
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
		push    ebx
		push    esi
		mov     edx, eax
		or      esi, -1
		pxor    xmm1, xmm1
		movd    xmm2, ecx
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		and     eax, 1
		jnz     unaligned
		mov     ecx, edx
		and     edx, -16
		and     ecx, 15
		jz      aligned_loop_entry
		shl     esi, cl
		movdqa  xmm0, xmmword ptr [edx]
		pcmpeqw xmm1, xmm0
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

__declspec(naked) static wchar_t * __cdecl wcsrchr386(const wchar_t *string, wint_t c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     edx, dword ptr [c]
		mov     ecx, dword ptr [string]
		test    dx, dx
		jz      char_is_null
		push    ebx
		sub     ecx, 2
		xor     ebx, ebx

		align   16
	main_loop:
		mov     ax, word ptr [ecx + 2]
		add     ecx, 2
		cmp     ax, dx
		jne     is_null
		mov     ebx, ecx
		jmp     main_loop
	is_null:
		test    ax, ax
		jnz     main_loop
		mov     eax, ebx
		pop     ebx
		ret

		align   16
	char_is_null:
		push    ecx
		push    ecx
		call    wcslen
		pop     edx
		pop     ecx
		lea     eax, [ecx + eax * 2]
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static wchar_t * __cdecl wcsrchrCPUDispatch(const wchar_t *string, wint_t c)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wcsrchrDispatch], offset wcsrchr386
		jmp     wcsrchr386
	L1:
		mov     dword ptr [wcsrchrDispatch], offset wcsrchrSSE2
		jmp     wcsrchrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
