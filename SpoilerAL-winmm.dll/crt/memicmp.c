#include <stddef.h>
#ifndef _M_IX86
#include <ctype.h>

int __cdecl _memicmp(const void *buffer1, const void *buffer2, size_t count)
{
	int ret;

	(char *)buffer1 += count;
	(char *)buffer2 += count;
	count ^= -1;
	while (++count)
		if (ret = tolower(((unsigned char *)buffer1)[count]) - tolower(((unsigned char *)buffer2)[count]))
			return ret;
	return 0;
}
#else
#include "page.h"

static int __cdecl memicmpAVX2(const void *buffer1, const void *buffer2, size_t count);
static int __cdecl memicmpSSE2(const void *buffer1, const void *buffer2, size_t count);
static int __cdecl memicmp386(const void *buffer1, const void *buffer2, size_t count);
static int __cdecl memicmpCPUDispatch(const void *buffer1, const void *buffer2, size_t count);

static int(__cdecl * memicmpDispatch)(const void *buffer1, const void *buffer2, size_t count) = memicmpCPUDispatch;

extern const char ymmconst_upperA[32];
extern const char ymmconst_azrangeA[32];
extern const char ymmconst_casebitA[32];
#define upper   ymmconst_upperA
#define azrange ymmconst_azrangeA
#define casebit ymmconst_casebitA

__declspec(naked) int __cdecl _memicmp(const void *buffer1, const void *buffer2, size_t count)
{
	__asm
	{
		jmp     dword ptr [memicmpDispatch]
	}
}

