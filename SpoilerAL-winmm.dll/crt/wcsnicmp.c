#include <wchar.h>
#ifndef _M_IX86

int __cdecl _wcsnicmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	int ret, c;

	string1 += count;
	string2 += count;
	count ^= -1;
	ret = 0;
	while (++count)
		if (ret = towlower(string1[count]) - (c = towlower(string2[count])))
			break;
		else if (!c)
			break;
	return ret;
}
#else
#include "page.h"

static int __cdecl wcsnicmpAVX2(const wchar_t *string1, const wchar_t *string2, size_t count);
static int __cdecl wcsnicmpSSE2(const wchar_t *string1, const wchar_t *string2, size_t count);
static int __cdecl wcsnicmp386(const wchar_t *string1, const wchar_t *string2, size_t count);
static int __cdecl wcsnicmpCPUDispatch(const wchar_t *string1, const wchar_t *string2, size_t count);

static int(__cdecl * wcsnicmpDispatch)(const wchar_t *string1, const wchar_t *string2, size_t count) = wcsnicmpCPUDispatch;

extern const wchar_t ymmconst_upperW[16];
extern const wchar_t ymmconst_azrangeW[16];
extern const wchar_t ymmconst_casebitW[16];
#define upper   ymmconst_upperW
#define azrange ymmconst_azrangeW
#define casebit ymmconst_casebitW

__declspec(naked) int __cdecl _wcsnicmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	__asm
	{
		jmp     dword ptr [wcsnicmpDispatch]
	}
}

