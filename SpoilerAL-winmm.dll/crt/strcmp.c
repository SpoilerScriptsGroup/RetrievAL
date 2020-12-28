#pragma function(strcmp)

#ifndef _M_IX86
int __cdecl strcmp(const char *string1, const char *string2)
{
	unsigned char c1, c2;

	do
	{
		c1 = *(string1++);
		c2 = *(string2++);
	} while (c1 && c1 == c2);
	return (int)c1 - (int)c2;
}
#else
#include "page.h"

static int __cdecl strcmpAVX2(const char *string1, const char *string2);
static int __cdecl strcmpSSE42(const char *string1, const char *string2);
static int __cdecl strcmpSSE2(const char *string1, const char *string2);
static int __cdecl strcmp386(const char *string1, const char *string2);
static int __cdecl strcmpCPUDispatch(const char *string1, const char *string2);

// Pointer to appropriate version. Initially points to dispatcher
static int(__cdecl * strcmpDispatch)(const char *string1, const char *string2) = strcmpCPUDispatch;

// strcmp function
__declspec(naked) int __cdecl strcmp(const char *string1, const char *string2)
{
	__asm
	{
		jmp     dword ptr [strcmpDispatch]                  // Go to appropriate version, depending on instruction set
	}
}

// AVX2 version
__declspec(naked) static int __cdecl strcmpAVX2(const char *string1, const char *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 8]                // esi = string1
		mov     eax, dword ptr [string2 + 8]                // eax = string2
		sub     esi, eax                                    // esi = string1 - string2
		pxor    xmm2, xmm2
		jmp     byte_loop_entry

		align   16
	byte_loop:
		mov     cl, byte ptr [eax + esi]
		mov     dl, byte ptr [eax]
		cmp     cl, dl
		jne     return_not_equal
		test    cl, cl
		jz      return_equal
		inc     eax
	byte_loop_entry:
		test    eax, 3                                      // use only eax for 'test reg, imm'
		jnz     byte_loop
		lea     edi, [eax + esi]
		and     edi, PAGE_SIZE - 1

		align   16
	dword_loop:
		test    eax, 15                                     // use only eax for 'test reg, imm'
		jz      xmmword_compare
	dword_check_cross_pages:
		cmp     edi, PAGE_SIZE - 3
		jae     byte_loop                                   // jump if cross pages
		mov     ecx, dword ptr [eax + esi]
		mov     edx, dword ptr [eax]
		cmp     ecx, edx
		jne     byte_loop                                   // not equal
		sub     ecx, 01010101H
		xor     edx, -1
		and     ecx, 80808080H
		add     edi, 4
		add     eax, 4
		and     edi, PAGE_SIZE - 1
		and     ecx, edx
		jz      dword_loop
	return_equal:
		xor     eax, eax                                    // strings are equal
		pop     edi
		pop     esi
		vzeroupper
		ret

		align   16
	return_not_equal:
		sbb     eax, eax
		pop     edi
		or      eax, 1
		pop     esi
		vzeroupper
		ret

		align   16
	xmmword_compare:
		test    eax, 31                                     // use only eax for 'test reg, imm'
		jz      ymmword_loop
	xmmword_check_cross_pages:
		cmp     edi, PAGE_SIZE - 15
		jae     dword_check_cross_pages                     // jump if cross pages
		movdqu  xmm0, xmmword ptr [eax + esi]
		movdqa  xmm1, xmmword ptr [eax]
		pcmpeqb xmm0, xmm1
		pcmpeqb xmm1, xmm2
		pmovmskb edx, xmm0
		pmovmskb ecx, xmm1
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     return_equal
		add     edi, 16
		add     eax, 16
		and     edi, PAGE_SIZE - 1

		align   16
	ymmword_loop:
		cmp     edi, PAGE_SIZE - 31
		jae     xmmword_check_cross_pages                   // jump if cross pages
		vmovdqu ymm0, ymmword ptr [eax + esi]
		vmovdqa ymm1, ymmword ptr [eax]
		vpcmpeqb ymm0, ymm0, ymm1
		vpcmpeqb ymm1, ymm1, ymm2
		vpmovmskb edx, ymm0
		vpmovmskb ecx, ymm1
		xor     edx, -1
		jnz     ymmword_not_equal
		test    ecx, ecx
		jnz     return_equal
		add     edi, 32
		add     eax, 32
		and     edi, PAGE_SIZE - 1
		jmp     ymmword_loop

		align   16
	xmmword_not_equal:
		test    ecx, ecx
		jz      ymmword_has_not_null
		bsf     ecx, ecx
		mov     edi, 0FFFFH
		xor     ecx, 15
		shr     edi, cl
		and     edx, edi
		jz      return_equal
		jmp     ymmword_has_not_null

		align   16
	ymmword_not_equal:
		test    ecx, ecx
		jz      ymmword_has_not_null
		bsf     ecx, ecx
		or      edi, -1
		xor     ecx, 31
		shr     edi, cl
		and     edx, edi
		jz      return_equal
	ymmword_has_not_null:
		bsf     edx, edx
		add     edx, eax
		xor     eax, eax
		xor     ecx, ecx
		pop     edi
		mov     al, byte ptr [edx + esi]
		mov     cl, byte ptr [edx]
		sub     eax, ecx
		pop     esi
		vzeroupper
		ret

		#undef string1
		#undef string2
	}
}

