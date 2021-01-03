#include <stddef.h>
#ifndef _M_IX86
#include <ctype.h>

int __cdecl _strnicmp(const char *string1, const char *string2, size_t count)
{
	int ret, c;

	string1 += count;
	string2 += count;
	count ^= -1;
	ret = 0;
	while (++count)
		if (ret = tolower((unsigned char)string1[count]) - (c = tolower((unsigned char)string2[count])))
			break;
		else if (!c)
			break;
	return ret;
}
#else
#include "page.h"

static int __cdecl strnicmpAVX2(const char *string1, const char *string2, size_t count);
static int __cdecl strnicmpSSE2(const char *string1, const char *string2, size_t count);
static int __cdecl strnicmp386(const char *string1, const char *string2, size_t count);
static int __cdecl strnicmpCPUDispatch(const char *string1, const char *string2, size_t count);

static int(__cdecl * strnicmpDispatch)(const char *string1, const char *string2, size_t count) = strnicmpCPUDispatch;

extern const char ymmconst_upperA[32];
extern const char ymmconst_azrangeA[32];
extern const char ymmconst_casebitA[32];
#define upper   ymmconst_upperA
#define azrange ymmconst_azrangeA
#define casebit ymmconst_casebitA

__declspec(naked) int __cdecl _strnicmp(const char *string1, const char *string2, size_t count)
{
	__asm
	{
		jmp     dword ptr [strnicmpDispatch]
	}
}

// AVX2 version
__declspec(naked) static int __cdecl strnicmpAVX2(const char *string1, const char *string2, size_t count)
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
		add     edi, ebx                                    // edi = end of string2
		add     esi, ebx                                    // esi = end of string1
		xor     ebx, -1                                     // ebx = -count - 1
		vmovdqa ymm4, ymmword ptr [upper]
		vmovdqa ymm5, ymmword ptr [azrange]
		vmovdqa ymm6, ymmword ptr [casebit]                 // bit to change
		vpxor   ymm7, ymm7, ymm7                            // set to zero
		jmp     byte_loop_increment

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
		cmp     edx, '\0' - 'A'
		je      epilog
	byte_loop_increment:
		inc     ebx
		jz      epilog
		lea     edx, [edi + ebx]
		lea     ecx, [esi + ebx]
		and     edx, 15
		jnz     byte_loop
		shl     ecx, 32 - PAGE_SHIFT
		lea     edx, [edi + ebx]
		and     edx, 31
		jz      ymmword_loop
	xmmword_check_cross_pages:
		cmp     ecx, -15 shl (32 - PAGE_SHIFT)
		jae     byte_loop                                   // jump if cross pages
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
		vpcmpeqb xmm2, xmm2, xmm7                           //
		vpmovmskb edx, xmm0                                 // get one bit for each byte result
		vpmovmskb ecx, xmm2                                 //
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		add     ebx, 16
		jc      epilog
		test    ecx, ecx
		jnz     epilog
		lea     ecx, [esi + ebx]
		shl     ecx, 32 - PAGE_SHIFT

		align   16
	ymmword_loop:
		cmp     ecx, -31 shl (32 - PAGE_SHIFT)
		jae     xmmword_check_cross_pages                   // jump if cross pages
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
		vpcmpeqb ymm2, ymm2, ymm7                           //
		vpmovmskb edx, ymm0                                 // get one bit for each byte result
		vpmovmskb ecx, ymm2                                 //
		xor     edx, -1
		jnz     ymmword_not_equal
		add     ebx, 32
		jc      epilog
		test    ecx, ecx
		jnz     epilog
		lea     ecx, [esi + ebx]
		shl     ecx, 32 - PAGE_SHIFT
		jmp     ymmword_loop

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
		add     ebx, edx
		jc      epilog
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
__declspec(naked) static int __cdecl strnicmpSSE2(const char *string1, const char *string2, size_t count)
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
		add     edi, ebx                                    // edi = end of string2
		add     esi, ebx                                    // esi = end of string1
		xor     ebx, -1                                     // ebx = -count - 1
		movdqa  xmm4, xmmword ptr [upper]
		movdqa  xmm5, xmmword ptr [azrange]
		movdqa  xmm6, xmmword ptr [casebit]                 // bit to change
		pxor    xmm7, xmm7                                  // set to zero
		jmp     byte_loop_increment

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
		cmp     edx, '\0' - 'A'
		je      epilog
	byte_loop_increment:
		inc     ebx
		jz      epilog
		lea     edx, [edi + ebx]
		lea     ecx, [esi + ebx]
		and     edx, 15
		jnz     byte_loop
		shl     ecx, 32 - PAGE_SHIFT

		align   16
	xmmword_loop:
		cmp     ecx, -15 shl (32 - PAGE_SHIFT)
		jae     byte_loop                                   // jump if cross pages
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
		pcmpeqb xmm2, xmm7                                  //
		pmovmskb edx, xmm0                                  // get one bit for each byte result
		pmovmskb ecx, xmm2                                  //
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		add     ebx, 16
		jc      epilog
		test    ecx, ecx
		jnz     epilog
		lea     ecx, [esi + ebx]
		shl     ecx, 32 - PAGE_SHIFT
		jmp     xmmword_loop

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
		add     ebx, edx
		jc      epilog
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
__declspec(naked) static int __cdecl strnicmp386(const char *string1, const char *string2, size_t count)
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
		add     esi, ecx                                    // esi = end of string1
		add     edi, ecx                                    // edi = end of string2
		xor     ecx, -1                                     // ecx = -count - 1

		align   16
	loop_begin:
		inc     ecx
		jz      return_equal
		mov     al, byte ptr [esi + ecx]
		mov     dl, byte ptr [edi + ecx]
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

__declspec(naked) static int __cdecl strnicmpCPUDispatch(const char *string1, const char *string2, size_t count)
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	static void *table[] = {
		(void *)strnicmp386,
		(void *)strnicmpSSE2,
		(void *)strnicmpSSE2
	};

	__asm
	{
		mov     ecx, dword ptr [__isa_available]
		mov     eax, offset strnicmpAVX2
		cmp     ecx, __ISA_AVAILABLE_AVX2
		jae     L1
		mov     eax, dword ptr [table + ecx * 4]
	L1:
		mov     dword ptr [strnicmpDispatch], eax
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
