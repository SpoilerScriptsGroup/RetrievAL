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
#include "page.h"

static int __cdecl wcsicmpAVX2(const wchar_t *string1, const wchar_t *string2);
static int __cdecl wcsicmpSSE2(const wchar_t *string1, const wchar_t *string2);
static int __cdecl wcsicmp386(const wchar_t *string1, const wchar_t *string2);
static int __cdecl wcsicmpCPUDispatch(const wchar_t *string1, const wchar_t *string2);

static int(__cdecl * wcsicmpDispatch)(const wchar_t *string1, const wchar_t *string2) = wcsicmpCPUDispatch;

extern const wchar_t ymmconst_upperW[16];
extern const wchar_t ymmconst_azrangeW[16];
extern const wchar_t ymmconst_casebitW[16];
#define upper   ymmconst_upperW
#define azrange ymmconst_azrangeW
#define casebit ymmconst_casebitW

__declspec(naked) int __cdecl _wcsicmp(const wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		jmp     dword ptr [wcsicmpDispatch]
	}
}

// AVX2 version
__declspec(naked) static int __cdecl wcsicmpAVX2(const wchar_t *string1, const wchar_t *string2)
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
		vmovdqa ymm4, ymmword ptr [upper]
		vmovdqa ymm5, ymmword ptr [azrange]
		vmovdqa ymm6, ymmword ptr [casebit]                 // bit to change
		vpxor   ymm7, ymm7, ymm7                            // set to zero
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
		jnz     epilog
		cmp     edx, '\0' - 'A'
		je      epilog
		lea     edx, [edi + 3]
		add     edi, 2
	word_loop_entry:
		and     edx, 14
		jnz     word_loop

		lea     edx, [esi + edi]
		mov     ecx, edi
		and     edx, PAGE_SIZE - 1
		and     ecx, 1
		jnz     unaligned_xmmword_compare
		test    edi, 31
		jz      aligned_ymmword_loop
	aligned_xmmword_check_cross_pages:
		cmp     edx, PAGE_SIZE - 15
		jae     word_loop                                   // jump if cross pages
		vmovdqu xmm2, xmmword ptr [esi + edi]               // load 16 byte
		vmovdqa xmm3, xmmword ptr [edi]                     //
		vpaddw  xmm0, xmm2, xmm4                            // all words greater than 'Z' if negative
		vpaddw  xmm1, xmm3, xmm4                            //
		vpcmpgtw xmm0, xmm0, xmm5                           // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		vpcmpgtw xmm1, xmm1, xmm5                           //
		vpand   xmm0, xmm0, xmm6                            // assign a mask for the appropriate words
		vpand   xmm1, xmm1, xmm6                            //
		vpor    xmm0, xmm0, xmm2                            // negation of the 5th bit - lowercase letters
		vpor    xmm1, xmm1, xmm3                            //
		vpcmpeqw xmm0, xmm0, xmm1                           // compare
		vpcmpeqw xmm2, xmm2, xmm7                           //
		vpmovmskb eax, xmm0                                 // get one bit for each byte result
		vpmovmskb ecx, xmm2                                 //
		xor     eax, 0FFFFH
		jnz     ymmword_not_equal
		test    ecx, ecx
		jnz     epilog
		add     edx, 16
		add     edi, 16
		and     edx, PAGE_SIZE - 1

		align   16
	aligned_ymmword_loop:
		cmp     edx, PAGE_SIZE - 31
		jae     aligned_xmmword_check_cross_pages           // jump if cross pages
		vmovdqu ymm2, ymmword ptr [esi + edi]               // load 32 byte
		vmovdqa ymm3, ymmword ptr [edi]                     //
		vpaddw  ymm0, ymm2, ymm4                            // all words greater than 'Z' if negative
		vpaddw  ymm1, ymm3, ymm4                            //
		vpcmpgtw ymm0, ymm0, ymm5                           // ymm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		vpcmpgtw ymm1, ymm1, ymm5                           //
		vpand   ymm0, ymm0, ymm6                            // assign a mask for the appropriate words
		vpand   ymm1, ymm1, ymm6                            //
		vpor    ymm0, ymm0, ymm2                            // negation of the 5th bit - lowercase letters
		vpor    ymm1, ymm1, ymm3                            //
		vpcmpeqw ymm0, ymm0, ymm1                           // compare
		vpcmpeqw ymm2, ymm2, ymm7                           //
		vpmovmskb eax, ymm0                                 // get one bit for each byte result
		vpmovmskb ecx, ymm2                                 //
		xor     eax, -1
		jnz     ymmword_not_equal
		test    ecx, ecx
		jnz     epilog
		add     edx, 32
		add     edi, 32
		and     edx, PAGE_SIZE - 1
		jmp     aligned_ymmword_loop

		align   16
	unaligned_xmmword_compare:
		lea     eax, [edi + 1]
		and     eax, 31
		jz      unaligned_ymmword_loop
	unaligned_xmmword_check_cross_pages:
		cmp     edx, PAGE_SIZE - 15
		jae     word_loop                                   // jump if cross pages
		vmovdqu xmm2, xmmword ptr [esi + edi]               // load 16 byte
		vmovdqu xmm3, xmmword ptr [edi]                     //
		vpaddw  xmm0, xmm2, xmm4                            // all words greater than 'Z' if negative
		vpaddw  xmm1, xmm3, xmm4                            //
		vpcmpgtw xmm0, xmm0, xmm5                           // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		vpcmpgtw xmm1, xmm1, xmm5                           //
		vpand   xmm0, xmm0, xmm6                            // assign a mask for the appropriate words
		vpand   xmm1, xmm1, xmm6                            //
		vpor    xmm0, xmm0, xmm2                            // negation of the 5th bit - lowercase letters
		vpor    xmm1, xmm1, xmm3                            //
		vpcmpeqw xmm0, xmm0, xmm1                           // compare
		vpcmpeqw xmm2, xmm2, xmm7                           //
		vpmovmskb eax, xmm0                                 // get one bit for each byte result
		vpmovmskb ecx, xmm2                                 //
		xor     eax, 0FFFFH
		jnz     ymmword_not_equal
		test    ecx, ecx
		jnz     epilog
		add     edx, 16
		add     edi, 16
		and     edx, PAGE_SIZE - 1

		align   16
	unaligned_ymmword_loop:
		cmp     edx, PAGE_SIZE - 31
		jae     unaligned_xmmword_check_cross_pages         // jump if cross pages
		vmovdqu ymm2, ymmword ptr [esi + edi]               // load 32 byte
		vmovdqu ymm3, ymmword ptr [edi]                     //
		vpaddw  ymm0, ymm2, ymm4                            // all words greater than 'Z' if negative
		vpaddw  ymm1, ymm3, ymm4                            //
		vpcmpgtw ymm0, ymm0, ymm5                           // ymm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		vpcmpgtw ymm1, ymm1, ymm5                           //
		vpand   ymm0, ymm0, ymm6                            // assign a mask for the appropriate words
		vpand   ymm1, ymm1, ymm6                            //
		vpor    ymm0, ymm0, ymm2                            // negation of the 5th bit - lowercase letters
		vpor    ymm1, ymm1, ymm3                            //
		vpcmpeqw ymm0, ymm0, ymm1                           // compare
		vpcmpeqw ymm2, ymm2, ymm7                           //
		vpmovmskb eax, ymm0                                 // get one bit for each byte result
		vpmovmskb ecx, ymm2                                 //
		xor     eax, -1
		jnz     ymmword_not_equal
		test    ecx, ecx
		jnz     epilog
		add     edx, 32
		add     edi, 32
		and     edx, PAGE_SIZE - 1
		jmp     unaligned_ymmword_loop

		align   16
	ymmword_not_equal:
		test    ecx, ecx
		jz      ymmword_has_not_null
		bsf     ecx, ecx
		or      edx, -1
		xor     ecx, 31
		shr     edx, cl
		and     eax, edx
		jz      epilog
	ymmword_has_not_null:
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
	epilog:
		pop     edi
		pop     esi
		ret

		#undef string1
		#undef string2
	}
}