// AVX2 version
__declspec(naked) static int __cdecl wcsnicmpAVX2(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		push    edi
		xor     eax, eax                                    // eax = 0
		mov     ebx, dword ptr [count + 12]                 // ebx = count
		mov     edi, dword ptr [string2 + 12]               // edi = string2
		mov     esi, dword ptr [string1 + 12]               // esi = string1
		lea     edi, [edi + ebx * 2]                        // edi = end of string2
		lea     esi, [esi + ebx * 2]                        // esi = end of string1
		xor     ebx, -1                                     // ebx = -count - 1
		vmovdqa ymm4, ymmword ptr [upper]
		vmovdqa ymm5, ymmword ptr [azrange]
		vmovdqa ymm6, ymmword ptr [casebit]                 // bit to change
		vpxor   xmm7, xmm7, xmm7                            // set to zero
		jmp     word_loop_increment

		align   16
	word_loop:
		movzx   eax, word ptr [esi + ebx * 2]
		movzx   edx, word ptr [edi + ebx * 2]
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
	word_loop_increment:
		inc     ebx
		jz      epilog
		lea     edx, [edi + ebx * 2 + 1]
		lea     ecx, [esi + ebx * 2]
		and     edx, 14
		jnz     word_loop
		shl     ecx, 32 - PAGE_SHIFT
		mov     edx, edi
		and     edx, 1
		jnz     unaligned_xmmword_compare
		lea     edx, [edi + ebx * 2]
		and     edx, 31
		jz      aligned_ymmword_loop
	aligned_xmmword_check_cross_pages:
		cmp     ecx, -15 shl (32 - PAGE_SHIFT)
		jae     word_loop                                   // jump if cross pages
		vmovdqu xmm0, xmmword ptr [esi + ebx * 2]           // load 16 byte
		vmovdqa xmm1, xmmword ptr [edi + ebx * 2]           //
		vmovdqa xmm2, xmm0                                  // copy
		vmovdqa xmm3, xmm1                                  //
		vpaddw  xmm0, xmm0, xmm4                            // all words greater than 'Z' if negative
		vpaddw  xmm1, xmm1, xmm4                            //
		vpcmpgtw xmm0, xmm0, xmm5                           // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		vpcmpgtw xmm1, xmm1, xmm5                           //
		vpand   xmm0, xmm0, xmm6                            // assign a mask for the appropriate words
		vpand   xmm1, xmm1, xmm6                            //
		vpor    xmm0, xmm0, xmm2                            // negation of the 5th bit - lowercase letters
		vpor    xmm1, xmm1, xmm3                            //
		vpcmpeqw xmm0, xmm0, xmm1                           // compare
		vpcmpeqw xmm2, xmm2, xmm7                           //
		vpmovmskb edx, xmm0                                 // get one bit for each byte result
		vpmovmskb ecx, xmm2                                 //
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		add     ebx, 8
		jc      epilog
		test    ecx, ecx
		jnz     epilog
		lea     ecx, [esi + ebx * 2]
		shl     ecx, 32 - PAGE_SHIFT

		align   16
	aligned_ymmword_loop:
		cmp     ecx, -31 shl (32 - PAGE_SHIFT)
		jae     aligned_xmmword_check_cross_pages           // jump if cross pages
		vmovdqu ymm0, ymmword ptr [esi + ebx * 2]           // load 32 byte
		vmovdqa ymm1, ymmword ptr [edi + ebx * 2]           //
		vmovdqa ymm2, ymm0                                  // copy
		vmovdqa ymm3, ymm1                                  //
		vpaddw  ymm0, ymm0, ymm4                            // all words greater than 'Z' if negative
		vpaddw  ymm1, ymm1, ymm4                            //
		vpcmpgtw ymm0, ymm0, ymm5                           // ymm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		vpcmpgtw ymm1, ymm1, ymm5                           //
		vpand   ymm0, ymm0, ymm6                            // assign a mask for the appropriate words
		vpand   ymm1, ymm1, ymm6                            //
		vpor    ymm0, ymm0, ymm2                            // negation of the 5th bit - lowercase letters
		vpor    ymm1, ymm1, ymm3                            //
		vpcmpeqw ymm0, ymm0, ymm1                           // compare
		vpcmpeqw ymm2, ymm2, ymm7                           //
		vpmovmskb edx, ymm0                                 // get one bit for each byte result
		vpmovmskb ecx, ymm2                                 //
		xor     edx, -1
		jnz     ymmword_not_equal
		add     ebx, 16
		jc      epilog
		test    ecx, ecx
		jnz     epilog
		lea     ecx, [esi + ebx * 2]
		shl     ecx, 32 - PAGE_SHIFT
		jmp     aligned_ymmword_loop

		align   16
	unaligned_xmmword_compare:
		lea     edx, [edi + ebx * 2 + 1]
		and     edx, 31
		jz      unaligned_ymmword_loop
	unaligned_xmmword_check_cross_pages:
		cmp     ecx, -15 shl (32 - PAGE_SHIFT)
		jae     word_loop                                   // jump if cross pages
		vmovdqu xmm0, xmmword ptr [esi + ebx * 2]           // load 16 byte
		vmovdqu xmm1, xmmword ptr [edi + ebx * 2]           //
		vmovdqa xmm2, xmm0                                  // copy
		vmovdqa xmm3, xmm1                                  //
		vpaddw  xmm0, xmm0, xmm4                            // all words greater than 'Z' if negative
		vpaddw  xmm1, xmm1, xmm4                            //
		vpcmpgtw xmm0, xmm0, xmm5                           // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		vpcmpgtw xmm1, xmm1, xmm5                           //
		vpand   xmm0, xmm0, xmm6                            // assign a mask for the appropriate words
		vpand   xmm1, xmm1, xmm6                            //
		vpor    xmm0, xmm0, xmm2                            // negation of the 5th bit - lowercase letters
		vpor    xmm1, xmm1, xmm3                            //
		vpcmpeqw xmm0, xmm0, xmm1                           // compare
		vpcmpeqw xmm2, xmm2, xmm7                           //
		vpmovmskb edx, xmm0                                 // get one bit for each byte result
		vpmovmskb ecx, xmm2                                 //
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		add     ebx, 8
		jc      epilog
		test    ecx, ecx
		jnz     epilog
		lea     ecx, [esi + ebx * 2]
		shl     ecx, 32 - PAGE_SHIFT

		align   16
	unaligned_ymmword_loop:
		cmp     ecx, -31 shl (32 - PAGE_SHIFT)
		jae     unaligned_xmmword_check_cross_pages         // jump if cross pages
		vmovdqu ymm0, ymmword ptr [esi + ebx * 2]           // load 32 byte
		vmovdqu ymm1, ymmword ptr [edi + ebx * 2]           //
		vmovdqa ymm2, ymm0                                  // copy
		vmovdqa ymm3, ymm1                                  //
		vpaddw  ymm0, ymm0, ymm4                            // all words greater than 'Z' if negative
		vpaddw  ymm1, ymm1, ymm4                            //
		vpcmpgtw ymm0, ymm0, ymm5                           // ymm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		vpcmpgtw ymm1, ymm1, ymm5                           //
		vpand   ymm0, ymm0, ymm6                            // assign a mask for the appropriate words
		vpand   ymm1, ymm1, ymm6                            //
		vpor    ymm0, ymm0, ymm2                            // negation of the 5th bit - lowercase letters
		vpor    ymm1, ymm1, ymm3                            //
		vpcmpeqw ymm0, ymm0, ymm1                           // compare
		vpcmpeqw ymm2, ymm2, ymm7                           //
		vpmovmskb edx, ymm0                                 // get one bit for each byte result
		vpmovmskb ecx, ymm2                                 //
		xor     edx, -1
		jnz     ymmword_not_equal
		add     ebx, 16
		jc      epilog
		test    ecx, ecx
		jnz     epilog
		lea     ecx, [esi + ebx * 2]
		shl     ecx, 32 - PAGE_SHIFT
		jmp     unaligned_ymmword_loop

		align   16
	xmmword_not_equal:
		test    ecx, ecx
		jz      ymmword_has_not_null
		bsf     ecx, ecx
		mov     eax, 0FFFFH
		xor     ecx, 15
		shr     eax, cl
		and     eax, edx
		jz      epilog
		mov     edx, eax
		xor     eax, eax
		jmp     ymmword_has_not_null

		align   16
	ymmword_not_equal:
		test    ecx, ecx
		jz      ymmword_has_not_null
		bsf     ecx, ecx
		or      eax, -1
		xor     ecx, 31
		shr     eax, cl
		and     eax, edx
		jz      epilog
		mov     edx, eax
		xor     eax, eax
	ymmword_has_not_null:
		bsf     edx, edx
		shr     edx, 1
		add     ebx, edx
		jc      epilog
		mov     ax, word ptr [esi + ebx * 2]
		mov     dx, word ptr [edi + ebx * 2]
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
		pop     ebx
		vzeroupper
		ret

		#undef string1
		#undef string2
		#undef count
	}
}