// AVX2 version
__declspec(naked) static int __cdecl memicmpAVX2(const void *buffer1, const void *buffer2, size_t count)
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
		add     esi, ebx                                    // esi = end of buffer1
		add     edi, ebx                                    // edi = end of buffer2
		xor     ebx, -1                                     // ebx = -count - 1
		and     ebp, 15                                     // ebp = -buffer2 & 15
		xor     eax, eax                                    // eax = 0
		jmp     byte_loop_entry

		align   16
	byte_loop:
		movzx   eax, byte ptr [esi + ebx]
		movzx   edx, byte ptr [edi + ebx]
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
	byte_loop_entry:
		inc     ebx
		jz      epilog
		dec     ebp
		jns     byte_loop
		vmovdqa ymm4, ymmword ptr [upper]
		vmovdqa ymm5, ymmword ptr [azrange]
		vmovdqa ymm6, ymmword ptr [casebit]                 // bit to change
		shr     ebp, 1
		lea     edx, [edi + ebx]
		and     edx, 31
		jz      ymmword_entry
		and     edx, 15
		jz      xmmword_entry
		xor     edx, edx
		lea     ecx, [esi + ebx]
		shl     ecx, 32 - PAGE_SHIFT
	qword_check_cross_pages:
		cmp     ebx, -8
		jae     qword_compare
		cmp     ecx, -7 shl (32 - PAGE_SHIFT)
		jae     byte_loop                                   // jump if cross pages
	qword_compare:
		vmovq   xmm2, qword ptr [esi + ebx]                 // load 8 byte
		vmovq   xmm3, qword ptr [edi + ebx]                 //
		vpaddb  xmm0, xmm2, xmm4                            // all bytes greater than 'Z' if negative
		vpaddb  xmm1, xmm3, xmm4                            //
		vpcmpgtb xmm0, xmm0, xmm5                           // xmm0 = (byte >= 'A' && byte <= 'Z') ? 0xFF : 0x00
		vpcmpgtb xmm1, xmm1, xmm5                           //
		vpand   xmm0, xmm0, xmm6                            // assign a mask for the appropriate bytes
		vpand   xmm1, xmm1, xmm6                            //
		vpor    xmm0, xmm0, xmm2                            // negation of the 5th bit - lowercase letters
		vpor    xmm1, xmm1, xmm3                            //
		vpcmpeqb xmm0, xmm0, xmm1                           // compare
		vpmovmskb ecx, xmm0                                 // get one bit for each byte result
		xor     ecx, 0FFFFH
		jnz     ymmword_not_equal
		add     ebx, 8
		jc      epilog
		lea     ecx, [edi + ebx]
		and     ecx, 31
		jz      ymmword_entry
	xmmword_entry:
		xor     edx, edx
		lea     ecx, [esi + ebx]
		shl     ecx, 32 - PAGE_SHIFT
	xmmword_check_cross_pages:
		cmp     ebx, -16
		jae     xmmword_compare
		cmp     ecx, -15 shl (32 - PAGE_SHIFT)
		jae     qword_check_cross_pages                     // jump if cross pages
	xmmword_compare:
		vmovdqu xmm2, xmmword ptr [esi + ebx]               // load 16 byte
		vmovdqa xmm3, xmmword ptr [edi + ebx]               //
		vpaddb  xmm0, xmm2, xmm4                            // all bytes greater than 'Z' if negative
		vpaddb  xmm1, xmm3, xmm4                            //
		vpcmpgtb xmm0, xmm0, xmm5                           // xmm0 = (byte >= 'A' && byte <= 'Z') ? 0xFF : 0x00
		vpcmpgtb xmm1, xmm1, xmm5                           //
		vpand   xmm0, xmm0, xmm6                            // assign a mask for the appropriate bytes
		vpand   xmm1, xmm1, xmm6                            //
		vpor    xmm0, xmm0, xmm2                            // negation of the 5th bit - lowercase letters
		vpor    xmm1, xmm1, xmm3                            //
		vpcmpeqb xmm0, xmm0, xmm1                           // compare
		vpmovmskb ecx, xmm0                                 // get one bit for each byte result
		xor     ecx, 0FFFFH
		jnz     ymmword_not_equal
		add     ebx, 16
		jc      epilog
	ymmword_entry:
		add     edx, 31
		sub     esi, 31
		sub     edi, 31
		add     ebx, 31
		jnc     ymmword_loop
		sub     ebx, 31

		align   16
	ymmword_loop_last:
		mov     ecx, ebx
		add     esi, 31
		add     ecx, esi
		add     edi, 31
		shl     ecx, 32 - PAGE_SHIFT
		xor     edx, edx
		cmp     ecx, -31 shl (32 - PAGE_SHIFT)
		jae     xmmword_check_cross_pages                   // jump if cross pages

		align   16
	ymmword_loop:
		vmovdqu ymm2, ymmword ptr [esi + ebx]               // load 32 byte
		vmovdqa ymm3, ymmword ptr [edi + ebx]               //
		vpaddb  ymm0, ymm2, ymm4                            // all bytes greater than 'Z' if negative
		vpaddb  ymm1, ymm3, ymm4                            //
		vpcmpgtb ymm0, ymm0, ymm5                           // ymm0 = (byte >= 'A' && byte <= 'Z') ? 0xFF : 0x00
		vpcmpgtb ymm1, ymm1, ymm5                           //
		vpand   ymm0, ymm0, ymm6                            // assign a mask for the appropriate bytes
		vpand   ymm1, ymm1, ymm6                            //
		vpor    ymm0, ymm0, ymm2                            // negation of the 5th bit - lowercase letters
		vpor    ymm1, ymm1, ymm3                            //
		vpcmpeqb ymm0, ymm0, ymm1                           // compare
		vpmovmskb ecx, ymm0                                 // get one bit for each byte result
		xor     ecx, -1
		jnz     ymmword_not_equal
		add     ebx, 32
		jnc     ymmword_loop
		sub     ebx, edx
		jb      ymmword_loop_last
		jmp     epilog

		align   16
	ymmword_not_equal:
		bsf     ecx, ecx
		sub     ebx, edx
		add     esi, edx
		add     ebx, ecx
		jc      epilog
		add     edi, edx
		xor     edx, edx
		mov     al, byte ptr [esi + ebx]
		mov     dl, byte ptr [edi + ebx]
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
__declspec(naked) static int __cdecl memicmpSSE2(const void *buffer1, const void *buffer2, size_t count)
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
		add     esi, ebx                                    // esi = end of buffer1
		add     edi, ebx                                    // edi = end of buffer2
		xor     ebx, -1                                     // ebx = -count - 1
		and     ebp, 7                                      // ebp = -buffer2 & 7
		xor     eax, eax                                    // eax = 0
		jmp     byte_loop_entry

		align   16
	byte_loop:
		movzx   eax, byte ptr [esi + ebx]
		movzx   edx, byte ptr [edi + ebx]
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
	byte_loop_entry:
		inc     ebx
		jz      epilog
		dec     ebp
		jns     byte_loop
		movdqa  xmm4, xmmword ptr [upper]
		movdqa  xmm5, xmmword ptr [azrange]
		movdqa  xmm6, xmmword ptr [casebit]                 // bit to change
		shr     ebp, 1
		lea     edx, [edi + ebx]
		and     edx, 15
		jz      xmmword_entry
		xor     edx, edx
		lea     ecx, [esi + ebx]
		shl     ecx, 32 - PAGE_SHIFT
	qword_check_cross_pages:
		cmp     ebx, -8
		jae     qword_compare
		cmp     ecx, -7 shl (32 - PAGE_SHIFT)
		jae     byte_loop                                   // jump if cross pages
	qword_compare:
		movq    xmm0, qword ptr [esi + ebx]                 // load 8 byte
		movq    xmm1, qword ptr [edi + ebx]                 //
		movdqa  xmm2, xmm0                                  // copy
		movdqa  xmm3, xmm1                                  //
		paddb   xmm0, xmm4                                  // all bytes greater than 'Z' if negative
		paddb   xmm1, xmm4                                  //
		pcmpgtb xmm0, xmm5                                  // xmm0 = (byte >= 'A' && byte <= 'Z') ? 0xFF : 0x00
		pcmpgtb xmm1, xmm5                                  //
		pand    xmm0, xmm6                                  // assign a mask for the appropriate bytes
		pand    xmm1, xmm6                                  //
		por     xmm0, xmm2                                  // negation of the 5th bit - lowercase letters
		por     xmm1, xmm3                                  //
		pcmpeqb xmm0, xmm1                                  // compare
		pmovmskb ecx, xmm0                                  // get one bit for each byte result
		xor     ecx, 0FFFFH
		jnz     xmmword_not_equal
		add     ebx, 8
		jc      epilog
	xmmword_entry:
		add     edx, 15
		sub     esi, 15
		sub     edi, 15
		add     ebx, 15
		jnc     xmmword_loop
		sub     ebx, 15

		align   16
	xmmword_loop_last:
		mov     ecx, ebx
		add     esi, 15
		add     ecx, esi
		add     edi, 15
		shl     ecx, 32 - PAGE_SHIFT
		xor     edx, edx
		cmp     ecx, -15 shl (32 - PAGE_SHIFT)
		jae     qword_check_cross_pages                     // jump if cross pages

		align   16
	xmmword_loop:
		movdqu  xmm0, xmmword ptr [esi + ebx]               // load 16 byte
		movdqa  xmm1, xmmword ptr [edi + ebx]               //
		movdqa  xmm2, xmm0                                  // copy
		movdqa  xmm3, xmm1                                  //
		paddb   xmm0, xmm4                                  // all bytes greater than 'Z' if negative
		paddb   xmm1, xmm4                                  //
		pcmpgtb xmm0, xmm5                                  // xmm0 = (byte >= 'A' && byte <= 'Z') ? 0xFF : 0x00
		pcmpgtb xmm1, xmm5                                  //
		pand    xmm0, xmm6                                  // assign a mask for the appropriate bytes
		pand    xmm1, xmm6                                  //
		por     xmm0, xmm2                                  // negation of the 5th bit - lowercase letters
		por     xmm1, xmm3                                  //
		pcmpeqb xmm0, xmm1                                  // compare
		pmovmskb ecx, xmm0                                  // get one bit for each byte result
		xor     ecx, 0FFFFH
		jnz     xmmword_not_equal
		add     ebx, 16
		jnc     xmmword_loop
		sub     ebx, edx
		jb      xmmword_loop_last
		jmp     epilog

		align   16
	xmmword_not_equal:
		bsf     ecx, ecx
		sub     ebx, edx
		add     esi, edx
		add     ebx, ecx
		jc      epilog
		add     edi, edx
		xor     edx, edx
		mov     al, byte ptr [esi + ebx]
		mov     dl, byte ptr [edi + ebx]
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
__declspec(naked) static int __cdecl memicmp386(const void *buffer1, const void *buffer2, size_t count)
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
		add     esi, ecx                                    // esi = end of buffer1
		add     edi, ecx                                    // edi = end of buffer2
		xor     ecx, -1                                     // ecx = -count - 1

		align   16
	loop_begin:
		inc     ecx
		jz      epilog
		mov     al, byte ptr [esi + ecx]
		mov     dl, byte ptr [edi + ecx]
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

__declspec(naked) static int __cdecl memicmpCPUDispatch(const void *buffer1, const void *buffer2, size_t count)
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	static void *table[] = {
		(void *)memicmp386,
		(void *)memicmpSSE2,
		(void *)memicmpSSE2,
		(void *)memicmpSSE2,
		(void *)memicmpSSE2,
	};

	__asm
	{
		mov     ecx, dword ptr [__isa_available]
		mov     eax, offset memicmpAVX2
		cmp     ecx, __ISA_AVAILABLE_AVX2
		jae     L1
		mov     eax, dword ptr [table + ecx * 4]
	L1:
		mov     dword ptr [memicmpDispatch], eax
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