// SSE2 version
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
		jnz     epilog
		cmp     edx, '\0' - 'A'
		je      epilog
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
		cmp     edx, PAGE_SIZE - 15
		jae     word_loop                                   // jump if cross pages
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
		jnz     epilog
		add     edx, 16
		add     edi, 16
		and     edx, PAGE_SIZE - 1
		jmp     aligned_xmmword_loop

		align   16
	unaligned_xmmword_loop:
		cmp     edx, PAGE_SIZE - 15
		jae     word_loop                                   // jump if cross pages
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
		jnz     epilog
		add     edx, 16
		add     edi, 16
		and     edx, PAGE_SIZE - 1
		jmp     unaligned_xmmword_loop

		align   16
	xmmword_not_equal:
		test    ecx, ecx
		jz      xmmword_has_not_null
		bsf     ecx, ecx
		or      edx, -1
		xor     ecx, 31
		shr     edx, cl
		and     eax, edx
		jz      epilog
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
	epilog:
		pop     edi
		pop     esi
		ret

		#undef string1
		#undef string2
	}
}

// 80386 version
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
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	static void *table[] = {
		(void *)wcsicmp386,
		(void *)wcsicmpSSE2,
		(void *)wcsicmpSSE2,
		(void *)wcsicmpSSE2,
		(void *)wcsicmpSSE2,
	};

	__asm
	{
		mov     ecx, dword ptr [__isa_available]
		mov     eax, offset wcsicmpAVX2
		cmp     ecx, __ISA_AVAILABLE_AVX2
		jae     L1
		mov     eax, dword ptr [table + ecx * 4]
	L1:
		mov     dword ptr [wcsicmpDispatch], eax
		jmp     eax
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
	#undef __ISA_AVAILABLE_AVX
	#undef __ISA_AVAILABLE_ENFSTRG
	#undef __ISA_AVAILABLE_AVX2
}
#endif
