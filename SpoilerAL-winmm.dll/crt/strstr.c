#include <string.h>
#include "PageSize.h"

#ifndef _M_IX86
char * __cdecl strstr(const char *haystack, const char *needle)
{
	size_t i;

	if (!*needle)
		return (char *)haystack;
	for (; *haystack; haystack++)
		for (i = 0; ; i++)
			if (!needle[i])
				return (char *)haystack;
			else if (needle[i] != haystack[i])
				break;
	return NULL;
}
#else
static char * __cdecl strstrSSE42(const char *string1, const char *string2);
static char * __cdecl strstrSSE2(const char *string1, const char *string2);
static char * __cdecl strstrGeneric(const char *string1, const char *string2);
static char * __cdecl strstrCPUDispatch(const char *string1, const char *string2);

// Pointer to appropriate version. Initially points to dispatcher
static char *(__cdecl *strstrDispatch)(const char *string1, const char *string2) = strstrCPUDispatch;

// function dispatching
__declspec(naked) char * __cdecl strstr(const char *string1, const char *string2)
{
	__asm
	{
		jmp     dword ptr [strstrDispatch]                  // Go to appropriate version, depending on instruction set
	}
}

// SSE4.2 version
__declspec(naked) static char * __cdecl strstrSSE42(const char *string1, const char *string2)
{
	__asm
	{
		#define haystack (esp + 4)
		#define needle   (esp + 8)

		mov     edx, dword ptr [needle]                     // needle (the string to be searched for)
		xor     ecx, ecx
		mov     cl, byte ptr [edx]                          // ecx contains first char from needle
		mov     eax, dword ptr [haystack]                   // haystack (the string to be searched)
		mov     edx, ecx                                    // set 2 bytes of ecx to first char
		push    esi                                         // preserve esi
		shl     ecx, 8
		lea     esi, [eax - 1]                              // haystack - 1
		or      ecx, edx                                    // is needle empty?
		jz      empty_needle                                // if so, return haystack (ANSI mandated)
		push    edi                                         // preserve edi
		movd    xmm2, ecx                                   // set all bytes of xmm2 to first char
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2

		// find the first character of needle in the haystack by doing linear scan
		align   16
	find_first_char:
		inc     esi
		or      edx, -1
		mov     ecx, esi
		and     esi, -16
		and     ecx, 15
		jz      xmmword_find_loop_entry
		shl     edx, cl
		jmp     xmmword_find_loop_entry

		align   16
	xmmword_find_loop:
		add     esi, 16
		or      edx, -1
	xmmword_find_loop_entry:
		movdqa  xmm0, xmmword ptr [esi]
		pxor    xmm1, xmm1
		pcmpeqb xmm1, xmm0
		pcmpeqb xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb eax, xmm0
		and     eax, edx
		jz      xmmword_find_loop
		bsf     eax, eax
		mov     cl, byte ptr [esi + eax]                    // cl is char from haystack
		add     esi, eax                                    // increment pointer into haystack
		cmp     cl, 0                                       // end of haystack?
		je      not_found                                   // yes, and no match has been found

		// check if remaining consecutive characters match continuously
		mov     eax, dword ptr [needle + 8]
		mov     edi, esi
		test    eax, 15
		jz      xmmword_compare_loop_entry

		align   16                                          // already aligned
	byte_compare_loop:
		inc     eax
		inc     edi
		test    eax, 15                                     // use only eax for 'test reg, imm'
		jz      xmmword_compare_loop_entry
	byte_compare_loop_entry:
		mov     cl, byte ptr [eax]
		mov     dl, byte ptr [edi]
		test    cl, cl
		jz      found
		cmp     cl, dl
		je      byte_compare_loop
		jmp     find_first_char

		align   8
		xchg    ax, ax                                      // padding 2 byte
	xmmword_compare_loop_entry:
		sub     edi, 16
		sub     eax, 16

		align   16                                          // already aligned
	xmmword_compare_loop:
		mov     ecx, PAGE_SIZE - 1
		add     edi, 16
		and     ecx, edi
		add     eax, 16
		cmp     ecx, PAGE_SIZE - 16
		ja      byte_compare_loop_entry                     // jump if cross pages
		movdqa  xmm0, xmmword ptr [eax]                     // read 16 bytes of needle
		pcmpistri xmm0, xmmword ptr [edi], 00011000B        // unsigned bytes, equal each, invert. returns index in ecx
		jnbe    xmmword_compare_loop                        // jump if not carry flag and not zero flag
		cmp     byte ptr [eax + ecx], 0
		jne     find_first_char
		jmp     found

		align   16
	not_found:
		xor     esi, esi
	found:
		pop     edi
		mov     eax, esi
	empty_needle:
		pop     esi
		ret

		#undef haystack
		#undef needle
	}
}

