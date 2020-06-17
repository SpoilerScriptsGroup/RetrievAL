#include <wchar.h>
#ifndef _M_IX86

int __cdecl _wcsicmp(const wchar_t *string1, const wchar_t *string2)
{
	int ret, c;

	do
		if (ret = towlower(*(string1++)) - (c = towlower(*(string2++))))
			break;
	while (c);
	return ret;
}
#else
#include "PageSize.h"

static int __cdecl wcsicmpSSE2(const wchar_t *string1, const wchar_t *string2);
static int __cdecl wcsicmp386(const wchar_t *string1, const wchar_t *string2);
static int __cdecl wcsicmpCPUDispatch(const wchar_t *string1, const wchar_t *string2);

static int(__cdecl * wcsicmpDispatch)(const wchar_t *string1, const wchar_t *string2) = wcsicmpCPUDispatch;

extern const wchar_t xmmconst_upperW[8];
extern const wchar_t xmmconst_azrangeW[8];
extern const wchar_t xmmconst_casebitW[8];
#define upper   xmmconst_upperW
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
		mov     esi, dword ptr [string1 + 8]                // esi = string1
		mov     edi, dword ptr [string2 + 8]                // edi = string2
		lea     edx, [edi + 1]                              // edx = (size_t)string2 + 1
		sub     esi, edi                                    // edi = (size_t)string1 - (size_t)string2
		movdqa  xmm4, xmmword ptr [upper]
		movdqa  xmm5, xmmword ptr [azrange]
		movdqa  xmm6, xmmword ptr [casebit]                 // bit to change
		pxor    xmm7, xmm7                                  // set to zero
		jmp     word_loop_entry

		align   16
	word_loop:
		movzx   eax, word ptr [esi + edi]
		movzx   edx, word ptr [edi]
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
		cmp     edx, '\0' - 'A'
		je      epilogue
		lea     edx, [edi + 3]
		add     edi, 2
	word_loop_entry:
		and     edx, 14
		jnz     word_loop
		lea     edx, [esi + edi]
		mov     ecx, edi
		and     edx, PAGE_SIZE - 1
		and     ecx, 1
		jnz     unaligned_xmmword_loop

		align   16
	aligned_xmmword_loop:
		cmp     edx, PAGE_SIZE - 16
		ja      word_loop                                   // jump if cross pages
		movdqu  xmm0, xmmword ptr [esi + edi]               // load 16 byte
		movdqa  xmm1, xmmword ptr [edi]                     //
		movdqa  xmm2, xmm0                                  // copy
		movdqa  xmm3, xmm1                                  //
		paddw   xmm0, xmm4                                  // all words greater than 'Z' if negative
		paddw   xmm1, xmm4                                  //
		pcmpgtw xmm0, xmm5                                  // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		pcmpgtw xmm1, xmm5                                  //
		pand    xmm0, xmm6                                  // assign a mask for the appropriate words
		pand    xmm1, xmm6                                  //
		por     xmm0, xmm2                                  // negation of the 5th bit - lowercase letters
		por     xmm1, xmm3                                  //
		pcmpeqw xmm0, xmm1                                  // compare
		pcmpeqw xmm2, xmm7                                  //
		pmovmskb eax, xmm0                                  // get one bit for each byte result
		pmovmskb ecx, xmm2                                  //
		xor     eax, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     epilogue
		add     edx, 16
		add     edi, 16
		and     edx, PAGE_SIZE - 1
		jmp     aligned_xmmword_loop

		align   16
	unaligned_xmmword_loop:
		cmp     edx, PAGE_SIZE - 16
		ja      word_loop                                   // jump if cross pages
		movdqu  xmm0, xmmword ptr [esi + edi]               // load 16 byte
		movdqu  xmm1, xmmword ptr [edi]                     //
		movdqa  xmm2, xmm0                                  // copy
		movdqa  xmm3, xmm1                                  //
		paddw   xmm0, xmm4                                  // all words greater than 'Z' if negative
		paddw   xmm1, xmm4                                  //
		pcmpgtw xmm0, xmm5                                  // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		pcmpgtw xmm1, xmm5                                  //
		pand    xmm0, xmm6                                  // assign a mask for the appropriate words
		pand    xmm1, xmm6                                  //
		por     xmm0, xmm2                                  // negation of the 5th bit - lowercase letters
		por     xmm1, xmm3                                  //
		pcmpeqw xmm0, xmm1                                  // compare
		pcmpeqw xmm2, xmm7                                  //
		pmovmskb eax, xmm0                                  // get one bit for each byte result
		pmovmskb ecx, xmm2                                  //
		xor     eax, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     epilogue
		add     edx, 16
		add     edi, 16
		and     edx, PAGE_SIZE - 1
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
		add     edi, eax
		movzx   eax, word ptr [esi + edi]
		movzx   edx, word ptr [edi]
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

		push    esi
		xor     eax, eax                                    // eax = 0
		mov     esi, dword ptr [string1 + 4]                // esi = string1
		mov     ecx, dword ptr [string2 + 4]                // ecx = string2
		sub     esi, ecx                                    // esi = (size_t)string1 - (size_t)string2

		align   16
	loop_begin:
		xor     edx, edx
		mov     ax, word ptr [ecx + esi]
		mov     dx, word ptr [ecx]
		add     ecx, 2
		sub     eax, edx
		jnz     compare_insensitive
		test    edx, edx
		jnz     loop_begin
		pop     esi
		ret

		align   16
	compare_insensitive:
		cmp     eax, 'a' - 'A'
		je      compare_above
		cmp     eax, 'A' - 'a'
		jne     not_equal
		sub     edx, 'a'
		xor     eax, eax
		cmp     edx, 'z' - 'a' + 1
		jb      loop_begin
		dec     eax
		pop     esi
		ret

		align   16
	compare_above:
		sub     edx, 'A'
		xor     eax, eax
		cmp     edx, 'Z' - 'A' + 1
		jb      loop_begin
		inc     eax
		pop     esi
		ret

		align   16
	not_equal:
		lea     eax, [eax + edx - 'A']
		sub     edx, 'A'
		cmp     eax, 'Z' - 'A' + 1
		jae     secondary_to_lower
		add     eax, 'a' - 'A'
	secondary_to_lower:
		cmp     edx, 'Z' - 'A' + 1
		jae     difference
		add     edx, 'a' - 'A'
	difference:
		sub     eax, edx
		pop     esi
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
