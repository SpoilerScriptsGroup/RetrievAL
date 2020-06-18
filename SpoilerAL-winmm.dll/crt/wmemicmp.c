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
#include "PageSize.h"

static int __cdecl wmemicmpSSE2(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
static int __cdecl wmemicmp386(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
static int __cdecl wmemicmpCPUDispatch(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);

static int(__cdecl * wmemicmpDispatch)(const wchar_t *buffer1, const wchar_t *buffer2, size_t count) = wmemicmpCPUDispatch;

extern const wchar_t xmmconst_upperW[8];
extern const wchar_t xmmconst_azrangeW[8];
extern const wchar_t xmmconst_casebitW[8];
#define upper   xmmconst_upperW
#define azrange xmmconst_azrangeW
#define casebit xmmconst_casebitW

__declspec(naked) int __cdecl _wmemicmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count)
{
	__asm
	{
		jmp     dword ptr [wmemicmpDispatch]
	}
}

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
		and     ebp, 14                                     // ebp = -buffer2 & 15
		xor     eax, eax                                    // eax = 0
		movdqa  xmm4, xmmword ptr [upper]
		movdqa  xmm5, xmmword ptr [azrange]
		movdqa  xmm6, xmmword ptr [casebit]                 // bit to change
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
		jnz     epilogue
	word_loop_entry:
		inc     ebx
		jz      epilogue
		sub     ebp, 2
		jae     word_loop
		mov     ecx, edi
		mov     edx, 7
		sub     esi, 14
		sub     edi, 14
		and     ecx, 1
		jnz     unaligned_xmmword_loop_entry
		add     ebx, edx
		jc      aligned_xmmword_loop_last

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
	aligned_xmmword_loop_last:
		sub     ebx, edx
		jae     epilogue
		lea     ecx, [ebx + ebx]
		add     esi, 14
		add     ecx, esi
		add     edi, 14
		and     ecx, PAGE_SIZE - 1
		xor     edx, edx
		cmp     ecx, PAGE_SIZE - 16
		jbe     aligned_xmmword_loop
		jmp     word_loop                                   // jump if cross pages

		align   8
		nop __asm nop __asm nop __asm nop                   // padding 4 byte
	unaligned_xmmword_loop_entry:
		add     ebx, edx
		jc      unaligned_xmmword_loop_last

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
	unaligned_xmmword_loop_last:
		sub     ebx, edx
		jae     epilogue
		lea     ecx, [ebx + ebx]
		add     esi, 14
		add     ecx, esi
		add     edi, 14
		and     ecx, PAGE_SIZE - 1
		xor     edx, edx
		cmp     ecx, PAGE_SIZE - 16
		jbe     unaligned_xmmword_loop
		jmp     word_loop                                   // jump if cross pages

		align   16
	xmmword_not_equal:
		bsf     ecx, ecx
		sub     ebx, edx
		add     edx, edx
		shr     ecx, 1
		add     esi, edx
		add     ebx, ecx
		jc      epilogue
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
	epilogue:
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
		jz      epilogue
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
		jmp     epilogue

		align   16
	compare_above:
		xor     eax, eax
		lea     ebx, [edx - 'A']
		cmp     ebx, 'Z' - 'A' + 1
		jb      loop_begin
		inc     eax
		jmp     epilogue

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
	epilogue:
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
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wmemicmpDispatch], offset wmemicmp386
		jmp     wmemicmp386

	L1:
		mov     dword ptr [wmemicmpDispatch], offset wmemicmpSSE2
		jmp     wmemicmpSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