// SSE2 version
__declspec(naked) static int __cdecl wcsnicmpSSE2(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		push    edi
		xor     eax, eax                                    // eax = 0
		mov     ebx, dword ptr [count + 12]                 // ebx = count
		mov     edi, dword ptr [string2 + 12]               // edi = string2
		mov     esi, dword ptr [string1 + 12]               // esi = string1
		lea     edi, [edi + ebx * 2]                        // edi = end of string2
		lea     esi, [esi + ebx * 2]                        // esi = end of string1
		xor     ebx, -1                                     // ebx = -count - 1
		movdqa  xmm4, xmmword ptr [upper]
		movdqa  xmm5, xmmword ptr [azrange]
		movdqa  xmm6, xmmword ptr [casebit]                 // bit to change
		pxor    xmm7, xmm7                                  // set to zero
		jmp     word_loop_increment

		align   16
	word_loop:
		movzx   eax, word ptr [esi + ebx * 2]
		movzx   edx, word ptr [edi + ebx * 2]
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
	word_loop_increment:
		inc     ebx
		jz      epilog
		lea     edx, [edi + ebx * 2 + 1]
		lea     ecx, [esi + ebx * 2]
		and     edx, 14
		jnz     word_loop
		shl     ecx, 32 - PAGE_SHIFT
		mov     edx, edi
		and     edx, 1
		jnz     unaligned_xmmword_loop

		align   16
	aligned_xmmword_loop:
		cmp     ecx, -15 shl (32 - PAGE_SHIFT)
		jae     word_loop                                   // jump if cross pages
		movdqu  xmm0, xmmword ptr [esi + ebx * 2]           // load 16 byte
		movdqa  xmm1, xmmword ptr [edi + ebx * 2]           //
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
		pmovmskb edx, xmm0                                  // get one bit for each byte result
		pmovmskb ecx, xmm2                                  //
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		add     ebx, 8
		jc      epilog
		test    ecx, ecx
		jnz     epilog
		lea     ecx, [esi + ebx * 2]
		shl     ecx, 32 - PAGE_SHIFT
		jmp     aligned_xmmword_loop

		align   16
	unaligned_xmmword_loop:
		cmp     ecx, -15 shl (32 - PAGE_SHIFT)
		jae     word_loop                                   // jump if cross pages
		movdqu  xmm0, xmmword ptr [esi + ebx * 2]           // load 16 byte
		movdqu  xmm1, xmmword ptr [edi + ebx * 2]           //
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
		pmovmskb edx, xmm0                                  // get one bit for each byte result
		pmovmskb ecx, xmm2                                  //
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		add     ebx, 8
		jc      epilog
		test    ecx, ecx
		jnz     epilog
		lea     ecx, [esi + ebx * 2]
		shl     ecx, 32 - PAGE_SHIFT
		jmp     unaligned_xmmword_loop

		align   16
	xmmword_not_equal:
		test    ecx, ecx
		jz      xmmword_has_not_null
		bsf     ecx, ecx
		mov     eax, 0FFFFH
		xor     ecx, 15
		shr     eax, cl
		and     eax, edx
		jz      epilog
		mov     edx, eax
		xor     eax, eax
	xmmword_has_not_null:
		bsf     edx, edx
		shr     edx, 1
		add     ebx, edx
		jc      epilog
		mov     ax, word ptr [esi + ebx * 2]
		mov     dx, word ptr [edi + ebx * 2]
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
		pop     ebx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}

