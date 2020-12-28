#include <wchar.h>
#ifndef _M_IX86

int __cdecl wcsncmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	unsigned short c1, c2;

	string1 += count;
	string2 += count;
	count ^= -1;
	do
		if (!++count)
			break;
		else if ((c1 = string1[count]) != (c2 = string2[count]))
			return (int)c1 - (int)c2;
	while (c1);
	return 0;
}
#else
#include "page.h"

static int __cdecl wcsncmpAVX2(const wchar_t *string1, const wchar_t *string2, size_t count);
static int __cdecl wcsncmpSSE2(const wchar_t *string1, const wchar_t *string2, size_t count);
static int __cdecl wcsncmp386(const wchar_t *string1, const wchar_t *string2, size_t count);
static int __cdecl wcsncmpCPUDispatch(const wchar_t *string1, const wchar_t *string2, size_t count);

static int(__cdecl * wcsncmpDispatch)(const wchar_t *string1, const wchar_t *string2, size_t count) = wcsncmpCPUDispatch;

__declspec(naked) int __cdecl wcsncmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	__asm
	{
		jmp     dword ptr [wcsncmpDispatch]
	}
}

// AVX2 version
__declspec(naked) static int __cdecl wcsncmpAVX2(const wchar_t *string1, const wchar_t *string2, size_t count)
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
		vpxor   ymm2, ymm2, ymm2
		jmp     word_loop_increment

		align   16
	word_loop:
		movzx   eax, word ptr [esi + ebx * 2]
		movzx   edx, word ptr [edi + ebx * 2]
		sub     eax, edx
		jnz     epilog
		test    edx, edx
		jz      epilog
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
		vmovdqu xmm0, xmmword ptr [esi + ebx * 2]
		vmovdqa xmm1, xmmword ptr [edi + ebx * 2]
		vpcmpeqw xmm0, xmm0, xmm1
		vpcmpeqw xmm1, xmm1, xmm2
		vpmovmskb edx, xmm0
		vpmovmskb ecx, xmm1
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     epilog
		add     ebx, 8
		jc      epilog
		lea     ecx, [esi + ebx * 2]
		shl     ecx, 32 - PAGE_SHIFT

		align   16
	aligned_ymmword_loop:
		cmp     ecx, -31 shl (32 - PAGE_SHIFT)
		jae     aligned_xmmword_check_cross_pages           // jump if cross pages
		vmovdqu ymm0, ymmword ptr [esi + ebx * 2]
		vmovdqa ymm1, ymmword ptr [edi + ebx * 2]
		vpcmpeqw ymm0, ymm0, ymm1
		vpcmpeqw ymm1, ymm1, ymm2
		vpmovmskb edx, ymm0
		vpmovmskb ecx, ymm1
		xor     edx, -1
		jnz     ymmword_not_equal
		test    ecx, ecx
		jnz     epilog
		add     ebx, 16
		jc      epilog
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
		vmovdqu xmm0, xmmword ptr [esi + ebx * 2]
		vmovdqu xmm1, xmmword ptr [edi + ebx * 2]
		vpcmpeqw xmm0, xmm0, xmm1
		vpcmpeqw xmm1, xmm1, xmm2
		vpmovmskb edx, xmm0
		vpmovmskb ecx, xmm1
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     epilog
		add     ebx, 8
		jc      epilog
		lea     ecx, [esi + ebx * 2]
		shl     ecx, 32 - PAGE_SHIFT

		align   16
	unaligned_ymmword_loop:
		cmp     ecx, -31 shl (32 - PAGE_SHIFT)
		jae     unaligned_xmmword_check_cross_pages         // jump if cross pages
		vmovdqu ymm0, ymmword ptr [esi + ebx * 2]
		vmovdqu ymm1, ymmword ptr [edi + ebx * 2]
		vpcmpeqw ymm0, ymm0, ymm1
		vpcmpeqw ymm1, ymm1, ymm2
		vpmovmskb edx, ymm0
		vpmovmskb ecx, ymm1
		xor     edx, -1
		jnz     ymmword_not_equal
		test    ecx, ecx
		jnz     epilog
		add     ebx, 16
		jc      epilog
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
		movzx   eax, word ptr [esi + ebx * 2]
		movzx   edx, word ptr [edi + ebx * 2]
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
__declspec(naked) static int __cdecl wcsncmpSSE2(const wchar_t *string1, const wchar_t *string2, size_t count)
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
		pxor    xmm2, xmm2
		jmp     word_loop_increment

		align   16
	word_loop:
		movzx   eax, word ptr [esi + ebx * 2]
		movzx   edx, word ptr [edi + ebx * 2]
		sub     eax, edx
		jnz     epilog
		test    edx, edx
		jz      epilog
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
		movdqu  xmm0, xmmword ptr [esi + ebx * 2]
		movdqa  xmm1, xmmword ptr [edi + ebx * 2]
		pcmpeqw xmm0, xmm1
		pcmpeqw xmm1, xmm2
		pmovmskb edx, xmm0
		pmovmskb ecx, xmm1
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     epilog
		add     ebx, 8
		jc      epilog
		lea     ecx, [esi + ebx * 2]
		shl     ecx, 32 - PAGE_SHIFT
		jmp     aligned_xmmword_loop

		align   16
	unaligned_xmmword_loop:
		cmp     ecx, -15 shl (32 - PAGE_SHIFT)
		jae     word_loop                                   // jump if cross pages
		movdqu  xmm0, xmmword ptr [esi + ebx * 2]
		movdqu  xmm1, xmmword ptr [edi + ebx * 2]
		pcmpeqw xmm0, xmm1
		pcmpeqw xmm1, xmm2
		pmovmskb edx, xmm0
		pmovmskb ecx, xmm1
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     epilog
		add     ebx, 8
		jc      epilog
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
		movzx   eax, word ptr [esi + ebx * 2]
		movzx   edx, word ptr [edi + ebx * 2]
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
__declspec(naked) static int __cdecl wcsncmp386(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 8]                // esi = string1
		mov     edi, dword ptr [string2 + 8]                // edi = string2
		mov     ecx, dword ptr [count + 8]                  // ecx = count
		xor     eax, eax                                    // eax = 0
		lea     esi, [esi + ecx * 2]                        // esi = end of string1
		lea     edi, [edi + ecx * 2]                        // edi = end of string2
		xor     ecx, -1                                     // ecx = -count - 1
		xor     edx, edx                                    // edx = 0

		align   16
	loop_begin:
		inc     ecx
		jz      epilog
		mov     ax, word ptr [esi + ecx * 2]
		mov     dx, word ptr [edi + ecx * 2]
		sub     eax, edx
		jnz     epilog
		test    edx, edx
		jnz     loop_begin
	epilog:
		pop     edi
		pop     esi
		ret

		#undef string1
		#undef string2
		#undef count
	}
}

__declspec(naked) static int __cdecl wcsncmpCPUDispatch(const wchar_t *string1, const wchar_t *string2, size_t count)
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
		mov     dword ptr [wcsncmpDispatch], offset wcsncmpAVX2
		jmp     wcsncmpAVX2

	L1:
		test    eax, eax
		jz      L2
		mov     dword ptr [wcsncmpDispatch], offset wcsncmpSSE2
		jmp     wcsncmpSSE2

	L2:
		mov     dword ptr [wcsncmpDispatch], offset wcsncmp386
		jmp     wcsncmp386
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
	#undef __ISA_AVAILABLE_AVX
	#undef __ISA_AVAILABLE_ENFSTRG
	#undef __ISA_AVAILABLE_AVX2
}
#endif
