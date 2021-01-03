#include <wchar.h>
#ifndef _M_IX86

int __cdecl _wmemicmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count)
{
	int ret;

	(wchar_t *)buffer1 += count;
	(wchar_t *)buffer2 += count;
	count ^= -1;
	while (++count)
		if (ret = towlower(buffer1[count]) - towlower(buffer2[count]))
			return ret;
	return 0;
}
#else
#include "page.h"

static int __cdecl wmemicmpAVX2(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
static int __cdecl wmemicmpSSE2(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
static int __cdecl wmemicmp386(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
static int __cdecl wmemicmpCPUDispatch(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);

static int(__cdecl * wmemicmpDispatch)(const wchar_t *buffer1, const wchar_t *buffer2, size_t count) = wmemicmpCPUDispatch;

extern const wchar_t ymmconst_upperW[16];
extern const wchar_t ymmconst_azrangeW[16];
extern const wchar_t ymmconst_casebitW[16];
#define upper   ymmconst_upperW
#define azrange ymmconst_azrangeW
#define casebit ymmconst_casebitW

__declspec(naked) int __cdecl _wmemicmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count)
{
	__asm
	{
		jmp     dword ptr [wmemicmpDispatch]
	}
}

// AVX2 version
__declspec(naked) static int __cdecl wmemicmpAVX2(const wchar_t *buffer1, const wchar_t *buffer2, size_t count)
{
	__asm
	{
		#define buffer1 (esp + 4)
		#define buffer2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    ebp
		push    esi
		push    edi
		xor     ebp, ebp                                    // ebp = 0
		mov     esi, dword ptr [buffer1 + 16]               // esi = buffer1
		mov     edi, dword ptr [buffer2 + 16]               // edi = buffer2
		mov     ebx, dword ptr [count + 16]                 // ebx = count
		sub     ebp, edi                                    // ebp = -buffer2
		lea     esi, [esi + ebx * 2]                        // esi = end of buffer1
		lea     edi, [edi + ebx * 2]                        // edi = end of buffer2
		xor     ebx, -1                                     // ebx = -count - 1
		and     ebp, 30                                     // ebp = -buffer2 & 30
		xor     eax, eax                                    // eax = 0
		jmp     word_loop_entry

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
	word_loop_entry:
		inc     ebx
		jz      epilog
		sub     ebp, 2
		jae     word_loop
		vmovdqa ymm4, ymmword ptr [upper]
		vmovdqa ymm5, ymmword ptr [azrange]
		vmovdqa ymm6, ymmword ptr [casebit]                 // bit to change
		mov     edx, edi
		lea     ecx, [edi + ebx * 2 + 1]
		and     edx, 1
		jnz     unaligned
		and     ecx, 16
		jnz     aligned_xmmword_entry
	aligned_ymmword_entry:
		mov     edx, 15
		sub     esi, 30
		sub     edi, 30
		add     ebx, 15
		jnc     aligned_ymmword_loop
		sub     ebx, 15
	aligned_ymmword_loop_last:
		lea     ecx, [ebx + ebx]
		add     esi, 30
		add     ecx, esi
		add     edi, 30
		shl     ecx, 32 - PAGE_SHIFT
		xor     edx, edx
		cmp     ecx, -31 shl (32 - PAGE_SHIFT)
		jb      aligned_ymmword_loop                        // jump if not cross pages
	aligned_xmmword_entry:
		lea     ecx, [esi + ebx * 2]
		shl     ecx, 32 - PAGE_SHIFT
		cmp     ecx, -15 shl (32 - PAGE_SHIFT)
		jae     word_loop                                   // jump if cross pages
		vmovdqu xmm2, xmmword ptr [esi + ebx * 2]           // load 16 byte
		vmovdqa xmm3, xmmword ptr [edi + ebx * 2]           //
		vpaddw  xmm0, xmm2, xmm4                            // all words greater than 'Z' if negative
		vpaddw  xmm1, xmm3, xmm4                            //
		vpcmpgtw xmm0, xmm0, xmm5                           // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		vpcmpgtw xmm1, xmm1, xmm5                           //
		vpand   xmm0, xmm0, xmm6                            // assign a mask for the appropriate words
		vpand   xmm1, xmm1, xmm6                            //
		vpor    xmm0, xmm0, xmm2                            // negation of the 5th bit - lowercase letters
		vpor    xmm1, xmm1, xmm3                            //
		vpcmpeqw xmm0, xmm0, xmm1                           // compare
		vpmovmskb ecx, xmm0                                 // get one bit for each byte result
		xor     ecx, 0FFFFH
		jnz     ymmword_not_equal
		add     ebx, 8
		jnc     aligned_ymmword_entry
		jmp     epilog

		align   16
	aligned_ymmword_loop:
		vmovdqu ymm2, ymmword ptr [esi + ebx * 2]           // load 32 byte
		vmovdqa ymm3, ymmword ptr [edi + ebx * 2]           //
		vpaddw  ymm0, ymm2, ymm4                            // all words greater than 'Z' if negative
		vpaddw  ymm1, ymm3, ymm4                            //
		vpcmpgtw ymm0, ymm0, ymm5                           // ymm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		vpcmpgtw ymm1, ymm1, ymm5                           //
		vpand   ymm0, ymm0, ymm6                            // assign a mask for the appropriate words
		vpand   ymm1, ymm1, ymm6                            //
		vpor    ymm0, ymm0, ymm2                            // negation of the 5th bit - lowercase letters
		vpor    ymm1, ymm1, ymm3                            //
		vpcmpeqw ymm0, ymm0, ymm1                           // compare
		vpmovmskb ecx, ymm0                                 // get one bit for each byte result
		xor     ecx, -1
		jnz     ymmword_not_equal
		add     ebx, 16
		jnc     aligned_ymmword_loop
		sub     ebx, edx
		jb      aligned_ymmword_loop_last
		jmp     epilog

		align   8
	unaligned:
		and     ecx, 16
		jnz     unaligned_xmmword_entry
	unaligned_ymmword_entry:
		mov     edx, 15
		sub     esi, 30
		sub     edi, 30
		add     ebx, 15
		jnc     unaligned_ymmword_loop
		sub     ebx, 15
	unaligned_ymmword_loop_last:
		lea     ecx, [ebx + ebx]
		add     esi, 30
		add     ecx, esi
		add     edi, 30
		shl     ecx, 32 - PAGE_SHIFT
		xor     edx, edx
		cmp     ecx, -31 shl (32 - PAGE_SHIFT)
		jb      unaligned_ymmword_loop                      // jump if not cross pages
	unaligned_xmmword_entry:
		lea     ecx, [esi + ebx * 2]
		shl     ecx, 32 - PAGE_SHIFT
		cmp     ecx, -15 shl (32 - PAGE_SHIFT)
		jae     word_loop                                   // jump if cross pages
		vmovdqu xmm2, xmmword ptr [esi + ebx * 2]           // load 16 byte
		vmovdqu xmm3, xmmword ptr [edi + ebx * 2]           //
		vpaddw  xmm0, xmm2, xmm4                            // all words greater than 'Z' if negative
		vpaddw  xmm1, xmm3, xmm4                            //
		vpcmpgtw xmm0, xmm0, xmm5                           // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		vpcmpgtw xmm1, xmm1, xmm5                           //
		vpand   xmm0, xmm0, xmm6                            // assign a mask for the appropriate words
		vpand   xmm1, xmm1, xmm6                            //
		vpor    xmm0, xmm0, xmm2                            // negation of the 5th bit - lowercase letters
		vpor    xmm1, xmm1, xmm3                            //
		vpcmpeqw xmm0, xmm0, xmm1                           // compare
		vpmovmskb ecx, xmm0                                 // get one bit for each byte result
		xor     ecx, 0FFFFH
		jnz     ymmword_not_equal
		add     ebx, 8
		jnc     unaligned_ymmword_entry
		jmp     epilog

		align   16
	unaligned_ymmword_loop:
		vmovdqu ymm2, ymmword ptr [esi + ebx * 2]           // load 32 byte
		vmovdqu ymm3, ymmword ptr [edi + ebx * 2]           //
		vpaddw  ymm0, ymm2, ymm4                            // all words greater than 'Z' if negative
		vpaddw  ymm1, ymm3, ymm4                            //
		vpcmpgtw ymm0, ymm0, ymm5                           // ymm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		vpcmpgtw ymm1, ymm1, ymm5                           //
		vpand   ymm0, ymm0, ymm6                            // assign a mask for the appropriate words
		vpand   ymm1, ymm1, ymm6                            //
		vpor    ymm0, ymm0, ymm2                            // negation of the 5th bit - lowercase letters
		vpor    ymm1, ymm1, ymm3                            //
		vpcmpeqw ymm0, ymm0, ymm1                           // compare
		vpmovmskb ecx, ymm0                                 // get one bit for each byte result
		xor     ecx, -1
		jnz     ymmword_not_equal
		add     ebx, 16
		jnc     unaligned_ymmword_loop
		sub     ebx, edx
		jb      unaligned_ymmword_loop_last
		jmp     epilog

		align   16
	ymmword_not_equal:
		bsf     ecx, ecx
		sub     ebx, edx
		add     edx, edx
		shr     ecx, 1
		add     esi, edx
		add     ebx, ecx
		jc      epilog
		add     edi, edx
		xor     edx, edx
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
		pop     ebp
		pop     ebx
		vzeroupper
		ret

		#undef buffer1
		#undef buffer2
		#undef count
	}
}

// SSE2 version
__declspec(naked) static int __cdecl wmemicmpSSE2(const wchar_t *buffer1, const wchar_t *buffer2, size_t count)
{
	__asm
	{
		#define buffer1 (esp + 4)
		#define buffer2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    ebp
		push    esi
		push    edi
		xor     ebp, ebp                                    // ebp = 0
		mov     esi, dword ptr [buffer1 + 16]               // esi = buffer1
		mov     edi, dword ptr [buffer2 + 16]               // edi = buffer2
		mov     ebx, dword ptr [count + 16]                 // ebx = count
		sub     ebp, edi                                    // ebp = -buffer2
		lea     esi, [esi + ebx * 2]                        // esi = end of buffer1
		lea     edi, [edi + ebx * 2]                        // edi = end of buffer2
		xor     ebx, -1                                     // ebx = -count - 1
		and     ebp, 14                                     // ebp = -buffer2 & 14
		xor     eax, eax                                    // eax = 0
		jmp     word_loop_entry

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
	word_loop_entry:
		inc     ebx
		jz      epilog
		sub     ebp, 2
		jae     word_loop
		movdqa  xmm4, xmmword ptr [upper]
		movdqa  xmm5, xmmword ptr [azrange]
		movdqa  xmm6, xmmword ptr [casebit]                 // bit to change
		mov     ecx, edi
		mov     edx, 7
		sub     esi, 14
		sub     edi, 14
		and     ecx, 1
		jnz     unaligned_xmmword_loop_entry
		add     ebx, 7
		jnc     aligned_xmmword_loop
		sub     ebx, 7

		align   16
	aligned_xmmword_loop_last:
		lea     ecx, [ebx + ebx]
		add     esi, 14
		add     ecx, esi
		add     edi, 14
		shl     ecx, 32 - PAGE_SHIFT
		xor     edx, edx
		cmp     ecx, -15 shl (32 - PAGE_SHIFT)
		jae     word_loop                                   // jump if cross pages

		align   16
	aligned_xmmword_loop:
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
		pmovmskb ecx, xmm0                                  // get one bit for each byte result
		xor     ecx, 0FFFFH
		jnz     xmmword_not_equal
		add     ebx, 8
		jnc     aligned_xmmword_loop
		sub     ebx, edx
		jb      aligned_xmmword_loop_last
		jmp     epilog

		align   8
	unaligned_xmmword_loop_entry:
		add     ebx, 7
		jnc     unaligned_xmmword_loop
		sub     ebx, 7

		align   16
	unaligned_xmmword_loop_last:
		lea     ecx, [ebx + ebx]
		add     esi, 14
		add     ecx, esi
		add     edi, 14
		shl     ecx, 32 - PAGE_SHIFT
		xor     edx, edx
		cmp     ecx, -15 shl (32 - PAGE_SHIFT)
		jae     word_loop                                   // jump if cross pages

		align   16
	unaligned_xmmword_loop:
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
		pmovmskb ecx, xmm0                                  // get one bit for each byte result
		xor     ecx, 0FFFFH
		jnz     xmmword_not_equal
		add     ebx, 8
		jnc     unaligned_xmmword_loop
		sub     ebx, edx
		jb      unaligned_xmmword_loop_last
		jmp     epilog

		align   16
	xmmword_not_equal:
		bsf     ecx, ecx
		sub     ebx, edx
		add     edx, edx
		shr     ecx, 1
		add     esi, edx
		add     ebx, ecx
		jc      epilog
		add     edi, edx
		xor     edx, edx
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
		pop     ebp
		pop     ebx
		ret

		#undef buffer1
		#undef buffer2
		#undef count
	}
}

// 80386 version
__declspec(naked) static int __cdecl wmemicmp386(const wchar_t *buffer1, const wchar_t *buffer2, size_t count)
{
	__asm
	{
		#define buffer1 (esp + 4)
		#define buffer2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		push    edi
		xor     eax, eax                                    // eax = 0
		mov     esi, dword ptr [buffer1 + 12]               // esi = buffer1
		mov     edi, dword ptr [buffer2 + 12]               // edi = buffer2
		mov     ecx, dword ptr [count + 12]                 // ecx = count
		xor     edx, edx                                    // edx = 0
		lea     esi, [esi + ecx * 2]                        // esi = end of buffer1
		lea     edi, [edi + ecx * 2]                        // edi = end of buffer2
		xor     ecx, -1                                     // ecx = -count - 1

		align   16
	loop_begin:
		inc     ecx
		jz      epilog
		mov     ax, word ptr [esi + ecx * 2]
		mov     dx, word ptr [edi + ecx * 2]
		sub     eax, edx
		jz      loop_begin
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

		#undef buffer1
		#undef buffer2
		#undef count
	}
}

__declspec(naked) static int __cdecl wmemicmpCPUDispatch(const wchar_t *buffer1, const wchar_t *buffer2, size_t count)
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	static void *table[] = {
		(void *)wmemicmp386,
		(void *)wmemicmpSSE2,
		(void *)wmemicmpSSE2
	};

	__asm
	{
		mov     ecx, dword ptr [__isa_available]
		mov     eax, offset wmemicmpAVX2
		cmp     ecx, __ISA_AVAILABLE_AVX2
		jae     L1
		mov     eax, dword ptr [table + ecx * 4]
	L1:
		mov     dword ptr [wmemicmpDispatch], eax
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
