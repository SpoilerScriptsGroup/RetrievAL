#include <wchar.h>
#include "PageSize.h"

#pragma warning(disable:4163)
#pragma function(wcscmp)

#ifndef _M_IX86
int __cdecl wcscmp(const wchar_t *string1, const wchar_t *string2)
{
	unsigned short c1, c2;

	do
		if ((c1 = *(string1++)) != (c2 = *(string2++)))
			return (int)c1 - (int)c2;
	while (c1);
	return 0;
}
#else
static int __cdecl wcscmpSSE2(const wchar_t *string1, const wchar_t *string2);
static int __cdecl wcscmp386(const wchar_t *string1, const wchar_t *string2);
static int __cdecl wcscmpCPUDispatch(const wchar_t *string1, const wchar_t *string2);

static int(__cdecl * wcscmpDispatch)(const wchar_t *string1, const wchar_t *string2) = wcscmpCPUDispatch;

__declspec(naked) int __cdecl wcscmp(const wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		jmp     dword ptr [wcscmpDispatch]
	}
}

__declspec(naked) static int __cdecl wcscmpSSE2(const wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 8]            // esi = string1
		mov     edi, dword ptr [string2 + 8]            // edi = string2
		lea     edx, [edi + 1]                          // edx = (size_t)string2 + 1
		sub     edi, esi                                // edi = (size_t)string2 - (size_t)string1
		pxor    xmm2, xmm2
		jmp     word_loop_entry

		align   16
	word_loop:
		movzx   eax, word ptr [esi]
		movzx   edx, word ptr [esi + edi]
		sub     eax, edx
		jnz     epilogue
		test    edx, edx
		jz      epilogue
		lea     edx, [esi + edi + 3]
		add     esi, 2
	word_loop_entry:
		and     edx, 14
		jnz     word_loop
		mov     ecx, esi
		lea     edx, [esi + edi]
		and     ecx, PAGE_SIZE - 1
		and     edx, 1
		jnz     unaligned_xmmword_loop

		align   16
	aligned_xmmword_loop:
		cmp     ecx, PAGE_SIZE - 16
		ja      word_loop                               // jump if cross pages
		movdqu  xmm0, xmmword ptr [esi]
		movdqa  xmm1, xmmword ptr [esi + edi]
		pcmpeqw xmm0, xmm1
		pcmpeqw xmm1, xmm2
		pmovmskb eax, xmm0
		pmovmskb ecx, xmm1
		xor     eax, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     epilogue
		lea     ecx, [esi + 16]
		add     esi, 16
		and     ecx, PAGE_SIZE - 1
		jmp     aligned_xmmword_loop

		align   16
	unaligned_xmmword_loop:
		cmp     ecx, PAGE_SIZE - 16
		ja      word_loop                               // jump if cross pages
		movdqu  xmm0, xmmword ptr [esi]
		movdqu  xmm1, xmmword ptr [esi + edi]
		pcmpeqw xmm0, xmm1
		pcmpeqw xmm1, xmm2
		pmovmskb eax, xmm0
		pmovmskb ecx, xmm1
		xor     eax, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     epilogue
		lea     ecx, [esi + 16]
		add     esi, 16
		and     ecx, PAGE_SIZE - 1
		jmp     unaligned_xmmword_loop

		align   16
	xmmword_not_equal:
		test    ecx, ecx
		jz      xmmword_has_not_null
		bsf     ecx, ecx
		mov     edx, 0FFFFH
		xor     ecx, 15
		shr     edx, cl
		and     eax, edx
		jz      epilogue
	xmmword_has_not_null:
		bsf     eax, eax
		add     esi, eax
		movzx   eax, word ptr [esi]
		movzx   edx, word ptr [esi + edi]
		sub     eax, edx
	epilogue:
		pop     edi
		pop     esi
		ret

		#undef string1
		#undef string2
	}
}

__declspec(naked) static int __cdecl wcscmp386(const wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    ebx
		xor     eax, eax                                // eax = 0
		mov     ecx, dword ptr [string1 + 4]            // ecx = string1
		mov     edx, dword ptr [string2 + 4]            // edx = string2
		xor     ebx, ebx                                // ebx = 0

		align   16
	loop_begin:
		mov     ax, word ptr [ecx]
		mov     bx, word ptr [edx]
		add     ecx, 2
		add     edx, 2
		cmp     ax, bx
		jne     epilogue
		test    ax, ax
		jnz     loop_begin
	epilogue:
		sub     eax, ebx
		pop     ebx
		ret

		#undef string1
		#undef string2
	}
}

__declspec(naked) static int __cdecl wcscmpCPUDispatch(const wchar_t *string1, const wchar_t *string2)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wcscmpDispatch], offset wcscmp386
		jmp     wcscmp386

	L1:
		mov     dword ptr [wcscmpDispatch], offset wcscmpSSE2
		jmp     wcscmpSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
