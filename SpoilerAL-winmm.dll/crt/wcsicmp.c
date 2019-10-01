#include <wchar.h>
#include "PageSize.h"

#ifndef _M_IX86
int __cdecl _wcsicmp(const wchar_t *string1, const wchar_t *string2)
{
	int ret, c;

	do
		if (ret = tolower(*(string1++)) - (c = tolower(*(string2++))))
			return ret;
	while (c);
	return 0;
}
#else
static int __cdecl wcsicmpSSE2(const wchar_t *string1, const wchar_t *string2);
static int __cdecl wcsicmp386(const wchar_t *string1, const wchar_t *string2);
static int __cdecl wcsicmpCPUDispatch(const wchar_t *string1, const wchar_t *string2);

static int(__cdecl * wcsicmpDispatch)(const wchar_t *string1, const wchar_t *string2) = wcsicmpCPUDispatch;

extern const wchar_t xmmconst_ahighW[8];
extern const wchar_t xmmconst_azrangeW[8];
extern const wchar_t xmmconst_casebitW[8];
#define ahigh   xmmconst_ahighW
#define azrange xmmconst_azrangeW
#define casebit xmmconst_casebitW

__declspec(naked) int __cdecl _wcsicmp(const wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		jmp     dword ptr [wcsicmpDispatch]
	}
}

__declspec(naked) static int __cdecl wcsicmpSSE2(const wchar_t *string1, const wchar_t *string2)
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
		movdqa  xmm4, xmmword ptr [ahigh]
		movdqa  xmm5, xmmword ptr [azrange]
		pxor    xmm6, xmm6                              // set to zero
		movdqa  xmm7, xmmword ptr [casebit]             // bit to change
		jmp     word_loop_entry

		align   16
	word_loop:
		movzx   eax, word ptr [esi]
		movzx   edx, word ptr [esi + edi]
		sub     eax, 'A'
		sub     edx, 'A'
		cmp     eax, 'Z' - 'A' + 1
		lea     ecx, [eax + 'a' - 'A']
		cmovb   eax, ecx
		cmp     edx, 'Z' - 'A' + 1
		lea     ecx, [edx + 'a' - 'A']
		cmovb   edx, ecx
		sub     eax, edx
		jnz     epilogue
		cmp     edx, -'A'
		je      epilogue
		lea     edx, [esi + edi + 3]
		add     esi, 2
	word_loop_entry:
		and     edx, 14
		jnz     word_loop
		mov     ecx, esi
		mov     edx, edi
		and     ecx, PAGE_SIZE - 1
		and     edx, 1
		jnz     unaligned_xmmword_loop

		align   16
	aligned_xmmword_loop:
		cmp     ecx, PAGE_SIZE - 16
		ja      word_loop                               // jump if cross pages
		movdqu  xmm3, xmmword ptr [esi]                 // load 16 byte
		movdqa  xmm1, xmmword ptr [esi + edi]           //
		movdqa  xmm0, xmm3                              // copy
		pcmpeqw xmm3, xmm6                              // compare 8 words with zero
		movdqa  xmm2, xmm0                              // copy
		pmovmskb ecx, xmm3                              // get one bit for each byte result
		movdqa  xmm3, xmm1                              // copy
		psubw   xmm0, xmm4                              // all words less than 'A'
		psubw   xmm1, xmm4                              //
		psubusw xmm0, xmm5                              // and 'Z' will be reset
		psubusw xmm1, xmm5                              //
		pcmpeqw xmm0, xmm6                              // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		pcmpeqw xmm1, xmm6                              //
		pand    xmm0, xmm7                              // assign a mask for the appropriate words
		pand    xmm1, xmm7                              //
		pxor    xmm0, xmm2                              // negation of the 5th bit - lowercase letters
		pxor    xmm1, xmm3                              //
		pcmpeqw xmm0, xmm1                              // compare
		pmovmskb eax, xmm0                              // get one bit for each byte result
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
		movdqu  xmm3, xmmword ptr [esi]                 // load 16 byte
		movdqu  xmm1, xmmword ptr [esi + edi]           //
		movdqa  xmm0, xmm3                              // copy
		pcmpeqw xmm3, xmm6                              // compare 8 words with zero
		movdqa  xmm2, xmm0                              // copy
		pmovmskb ecx, xmm3                              // get one bit for each byte result
		movdqa  xmm3, xmm1                              // copy
		psubw   xmm0, xmm4                              // all words less than 'A'
		psubw   xmm1, xmm4                              //
		psubusw xmm0, xmm5                              // and 'Z' will be reset
		psubusw xmm1, xmm5                              //
		pcmpeqw xmm0, xmm6                              // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		pcmpeqw xmm1, xmm6                              //
		pand    xmm0, xmm7                              // assign a mask for the appropriate words
		pand    xmm1, xmm7                              //
		pxor    xmm0, xmm2                              // negation of the 5th bit - lowercase letters
		pxor    xmm1, xmm3                              //
		pcmpeqw xmm0, xmm1                              // compare
		pmovmskb eax, xmm0                              // get one bit for each byte result
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
		sub     eax, 'A'
		sub     edx, 'A'
		cmp     eax, 'Z' - 'A' + 1
		lea     ecx, [eax + 'a' - 'A']
		cmovb   eax, ecx
		cmp     edx, 'Z' - 'A' + 1
		lea     ecx, [edx + 'a' - 'A']
		cmovb   edx, ecx
		sub     eax, edx
	epilogue:
		pop     edi
		pop     esi
		ret

		#undef string1
		#undef string2
	}
}

__declspec(naked) static int __cdecl wcsicmp386(const wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    ebx
		xor     eax, eax                                // eax = 0
		mov     ecx, dword ptr [string1 + 4]            // ecx = string1
		mov     ebx, dword ptr [string2 + 4]            // ebx = string2
		sub     ebx, ecx                                // ebx = (size_t)string2 - (size_t)string1

		align   16
	loop_begin:
		xor     edx, edx
		mov     ax, word ptr [ecx]
		mov     dx, word ptr [ecx + ebx]
		add     ecx, 2
		sub     eax, edx
		jnz     compare_insensitive
		test    edx, edx
		jnz     loop_begin
		pop     ebx
		ret

		align   16
	compare_insensitive:
		cmp     eax, 'a' - 'A'
		je      compare_above
		cmp     eax, 'A' - 'a'
		jne     return_not_equal
		sub     edx, 'a'
		xor     eax, eax
		cmp     edx, 'z' - 'a'
		jbe     loop_begin
		dec     eax
		pop     ebx
		ret

		align   16
	compare_above:
		sub     edx, 'A'
		xor     eax, eax
		cmp     edx, 'Z' - 'A'
		jbe     loop_begin
		inc     eax
		pop     ebx
		ret

		align   16
	return_not_equal:
		lea     eax, [eax + edx - 'A']
		sub     edx, 'A'
		cmp     eax, 'Z' - 'A'
		ja      secondary_to_lower
		add     eax, 'a' - 'A'
	secondary_to_lower:
		cmp     edx, 'Z' - 'A'
		ja      difference
		add     edx, 'a' - 'A'
	difference:
		sub     eax, edx
		pop     ebx
		ret

		#undef string1
		#undef string2
	}
}

__declspec(naked) static int __cdecl wcsicmpCPUDispatch(const wchar_t *string1, const wchar_t *string2)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wcsicmpDispatch], offset wcsicmp386
		jmp     wcsicmp386
	L1:
		mov     dword ptr [wcsicmpDispatch], offset wcsicmpSSE2
		jmp     wcsicmpSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
