#ifndef _M_IX86
#include <ctype.h>

int __cdecl _stricmp(const char *string1, const char *string2)
{
	int ret, c;

	do
		if (ret = tolower((unsigned char)*(string1++)) - (c = tolower((unsigned char)*(string2++))))
			break;
	while (c);
	return ret;
}
#else
#include "page.h"

static int __cdecl stricmpAVX2(const char *string1, const char *string2);
static int __cdecl stricmpSSE2(const char *string1, const char *string2);
static int __cdecl stricmp386(const char *string1, const char *string2);
static int __cdecl stricmpCPUDispatch(const char *string1, const char *string2);

static int(__cdecl * stricmpDispatch)(const char *string1, const char *string2) = stricmpCPUDispatch;

extern const char ymmconst_upperA[32];
extern const char ymmconst_azrangeA[32];
extern const char ymmconst_casebitA[32];
#define upper   ymmconst_upperA
#define azrange ymmconst_azrangeA
#define casebit ymmconst_casebitA

__declspec(naked) int __cdecl _stricmp(const char *string1, const char *string2)
{
	__asm
	{
		jmp     dword ptr [stricmpDispatch]
	}
}

// AVX2 version
__declspec(naked) static int __cdecl stricmpAVX2(const char *string1, const char *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 8]                // esi = string1
		mov     edi, dword ptr [string2 + 8]                // edi = string2
		mov     ecx, edi                                    // ecx = string2
		sub     esi, edi                                    // esi = string1 - string2
		vmovdqa ymm4, ymmword ptr [upper]
		vmovdqa ymm5, ymmword ptr [azrange]
		vmovdqa ymm6, ymmword ptr [casebit]                 // bit to change
		vpxor   ymm7, ymm7, ymm7                            // set to zero
		jmp     byte_loop_entry

		align   16
	byte_loop:
		movzx   eax, byte ptr [esi + edi]
		movzx   edx, byte ptr [edi]
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
		lea     ecx, [edi + 1]
		inc     edi
	byte_loop_entry:
		and     ecx, 15
		jnz     byte_loop
		lea     edx, [esi + edi]
		mov     ecx, edi
		and     edx, PAGE_SIZE - 1
		and     ecx, 31
		jz      ymmword_loop
	xmmword_check_cross_pages:
		cmp     edx, PAGE_SIZE - 15
		jae     byte_loop                                   // jump if cross pages
		vmovdqu xmm2, xmmword ptr [esi + edi]               // load 16 byte
		vmovdqa xmm3, xmmword ptr [edi]                     //
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
		vpmovmskb eax, xmm0                                 // get one bit for each byte result
		vpmovmskb ecx, xmm2                                 //
		xor     eax, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     epilog
		add     edx, 16
		add     edi, 16
		and     edx, PAGE_SIZE - 1

		align   16
	ymmword_loop:
		cmp     edx, PAGE_SIZE - 31
		jae     xmmword_check_cross_pages                   // jump if cross pages
		vmovdqu ymm2, ymmword ptr [esi + edi]               // load 32 byte
		vmovdqa ymm3, ymmword ptr [edi]                     //
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
		vpmovmskb eax, ymm0                                 // get one bit for each byte result
		vpmovmskb ecx, ymm2                                 //
		xor     eax, -1
		jnz     ymmword_not_equal
		test    ecx, ecx
		jnz     epilog
		add     edx, 32
		add     edi, 32
		and     edx, PAGE_SIZE - 1
		jmp     ymmword_loop

		align   16
	xmmword_not_equal:
		test    ecx, ecx
		jz      ymmword_has_not_null
		bsf     ecx, ecx
		mov     edx, 0FFFFH
		xor     ecx, 15
		shr     edx, cl
		and     eax, edx
		jz      epilog
		jmp     ymmword_has_not_null

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
		movzx   eax, byte ptr [esi + edi]
		movzx   edx, byte ptr [edi]
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
__declspec(naked) static int __cdecl stricmpSSE2(const char *string1, const char *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 8]                // esi = string1
		mov     edi, dword ptr [string2 + 8]                // edi = string2
		mov     ecx, edi                                    // ecx = string2
		sub     esi, edi                                    // esi = string1 - string2
		movdqa  xmm4, xmmword ptr [upper]
		movdqa  xmm5, xmmword ptr [azrange]
		movdqa  xmm6, xmmword ptr [casebit]                 // bit to change
		pxor    xmm7, xmm7                                  // set to zero
		jmp     byte_loop_entry

		align   16
	byte_loop:
		movzx   eax, byte ptr [esi + edi]
		movzx   edx, byte ptr [edi]
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
		lea     ecx, [edi + 1]
		inc     edi
	byte_loop_entry:
		and     ecx, 15
		jnz     byte_loop
		lea     edx, [esi + edi]
		and     edx, PAGE_SIZE - 1

		align   16
	xmmword_loop:
		cmp     edx, PAGE_SIZE - 15
		jae     byte_loop                                   // jump if cross pages
		movdqu  xmm0, xmmword ptr [esi + edi]               // load 16 byte
		movdqa  xmm1, xmmword ptr [edi]                     //
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
		pmovmskb eax, xmm0                                  // get one bit for each byte result
		pmovmskb ecx, xmm2                                  //
		xor     eax, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     epilog
		add     edx, 16
		add     edi, 16
		and     edx, PAGE_SIZE - 1
		jmp     xmmword_loop

		align   16
	xmmword_not_equal:
		test    ecx, ecx
		jz      xmmword_has_not_null
		bsf     ecx, ecx
		mov     edx, 0FFFFH
		xor     ecx, 15
		shr     edx, cl
		and     eax, edx
		jz      epilog
	xmmword_has_not_null:
		bsf     eax, eax
		add     edi, eax
		movzx   eax, byte ptr [esi + edi]
		movzx   edx, byte ptr [edi]
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
__declspec(naked) static int __cdecl stricmp386(const char *string1, const char *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    esi
		xor     eax, eax                                    // eax = 0
		mov     esi, dword ptr [string1 + 4]                // esi = string1
		mov     ecx, dword ptr [string2 + 4]                // ecx = string2
		sub     esi, ecx                                    // esi = string1 - string2

		align   16
	loop_begin:
		xor     edx, edx
		mov     al, byte ptr [ecx + esi]
		mov     dl, byte ptr [ecx]
		inc     ecx
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

__declspec(naked) static int __cdecl stricmpCPUDispatch(const char *string1, const char *string2)
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
		mov     dword ptr [stricmpDispatch], offset stricmpAVX2
		jmp     stricmpAVX2

	L1:
		test    eax, eax
		jz      L2
		mov     dword ptr [stricmpDispatch], offset stricmpSSE2
		jmp     stricmpSSE2

	L2:
		mov     dword ptr [stricmpDispatch], offset stricmp386
		jmp     stricmp386
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
	#undef __ISA_AVAILABLE_AVX
	#undef __ISA_AVAILABLE_ENFSTRG
	#undef __ISA_AVAILABLE_AVX2
}
#endif
