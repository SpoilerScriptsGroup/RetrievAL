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
#include "PageSize.h"

static int __cdecl strcmpSSE42(const char *string1, const char *string2);
static int __cdecl strcmpSSE2(const char *string1, const char *string2);
static int __cdecl strcmpGeneric(const char *string1, const char *string2);
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

// SSE4.2 version
__declspec(naked) static int __cdecl strcmpSSE42(const char *string1, const char *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    esi
		push    edi
		mov     eax, dword ptr [string1 + 8]                // esi = string1
		mov     esi, dword ptr [string2 + 8]                // eax = string2
		sub     esi, eax                                    // esi = string1 - string2
		jmp     byte_loop_entry

		align   16
	byte_loop:
		mov     cl, byte ptr [eax]
		mov     dl, byte ptr [eax + esi]
		cmp     cl, dl
		jne     return_not_equal
		test    cl, cl
		jz      return_equal
		lea     edi, [eax + esi + 1]
		inc     eax
	byte_loop_entry:
		test    eax, 3                                      // use only eax for 'test reg, imm'
		jnz     byte_loop
		and     edi, PAGE_SIZE - 1

		align   16
	dword_loop:
		test    eax, 15                                     // use only eax for 'test reg, imm'
		jz      xmmword_loop
	dword_check_cross_pages:
		cmp     edi, PAGE_SIZE - 4
		ja      byte_loop                                   // jump if cross pages
		mov     ecx, dword ptr [eax]
		mov     edx, dword ptr [eax + esi]
		cmp     ecx, edx
		jne     byte_loop                                   // not equal
		add     eax, 4
		sub     ecx, 01010101H
		xor     edx, -1
		lea     edi, [eax + esi]
		and     ecx, 80808080H
		and     edi, PAGE_SIZE - 1
		and     edx, ecx
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
		cmp     edi, PAGE_SIZE - 16
		ja      dword_check_cross_pages                     // jump if cross pages
		movdqa  xmm0, xmmword ptr [eax]                     // read 16 bytes of string 1
		pcmpistri xmm0, xmmword ptr [eax + esi], 00011000B      // unsigned bytes, equal each, invert. returns index in ecx
		jc      xmmword_not_equal
		jz      return_equal
		lea     edi, [eax + esi + 16]
		add     eax, 16
		and     edi, PAGE_SIZE - 1
		jmp     xmmword_loop

	xmmword_not_equal:
		// strings are not equal
		add     ecx, eax                                    // offset to first differing byte
		movzx   eax, byte ptr [ecx]                         // compare bytes
		movzx   edx, byte ptr [ecx + esi]
		sub     eax, edx
		pop     edi
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
		lea     edi, [eax + esi + 1]
		inc     eax
	byte_loop_entry:
		test    eax, 3                                      // use only eax for 'test reg, imm'
		jnz     byte_loop
		and     edi, PAGE_SIZE - 1

		align   16
	dword_loop:
		test    eax, 15                                     // use only eax for 'test reg, imm'
		jz      xmmword_loop
	dword_check_cross_pages:
		cmp     edi, PAGE_SIZE - 4
		ja      byte_loop                                   // jump if cross pages
		mov     ecx, dword ptr [eax + esi]
		mov     edx, dword ptr [eax]
		cmp     ecx, edx
		jne     byte_loop                                   // not equal
		add     eax, 4
		sub     ecx, 01010101H
		xor     edx, -1
		lea     edi, [eax + esi]
		and     ecx, 80808080H
		and     edi, PAGE_SIZE - 1
		and     edx, ecx
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

		align   16
	xmmword_loop:
		cmp     edi, PAGE_SIZE - 16
		ja      dword_check_cross_pages                     // jump if cross pages
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
		lea     edi, [eax + esi + 16]
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

// generic version
__declspec(naked) static int __cdecl strcmpGeneric(const char *string1, const char *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    ebx
		push    esi
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
		lea     ebx, [eax + esi + 1]
		inc     eax
	byte_loop_entry:
		test    eax, 3                                      // use only eax for 'test reg, imm'
		jnz     byte_loop
		and     ebx, PAGE_SIZE - 1

		align   16
	dword_loop:
		cmp     ebx, PAGE_SIZE - 4
		ja      byte_loop                                   // jump if cross pages
		mov     ecx, dword ptr [eax + esi]
		mov     edx, dword ptr [eax]
		cmp     ecx, edx
		jne     byte_loop                                   // not equal
		add     eax, 4
		sub     edx, 01010101H
		xor     ecx, -1
		lea     ebx, [eax + esi]
		and     edx, 80808080H
		and     ebx, PAGE_SIZE - 1
		and     edx, ecx
		jz      dword_loop
	return_equal:
		xor     eax, eax
		pop     esi
		pop     ebx
		ret

		align   16
	return_not_equal:
		sbb     eax, eax
		pop     esi
		or      eax, 1
		pop     ebx
		ret

		#undef string1
		#undef string2
	}
}

// CPU dispatching for strcmp. This is executed only once
__declspec(naked) static int __cdecl strcmpCPUDispatch(const char *string1, const char *string2)
{
	#define __ISA_AVAILABLE_X86   0
	#define __ISA_AVAILABLE_SSE2  1
	#define __ISA_AVAILABLE_SSE42 2

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_SSE2
		jbe     L1
		mov     dword ptr [strcmpDispatch], offset strcmpSSE42
		jmp     strcmpSSE42

	L1:
		mov     dword ptr [strcmpDispatch], offset strcmpSSE2
		jb      L2
		jmp     strcmpSSE2

	L2:
		mov     dword ptr [strcmpDispatch], offset strcmpGeneric
		jmp     strcmpGeneric
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
}
#endif