// SSE4.2 version
__declspec(naked) static int __cdecl strcmpSSE42(const char *string1, const char *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 8]                // esi = string1
		mov     eax, dword ptr [string2 + 8]                // eax = string2
		sub     esi, eax                                    // esi = string1 - string2
		jmp     byte_loop_entry

		align   16
	byte_loop:
		mov     cl, byte ptr [eax + esi]
		mov     dl, byte ptr [eax]
		cmp     cl, dl
		jne     return_not_equal
		test    cl, cl
		jz      return_equal
		inc     eax
	byte_loop_entry:
		test    eax, 3                                      // use only eax for 'test reg, imm'
		jnz     byte_loop
		lea     edi, [eax + esi]
		and     edi, PAGE_SIZE - 1

		align   16
	dword_loop:
		test    eax, 15                                     // use only eax for 'test reg, imm'
		jz      xmmword_loop
	dword_check_cross_pages:
		cmp     edi, PAGE_SIZE - 3
		jae     byte_loop                                   // jump if cross pages
		mov     ecx, dword ptr [eax + esi]
		mov     edx, dword ptr [eax]
		cmp     ecx, edx
		jne     byte_loop                                   // not equal
		sub     ecx, 01010101H
		xor     edx, -1
		and     ecx, 80808080H
		add     edi, 4
		add     eax, 4
		and     edi, PAGE_SIZE - 1
		and     ecx, edx
		jz      dword_loop
	return_equal:
		xor     eax, eax                                    // strings are equal
		pop     edi
		pop     esi
		ret

		align   16
	return_not_equal:
		sbb     eax, eax
		pop     edi
		or      eax, 1
		pop     esi
		ret

		align   16
	xmmword_loop:
		cmp     edi, PAGE_SIZE - 15
		jae     dword_check_cross_pages                     // jump if cross pages
		movdqu  xmm0, xmmword ptr [eax + esi]               // read 16 bytes of string1
		pcmpistri xmm0, xmmword ptr [eax], 00011000B        // unsigned bytes, equal each, invert. returns index in ecx
		jbe     xmmword_break                               // jump if carry flag or zero flag
		add     edi, 16
		add     eax, 16
		and     edi, PAGE_SIZE - 1
		jmp     xmmword_loop

		align   16
	xmmword_break:
		jnc     return_equal
		// strings are not equal
		add     ecx, eax                                    // offset to first differing byte
		pop     edi
		movzx   eax, byte ptr [ecx + esi]                   // compare bytes
		movzx   edx, byte ptr [ecx]
		sub     eax, edx
		pop     esi
		ret

		#undef string1
		#undef string2
	}
}