// SSE2 version
__declspec(naked) static char * __cdecl strstrSSE2(const char *string1, const char *string2)
{
	__asm
	{
		#define haystack (esp + 4)
		#define needle   (esp + 8)

		mov     edx, dword ptr [needle]                     // needle (the string to be searched for)
		xor     ecx, ecx
		mov     cl, byte ptr [edx]                          // ecx contains first char from needle
		mov     eax, dword ptr [haystack]                   // haystack (the string to be searched)
		mov     edx, ecx                                    // set 2 bytes of ecx to first char
		push    esi                                         // preserve esi
		shl     ecx, 8
		lea     esi, [eax - 1]                              // haystack - 1
		or      ecx, edx                                    // is needle empty?
		jz      empty_needle                                // if so, return haystack (ANSI mandated)
		push    edi                                         // preserve edi
		movd    xmm2, ecx                                   // set all bytes of xmm2 to first char
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		pxor    xmm3, xmm3                                  // set to zero

		// find the first character of needle in the haystack by doing linear scan
		align   16
	find_first_char:
		inc     esi
		or      edx, -1
		mov     ecx, esi
		and     esi, -16
		and     ecx, 15
		jz      xmmword_find_loop_entry
		shl     edx, cl
		jmp     xmmword_find_loop_entry

		align   16
	xmmword_find_loop:
		add     esi, 16
		or      edx, -1
	xmmword_find_loop_entry:
		movdqa  xmm0, xmmword ptr [esi]
		pxor    xmm1, xmm1
		pcmpeqb xmm1, xmm0
		pcmpeqb xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb eax, xmm0
		and     eax, edx
		jz      xmmword_find_loop
		bsf     eax, eax
		mov     cl, byte ptr [esi + eax]                    // cl is char from haystack
		add     esi, eax                                    // increment pointer into haystack
		cmp     cl, 0                                       // end of haystack?
		je      not_found                                   // yes, and no match has been found

		// check if remaining consecutive characters match continuously
		mov     eax, dword ptr [needle + 8]
		mov     edi, esi
		test    eax, 15
		jz      xmmword_compare_loop_entry

		align   16                                          // already aligned
	byte_compare_loop:
		inc     eax
		inc     edi
		test    eax, 15                                     // use only eax for 'test reg, imm'
		jz      xmmword_compare_loop_entry
	byte_compare_loop_entry:
		mov     cl, byte ptr [eax]
		mov     dl, byte ptr [edi]
		test    cl, cl
		jz      found
		cmp     cl, dl
		je      byte_compare_loop
		jmp     find_first_char

		align   8
		xchg    ax, ax                                      // padding 2 byte
	xmmword_compare_loop_entry:
		sub     edi, 16
		sub     eax, 16

		align   16                                          // already aligned
	xmmword_compare_loop:
		mov     ecx, PAGE_SIZE - 1
		add     edi, 16
		and     ecx, edi
		add     eax, 16
		cmp     ecx, PAGE_SIZE - 16
		ja      byte_compare_loop_entry                     // jump if cross pages
		movdqu  xmm0, xmmword ptr [edi]
		movdqa  xmm1, xmmword ptr [eax]
		pcmpeqb xmm0, xmm1
		pcmpeqb xmm1, xmm3
		pmovmskb ecx, xmm0
		pmovmskb edx, xmm1
		xor     ecx, 0FFFFH
		or      ecx, edx
		jz      xmmword_compare_loop
		bsf     ecx, ecx
		cmp     byte ptr [eax + ecx], 0
		jne     find_first_char
		jmp     found

		align   16
	not_found:
		xor     esi, esi
	found:
		pop     edi
		mov     eax, esi
	empty_needle:
		pop     esi
		ret

		#undef haystack
		#undef needle
	}
}

// generic version
__declspec(naked) static char * __cdecl strstrGeneric(const char *string1, const char *string2)
{
	__asm
	{
		#define haystack (esp + 4)
		#define needle   (esp + 8)

		mov     eax, dword ptr [haystack]                   // haystack
		mov     edx, dword ptr [needle]                     // needle
		mov     cl, byte ptr [edx]
		inc     edx
		test    cl, cl
		jz      EmptyNeedle                                 // a zero-length needle is always found
		push    ebx
		push    esi
		push    edi
		jmp     SearchLoop

		align   16
	SearchLoop:
		// search for first character match
		mov     bl, byte ptr [eax]
		inc     eax
		test    bl, bl
		jz      NotFound                                    // end of haystack reached without finding
		cmp     bl, cl
		jne     SearchLoop                                  // loop through haystack

		// first character match
		dec     eax                                         // begin of match position
		mov     edi, edx
		mov     esi, eax
		sub     edi, eax

	MatchLoop:
		mov     bl, byte ptr [edi + esi]
		inc     esi
		test    bl, bl
		jz      Found                                       // end of needle. match ok
		cmp     bl, byte ptr [esi]
		je      MatchLoop

		// match failed, recover and continue
		inc     eax
		jmp     SearchLoop

		align   16
	NotFound:
		// needle not found. return NULL
		xor     eax, eax
	Found:
		// needle found. return pointer to position in haystack
		pop     edi
		pop     esi
		pop     ebx
	EmptyNeedle:
		ret

		#undef haystack
		#undef needle
	}
}

// CPU dispatching for strstr. This is executed only once
__declspec(naked) static char * __cdecl strstrCPUDispatch(const char *string1, const char *string2)
{
	#define __ISA_AVAILABLE_X86   0
	#define __ISA_AVAILABLE_SSE2  1
	#define __ISA_AVAILABLE_SSE42 2

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_SSE2
		jbe     L1
		mov     dword ptr [strstrDispatch], offset strstrSSE42
		jmp     strstrSSE42

	L1:
		mov     dword ptr [strstrDispatch], offset strstrSSE2
		jb      L2
		jmp     strstrSSE2

	L2:
		mov     dword ptr [strstrDispatch], offset strstrGeneric
		jmp     strstrGeneric
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
}
#endif