// 80386 version
__declspec(naked) static int __cdecl wcsnicmp386(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		push    edi
		xor     eax, eax                                    // eax = 0
		mov     esi, dword ptr [string1 + 12]               // esi = string1
		mov     edi, dword ptr [string2 + 12]               // edi = string2
		mov     ecx, dword ptr [count + 12]                 // ecx = count
		xor     edx, edx                                    // edx = 0
		lea     esi, [esi + ecx * 2]                        // esi = end of string1
		lea     edi, [edi + ecx * 2]                        // edi = end of string2
		xor     ecx, -1                                     // ecx = -count - 1

		align   16
	loop_begin:
		inc     ecx
		jz      return_equal
		mov     ax, word ptr [esi + ecx * 2]
		mov     dx, word ptr [edi + ecx * 2]
		sub     eax, edx
		jnz     compare_insensitive
		test    edx, edx
		jnz     loop_begin
	return_equal:
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	compare_insensitive:
		cmp     eax, 'a' - 'A'
		je      compare_above
		cmp     eax, 'A' - 'a'
		jne     not_equal
		xor     eax, eax
		lea     ebx, [edx - 'a']
		cmp     ebx, 'z' - 'a' + 1
		jb      loop_begin
		dec     eax
		jmp     epilog

		align   16
	compare_above:
		xor     eax, eax
		lea     ebx, [edx - 'A']
		cmp     ebx, 'Z' - 'A' + 1
		jb      loop_begin
		inc     eax
		jmp     epilog

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
	epilog:
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}

__declspec(naked) static int __cdecl wcsnicmpCPUDispatch(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	__asm
	{
		mov     eax, dword ptr [__isa_available]
		cmp     eax, __ISA_AVAILABLE_AVX2
		jb      L1
		mov     dword ptr [wcsnicmpDispatch], offset wcsnicmpAVX2
		jmp     wcsnicmpAVX2

	L1:
		test    eax, eax
		jz      L2
		mov     dword ptr [wcsnicmpDispatch], offset wcsnicmpSSE2
		jmp     wcsnicmpSSE2

	L2:
		mov     dword ptr [wcsnicmpDispatch], offset wcsnicmp386
		jmp     wcsnicmp386
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
	#undef __ISA_AVAILABLE_AVX
	#undef __ISA_AVAILABLE_ENFSTRG
	#undef __ISA_AVAILABLE_AVX2
}
#endif