// SSE2 version
__declspec(naked) static int __cdecl strcmpSSE2(const char *string1, const char *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 8]                // esi = string1
		mov     eax, dword ptr [string2 + 8]                // eax = string2
		sub     esi, eax                                    // esi = string1 - string2
		pxor    xmm2, xmm2
		jmp     byte_loop_entry

		align   16
	byte_loop:
		mov     cl, byte ptr [eax + esi]
		mov     dl, byte ptr [eax]
		cmp     cl, dl
		jne     return_not_equal
		test    cl, cl
		jz      return_equal
		inc     eax
	byte_loop_entry:
		test    eax, 3                                      // use only eax for 'test reg, imm'
		jnz     byte_loop
		lea     edi, [eax + esi]
		and     edi, PAGE_SIZE - 1

		align   16
	dword_loop:
		test    eax, 15                                     // use only eax for 'test reg, imm'
		jz      xmmword_loop
	dword_check_cross_pages:
		cmp     edi, PAGE_SIZE - 3
		jae     byte_loop                                   // jump if cross pages
		mov     ecx, dword ptr [eax + esi]
		mov     edx, dword ptr [eax]
		cmp     ecx, edx
		jne     byte_loop                                   // not equal
		sub     ecx, 01010101H
		xor     edx, -1
		and     ecx, 80808080H
		add     edi, 4
		add     eax, 4
		and     edi, PAGE_SIZE - 1
		and     ecx, edx
		jz      dword_loop
	return_equal:
		xor     eax, eax                                    // strings are equal
		pop     edi
		pop     esi
		ret

		align   16
	return_not_equal:
		sbb     eax, eax
		pop     edi
		or      eax, 1
		pop     esi
		ret

		align   16
	xmmword_loop:
		cmp     edi, PAGE_SIZE - 15
		jae     dword_check_cross_pages                     // jump if cross pages
		movdqu  xmm0, xmmword ptr [eax + esi]
		movdqa  xmm1, xmmword ptr [eax]
		pcmpeqb xmm0, xmm1
		pcmpeqb xmm1, xmm2
		pmovmskb edx, xmm0
		pmovmskb ecx, xmm1
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     return_equal
		add     edi, 16
		add     eax, 16
		and     edi, PAGE_SIZE - 1
		jmp     xmmword_loop

		align   16
	xmmword_not_equal:
		test    ecx, ecx
		jz      xmmword_has_not_null
		bsf     ecx, ecx
		mov     edi, 0FFFFH
		xor     ecx, 15
		shr     edi, cl
		and     edx, edi
		jz      return_equal
	xmmword_has_not_null:
		bsf     edx, edx
		add     edx, eax
		xor     eax, eax
		xor     ecx, ecx
		pop     edi
		mov     al, byte ptr [edx + esi]
		mov     cl, byte ptr [edx]
		sub     eax, ecx
		pop     esi
		ret

		#undef string1
		#undef string2
	}
}

// 80386 version
__declspec(naked) static int __cdecl strcmp386(const char *string1, const char *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 8]                // esi = string1
		mov     eax, dword ptr [string2 + 8]                // eax = string2
		sub     esi, eax                                    // esi = string1 - string2
		jmp     byte_loop_entry

		align   16
	byte_loop:
		mov     cl, byte ptr [eax + esi]
		mov     dl, byte ptr [eax]
		cmp     cl, dl
		jne     return_not_equal
		test    cl, cl
		jz      return_equal
		inc     eax
	byte_loop_entry:
		test    eax, 3                                      // use only eax for 'test reg, imm'
		jnz     byte_loop
		lea     edi, [eax + esi]
		and     edi, PAGE_SIZE - 1

		align   16
	dword_loop:
		cmp     edi, PAGE_SIZE - 3
		jae     byte_loop                                   // jump if cross pages
		mov     ecx, dword ptr [eax + esi]
		mov     edx, dword ptr [eax]
		cmp     ecx, edx
		jne     byte_loop                                   // not equal
		sub     ecx, 01010101H
		xor     edx, -1
		and     ecx, 80808080H
		add     edi, 4
		add     eax, 4
		and     edi, PAGE_SIZE - 1
		and     ecx, edx
		jz      dword_loop
	return_equal:
		xor     eax, eax
		pop     edi
		pop     esi
		ret

		align   16
	return_not_equal:
		sbb     eax, eax
		pop     edi
		or      eax, 1
		pop     esi
		ret

		#undef string1
		#undef string2
	}
}

// CPU dispatching for strcmp. This is executed only once
__declspec(naked) static int __cdecl strcmpCPUDispatch(const char *string1, const char *string2)
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
		mov     dword ptr [strcmpDispatch], offset strcmpAVX2
		jmp     strcmpAVX2

	L1:
		cmp     eax, __ISA_AVAILABLE_SSE2
		jbe     L2
		mov     dword ptr [strcmpDispatch], offset strcmpSSE42
		jmp     strcmpSSE42

	L2:
		mov     dword ptr [strcmpDispatch], offset strcmpSSE2
		jb      L3
		jmp     strcmpSSE2

	L3:
		mov     dword ptr [strcmpDispatch], offset strcmp386
		jmp     strcmp386
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
	#undef __ISA_AVAILABLE_AVX
	#undef __ISA_AVAILABLE_ENFSTRG
	#undef __ISA_AVAILABLE_AVX2
}
#endif
